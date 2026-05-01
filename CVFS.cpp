////////////////////////////////////////////////////////////////////////
//
//  Name:        CVFS.cpp 
//  Description: Project 3 - CVFS  
//  Author:      Pradhumnya Changdev Kalsait.
//  Date:        15/08/25
//
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// 
// Header File Inclusion
//
////////////////////////////////////////////////////////////////////

#include<iostream>

#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<strings.h>

using namespace std;

////////////////////////////////////////////////////////////////////
// 
// User Define Macros
//
////////////////////////////////////////////////////////////////////

# define MAXFILESIZE 100                //Max File Size

# define MAXINODE 5                     //Max No Of File Can Create

# define MAX_OPENED_FILES (MAXINODE*4)  //Max No. Of File Opened

# define READ 1
# define WRITE 2
# define EXECUTE 4

# define REGULARFILE 1
# define SPECIALFILE 2

# define START 0
# define CURRENT 1
# define END 2

# define EXECUTE_SUCCESS 0

////////////////////////////////////////////////////////////////////
// 
// User Define Macros for Error Handling
//
////////////////////////////////////////////////////////////////////

# define ERR_INVALID_PARAMETER -1
# define ERR_NO_INODES -2
# define ERR_FILE_ALREADY_EXISTS -3
# define ERR_NO_EMPTY_INODE_FOUND -4
# define ERR_FILE_NOT_EXISTS -5
# define  ERR_PERMISSION_DENIED -6
# define ERR_INSUFFICIENT_SPACE -7
# define ERR_INSUFFICIENT_DATA -8

////////////////////////////////////////////////////////////////////
// 
// Structure Name : BootBlock
// Description    : Holds the information of the Boot Of OS
//
////////////////////////////////////////////////////////////////////
struct BootBlock
{
    char Information[100];
};


////////////////////////////////////////////////////////////////////
// 
// Structure Name : SuperBlock
// Description    : Holds the information about file System
//
////////////////////////////////////////////////////////////////////
struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;

};


////////////////////////////////////////////////////////////////////
// 
// Structure Name : Inode
// Description    : Holds the information about the File 
//
////////////////////////////////////////////////////////////////////
typedef struct Inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    int LinkCount;
    char * Buffer;
    struct Inode * next;

}INODE,*PINODE,**PPINODE;

////////////////////////////////////////////////////////////////////
// 
// Structure Name : FileTable
// Description    : Holds the information about opened File 
//
////////////////////////////////////////////////////////////////////
typedef struct FileTable
{

    int ReadOffset;
    int WriteOffset;
    int Count;
    int MODE;
    PINODE ptrinode;

}FILETABLE,*PFILETABLE;

////////////////////////////////////////////////////////////////////
// 
// Structure Name : UAREA
// Description    : Holds the information about Process 
//
////////////////////////////////////////////////////////////////////
struct UAREA
{
    char ProcessName[50];
    PFILETABLE UFDT[MAX_OPENED_FILES];
};


////////////////////////////////////////////////////////////////////
// 
//  Global VAriables and Object used in Project 
//
////////////////////////////////////////////////////////////////////

SuperBlock superobj;
PINODE head = NULL;
UAREA ureaobj;
BootBlock Bootbobj;

////////////////////////////////////////////////////////////////////
//
// Function Name  : InitialiseSuperBlock
// Description    : USed to initaialise contents of Super Block
// Author         : Pradhumnya Changdev Kalsait
// Date           : 10/08/25
//
////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()         //Auxillary Data
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;
    cout<<"CVFS Super Block Initialised Successfully\n";
}

///////////////////////////////////////////////////////////////////
//
// Function Name  : CreateDILB
// Description    : USed to create the linked list of Inode
// Author         : Pradhumnya Changdev Kalsait
// Date           : 10/08/25
//
////////////////////////////////////////////////////////////////////
void CreateDILB()
{
    int iCnt = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(iCnt <= MAXINODE)
    {
        newn = new INODE;
        newn -> InodeNumber = iCnt;
        newn -> FileSize = 0;
        newn -> ActualFileSize = 0; 
        newn -> LinkCount = 0 ; 
        newn -> Permission = 0;
        newn -> Buffer = NULL;   
        newn -> next = NULL;
        newn -> FileType = 0;
        newn -> ReferenceCount = 0;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp -> next = newn;
            temp = temp -> next;
        }
        iCnt++;
   }

   cout<<"CVFS : DILB created Successfully\n";
}


///////////////////////////////////////////////////////////////////
//
// Function Name  : InitialiseUAREA
// Description    : USed to  initialise UAREA
// Author         : Pradhumnya Changdev Kalsait
// Date           : 10/08/25
//
////////////////////////////////////////////////////////////////////
void InitialiseUAREA()
{
    strcpy(ureaobj.ProcessName,"myexe");

    int iCnt = 0;
    for(iCnt = 0; iCnt < MAX_OPENED_FILES ; iCnt++)
    {
        ureaobj.UFDT[iCnt] = NULL;
    }
    cout<<"CVFS : UAREA Initialised Successfully\n";
}

////////////////////////////////////////////////////////////////////
//
// Function Name  : StartAuxillaryDataInitialisation
// Description    : USed to initaialise the AuxillaryData
// Author         : Pradhumnya Changdev Kalsait
// Date           : 10/08/25
//
////////////////////////////////////////////////////////////////////
void StartAuxillaryDataInitialisation()
{
    strcpy(Bootbobj.Information , "Boot Process of Operating System is Done\n");
    cout<<Bootbobj.Information;

    InitialiseSuperBlock();

    CreateDILB();
    
    InitialiseUAREA();

    cout<<"CVFS : Auxillary Data Initialised Successfully\n";

}
////////////////////////////////////////////////////////////////////
//
// Function Name  : DisplayHelp
// Description    : Used to Display the Information about commands
// Author         : Pradhumnya Changdev Kalsait
// Date           : 11/08/25
//
////////////////////////////////////////////////////////////////////
void DisplayHelp()
{

    printf("--------------------------------------------------------\n");
    printf("---------- Command Manual of CVFS ------------\n");
    printf("--------------------------------------------------------\n");
    printf(" man      : Display the specific manual page of a command\n");
    printf(" clear    : Clear the console of CVFS\n");
    printf(" creat    : Create a new file in CVFS\n");
    printf(" open     : Open an existing file\n");
    printf(" close    : Close an opened file by descriptor\n");
    printf(" closeall : Close all opened files\n");
    printf(" read     : Read data from a file\n");
    printf(" write    : Write data into a file\n");
    printf(" ls       : List all files in the virtual file system\n");
    printf(" stat     : Display statistical information about a file\n");
    printf(" fstat    : Display file info using file descriptor\n");
    printf(" truncate : Remove all content from a file\n");
    printf(" rm       : Delete an existing file\n");
    printf(" exit     : Terminate the shell of CVFS\n");
    printf(" help     : Display this command manual\n");
    printf("--------------------------------------------------------\n");

}
////////////////////////////////////////////////////////////////////
//
// Function Name  : IsFileExists
// Description    : Used to check given named file exists or not
// Input          : It Accepts The File Name 
// Output         : Returns boolean value
// Author         : Pradhumnya Changdev Kalsait
// Date           : 11/08/25
//
////////////////////////////////////////////////////////////////////
bool IsFileExists(
                    char *name      // Name of File That We Wanna Check
                )
{
    PINODE temp = head;
    bool bFlag = false;
    while(temp != NULL)
    {
        if((strcmp(name,temp->FileName) == 0) && (temp -> FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }
    return bFlag; 
}
////////////////////////////////////////////////////////////////////
//
// Function Name  : CreateFile
// Description    : Used to create to new regular file
// Input          : It Accepts The File Name And Permissions
// Output         : Returns File Descriptor
// Author         : Pradhumnya Changdev Kalsait
// Date           : 11/08/25
//
////////////////////////////////////////////////////////////////////
int CreateFile(
                char *name ,        //Name Of File
                int permission      //Permission To Create File
            )
{
    //Filters
    // File Name is Missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    // Entered Permission is invalid
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;   
    }
    //Check Whether empty inode is There or not
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }
    // Check The File Exists or Not
    if(IsFileExists(name))
    {   
        return ERR_FILE_ALREADY_EXISTS;
    }


    //Loop to search free inode
    PINODE temp = head;
    int iCnt = 0;

    while(temp != NULL)
    {
        if(temp -> FileType == 0)
        {
            break;
        }
        temp = temp -> next;
    }

    //Inode Not Found
    if(temp == NULL)
    {
        printf("iNode Not Found\n");
        return ERR_NO_EMPTY_INODE_FOUND;
    }


    //Search first  NULL from UFDT
    for(iCnt = 0 ; iCnt < MAX_OPENED_FILES ; iCnt++)
    {
        if(ureaobj.UFDT[iCnt] == NULL)
        {
            break;
        }
    }

    if(iCnt == MAX_OPENED_FILES)
    {
        printf("Unable to Create file As maxed opened file limit reached\n");
        return ERR_NO_EMPTY_INODE_FOUND;
    }

    ureaobj.UFDT[iCnt] = (PFILETABLE)malloc(sizeof(FILETABLE));
    ureaobj.UFDT[iCnt] ->Count = 1;
    ureaobj.UFDT[iCnt] ->ReadOffset = 0;
    ureaobj.UFDT[iCnt] ->WriteOffset = 0;
    ureaobj.UFDT[iCnt] ->MODE = permission;


    //connect filetable with IIT
    ureaobj.UFDT[iCnt] ->ptrinode = temp;

    strcpy(ureaobj.UFDT[iCnt] ->ptrinode->FileName,name);
    ureaobj.UFDT[iCnt] ->ptrinode->FileSize = MAXFILESIZE;
    ureaobj.UFDT[iCnt] ->ptrinode->ActualFileSize = 0;
    ureaobj.UFDT[iCnt] ->ptrinode->FileType = REGULARFILE;
    ureaobj.UFDT[iCnt] ->ptrinode->ReferenceCount = 1;
    ureaobj.UFDT[iCnt] ->ptrinode->LinkCount = 1;
    ureaobj.UFDT[iCnt] ->ptrinode->Permission = permission;
    

    //Allocate memory for Buffer
    ureaobj.UFDT[iCnt] ->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);

    // Decrement the number of free inode by 1
    superobj.FreeInodes--;

    return iCnt;

}

////////////////////////////////////////////////////////////////////
//
// Function Name  : ManPage
// Description    : Used to Display the Manual Page of the commands
// Input          : It Accepts The Command Name
// Output         : Displays The Manual Details Of the Commands
// Author         : Pradhumnya Changdev Kalsait
// Date           : 11/08/25
//
////////////////////////////////////////////////////////////////////
void ManPage(char *name)   // Name Of Command
{
    if(strcasecmp(name,"creat") == 0)
    {
        printf("Description : This Command is used to create a new regular file in the\n");
        printf("              Virtual File System. If the file already exists,\n");
        printf("              it will not be overwritten.\n");
        printf("Usage       : creat File_Name Permissions \n");
        printf("Arguments   :\n");
        printf("   File_Name    : The name of the file that you want to create.\n");
        printf("   Permissions  :\n");
        printf("                  1 : Read Only\n");
        printf("                  2 : Write Only\n");
        printf("                  3 : Read + Write\n");
    }
    else if(strcasecmp(name,"exit") == 0)
    {
        printf("Description : Terminates the Virtual File System and frees\n");
        printf("              all allocated resources before shutting down.\n");
        printf("Usage       : exit \n");   
    }
    else if(strcasecmp(name,"unlink") == 0 || strcasecmp(name,"rm") == 0)
    {
        printf("Description : Deletes a regular file from the Virtual File System.\n");
        printf("              The file and its data are permanently removed.\n");
        printf("Usage       : rm File_Name  \n");
        printf("Arguments   :\n");
        printf("   File_Name : The name of the file that you want to delete.\n");
    }
    else if(strcasecmp(name,"stat") == 0)
    {
        printf("Description : Displays detailed information about a file such as its\n");
        printf("              name, size, permissions, type, and inode details.\n");
        printf("Usage       : stat File_Name  \n");
        printf("Arguments   :\n");
        printf("   File_Name : The name of the file that you want information about.\n");
    }
    else if(strcasecmp(name,"fstat") == 0)
    {
        printf("Description : Displays file information using its File Descriptor.\n");
        printf("Usage       : fstat FD \n");
        printf("Arguments   :\n");
        printf("   FD : File Descriptor of the file.\n");
    }
    else if(strcasecmp(name,"ls") == 0)
    {
        printf("Description : Lists all the files currently stored in the Virtual\n");
        printf("              File System, along with inode number and sizes.\n");
        printf("Usage       : ls \n");
    }
    else if(strcasecmp(name, "write") == 0)
    {
        printf("Description : Writes data into an opened file in the Virtual File System.\n");
        printf("              The write operation appends to the existing data based on\n");
        printf("              the file's current write offset.\n");
        printf("Usage       : write FD  \n");
        printf("Arguments   :\n");
        printf("   FD : File Descriptor of the file (obtained while opening the file).\n");
    }
    else if(strcasecmp(name, "read") == 0)
    {
        printf("Description : Reads data from an opened file in the Virtual File System.\n");
        printf("              The data is read sequentially based on the file's read\n");
        printf("              offset and the specified size.\n");
        printf("Usage       : read FD Size  \n");
        printf("Arguments   :\n");
        printf("   FD   : File Descriptor of the file.\n");
        printf("   Size : Number of bytes to read from the file.\n");
    }
    else if(strcasecmp(name, "open") == 0)
    {
        printf("Description : Opens an existing file in the Virtual File System with\n");
        printf("              specified permissions and returns a File Descriptor.\n");
        printf("Usage       : open File_Name Mode \n");
        printf("Arguments   :\n");
        printf("   File_Name : The name of the file to open.\n");
        printf("   Mode      :\n");
        printf("               1 : Read Only\n");
        printf("               2 : Write Only\n");
        printf("               3 : Read + Write\n");
    }
    else if(strcasecmp(name, "close") == 0)
    {
        printf("Description : Closes an opened file and releases its file descriptor.\n");
        printf("Usage       : close FD \n");
        printf("Arguments   :\n");
        printf("   FD : File Descriptor of the file to be closed.\n");
    }
    else if(strcasecmp(name, "closeall") == 0)
    {
        printf("Description : Closes all currently opened files.\n");
        printf("Usage       : closeall \n");
    }
    else if(strcasecmp(name, "truncate") == 0)
    {
        printf("Description : Removes all the data from the specified file but keeps\n");
        printf("              the file entry and its metadata intact.\n");
        printf("Usage       : truncate File_Name \n");
        printf("Arguments   :\n");
        printf("   File_Name : The name of the file whose content you want to erase.\n");
    }
    else if(strcasecmp(name, "help") == 0)
    {
        printf("Description : Displays a list of all supported commands in the\n");
        printf("              Virtual File System along with a short description.\n");
        printf("Usage       : help \n");
    }
    else
    {
        printf("No manual entry for '%s'\n",name);
    }
}


////////////////////////////////////////////////////////////////////
//
// Function Name  : unlinkFile
// Description    : Used to delete a regular file
// Input          : It Accepts The File Name And Permissions
// Output         : Returns nothing
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////
int UnlinkFile(
                   char * name      //Name of the file
                )
{
    // Filters
    // No Name Given
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    // File Not Exists
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // First close any open UFDT entry for this file
    int iCnt = 0;
    for(iCnt = 0 ; iCnt < MAX_OPENED_FILES ; iCnt++)
    {
        if(ureaobj.UFDT[iCnt] != NULL)
        {
            if(strcmp(ureaobj.UFDT[iCnt] -> ptrinode -> FileName, name) == 0)
            {
                free(ureaobj.UFDT[iCnt]);
                ureaobj.UFDT[iCnt] = NULL;
                break;
            }
        }
    }

    // Now reset the inode directly from the DILB
    PINODE temp = head;
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0 && temp->FileType == REGULARFILE)
        {
            if(temp->Buffer != NULL)
            {
                free(temp->Buffer);
                temp->Buffer = NULL;
            }
            temp->FileName[0] = '\0';
            temp->FileSize = 0;
            temp->ActualFileSize = 0;
            temp->FileType = 0;
            temp->ReferenceCount = 0;
            temp->LinkCount = 0;
            temp->Permission = 0;

            superobj.FreeInodes++;
            break;
        }
        temp = temp->next;
    }

    return EXECUTE_SUCCESS;

}
////////////////////////////////////////////////////////////////////
//
// Function Name  : ls_file
// Description    : Used to Display All the information all regular file
// Input          : nothing
// Output         : nothing
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////
void ls_file()
{
    PINODE temp = head;
    int fileCount = 0;

    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            fileCount++;
        }
        temp = temp->next;
    }

    if(fileCount == 0)
    {
        printf("No files present in the Virtual File System.\n");
        return;
    }

    printf("-------------------------------------------------------------\n");
    printf("%-20s %-15s %-12s %-15s\n", "File Name", "Inode Number", "File Size", "Actual File Size");
    printf("-------------------------------------------------------------\n");

    temp = head;
    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%-20s %-15d %-12d %-15d\n",
                   temp->FileName,
                   temp->InodeNumber,
                   temp->FileSize,
                   temp->ActualFileSize);
        }
        temp = temp->next;
    }
    printf("-------------------------------------------------------------\n");

}

////////////////////////////////////////////////////////////////////
//
// Function Name  : stat_file
// Description    : Used to Display about the information of regular file
// Input          : nothing
// Output         : nothing
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////

// CVFS > stat demo.txt
int stat_file(
                   char * name      //Name of the file
                )
{
    // Filters
    // No Name Given
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    // File Not Exists
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    PINODE temp = head;
    while(temp != NULL)
    {
        if(strcmp(name,temp->FileName) == 0 && (temp->FileType != 0))
        {
            printf("----------Statistical Information of File-------------\n");

            printf("File Name : %s\n",temp->FileName);
            printf("Filse Size On Disk : %d\n",temp->FileSize);
            printf("Actual File Size:%d\n",temp -> ActualFileSize);
            printf("Link Count : %d\n",temp -> LinkCount);
            printf("File Permission:");
            if(temp->Permission == READ)
            {
                printf("Read\n");
            }
            else if(temp->Permission == WRITE)
            {
                printf("Write\n");
            }
            else if(temp->Permission == READ + WRITE)
            {
                printf("Read + Write\n");
            }
            printf("File Type:");
            if(temp ->FileType == REGULARFILE)
            {
                printf("Regular File\n");
            }
             if(temp ->FileType == SPECIALFILE)
            {
                printf("Special File\n");
            }
            cout<<"--------------------------------------------------------\n";
        }

        temp = temp -> next;
    }
    return EXECUTE_SUCCESS;

}

////////////////////////////////////////////////////////////////////
//
// Function Name  : write_file
// Description    : Used to write the contents into the regular file
// Input          : File Decriptor,
//                  Address Of Buffer While contains the data
//                  Size of Data that we want to write
// Output         : Number of Bytes Written Successfully
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////

// CVFS > write 3
int write_file(
                int fd,             //File Descriptor
                char * data,        // Data  Which Has to be written
                int Size            // Size of the data That Has to write into file
            )
{   
    //Invalid Value of FD
    if(fd < 0 || fd >= MAX_OPENED_FILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    //File Is Not Opened Or Created With the Given FD
    if(ureaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }
    // If There is no Permission to Write the data
    if(!(ureaobj.UFDT[fd] -> ptrinode -> Permission & WRITE))
    {
        return ERR_PERMISSION_DENIED;
    }
    //Unable to write As there is no Sufficient Space
    if((MAXFILESIZE - ureaobj.UFDT[fd] ->WriteOffset) < Size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    strncpy(ureaobj.UFDT[fd] -> ptrinode -> Buffer + ureaobj.UFDT[fd] -> WriteOffset, data, Size);
    
    //Update the WriteOffset
    ureaobj.UFDT[fd] -> WriteOffset = ureaobj.UFDT[fd] -> WriteOffset  + Size;

    // Update The Actual Size
    ureaobj.UFDT[fd] -> ptrinode -> ActualFileSize = ureaobj.UFDT[fd] -> WriteOffset;

    return Size;
}


////////////////////////////////////////////////////////////////////
//
// Function Name  : read_file
// Description    : Used to read the contents from the regular file
// Input          : File Decriptor,
//                  Address Of Empty Buffer 
//                  Size of Data that we want to read
// Output         : Number of Bytes Written Successfully
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////

// CVFS > read 3 10
int read_file(
                int fd,             //File Descriptor
                char * data,        // Empty Buffer
                int Size            // Size of the data That Has to read from file
            )
{   
   
    
    //Invalid Value of FD
    if(fd < 0 || fd >= MAX_OPENED_FILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL || Size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }
    //File Is Not Opened Or Created With the Given FD
    if(ureaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }
    // If There is no Permission to Read the data
    if(!(ureaobj.UFDT[fd] -> ptrinode -> Permission & READ))
    {
        return ERR_PERMISSION_DENIED;
    }
    // Check that there is enough data available to read
    int availableData = ureaobj.UFDT[fd]->ptrinode->ActualFileSize - ureaobj.UFDT[fd]->ReadOffset;
    if(availableData < Size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Copy The Data Into Buffer; caller must allocate at least Size+1 bytes
    strncpy(data, (ureaobj.UFDT[fd]->ptrinode->Buffer + ureaobj.UFDT[fd]->ReadOffset), Size);
    data[Size] = '\0';

    // Update the ReadOffset
    ureaobj.UFDT[fd] -> ReadOffset = ureaobj.UFDT[fd] -> ReadOffset + Size;
    

    return Size;
}

////////////////////////////////////////////////////////////////////
//
// Function Name  : OpenFile
// Description    : Used to open an existing file with given mode
// Input          : File Name, Mode (Read/Write/Read+Write)
// Output         : Returns File Descriptor or error code
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////

// CVFS > open demo.txt 3
int OpenFile(
                char *name,     // Name of the file to open
                int mode        // Mode: READ(1), WRITE(2), READ+WRITE(3)
            )
{
    // Filters
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    if(mode < 1 || mode > 3)
    {
        return ERR_INVALID_PARAMETER;
    }
    // File must exist
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Find the inode for this file
    PINODE temp = head;
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0 && temp->FileType == REGULARFILE)
        {
            break;
        }
        temp = temp->next;
    }

    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Check permission compatibility
    if((mode & READ) && !(temp->Permission & READ))
    {
        return ERR_PERMISSION_DENIED;
    }
    if((mode & WRITE) && !(temp->Permission & WRITE))
    {
        return ERR_PERMISSION_DENIED;
    }

    // Find a free UFDT slot
    int iCnt = 0;
    for(iCnt = 0; iCnt < MAX_OPENED_FILES; iCnt++)
    {
        if(ureaobj.UFDT[iCnt] == NULL)
        {
            break;
        }
    }

    if(iCnt == MAX_OPENED_FILES)
    {
        return ERR_NO_EMPTY_INODE_FOUND;
    }

    ureaobj.UFDT[iCnt] = (PFILETABLE)malloc(sizeof(FILETABLE));
    ureaobj.UFDT[iCnt]->Count = 1;
    ureaobj.UFDT[iCnt]->ReadOffset = 0;
    ureaobj.UFDT[iCnt]->WriteOffset = temp->ActualFileSize; // append writes at end
    ureaobj.UFDT[iCnt]->MODE = mode;
    ureaobj.UFDT[iCnt]->ptrinode = temp;

    temp->ReferenceCount++;

    return iCnt;
}

////////////////////////////////////////////////////////////////////
//
// Function Name  : CloseFile
// Description    : Used to close an opened file by its file descriptor
// Input          : File Descriptor
// Output         : Returns EXECUTE_SUCCESS or error code
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////

// CVFS > close 3
int CloseFile(int fd)
{
    if(fd < 0 || fd >= MAX_OPENED_FILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(ureaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    ureaobj.UFDT[fd]->ptrinode->ReferenceCount--;

    free(ureaobj.UFDT[fd]);
    ureaobj.UFDT[fd] = NULL;

    return EXECUTE_SUCCESS;
}

////////////////////////////////////////////////////////////////////
//
// Function Name  : CloseAllFiles
// Description    : Used to close all currently opened files
// Input          : Nothing
// Output         : Nothing
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////
void CloseAllFiles()
{
    int iCnt = 0;
    for(iCnt = 0; iCnt < MAX_OPENED_FILES; iCnt++)
    {
        if(ureaobj.UFDT[iCnt] != NULL)
        {
            ureaobj.UFDT[iCnt]->ptrinode->ReferenceCount--;
            free(ureaobj.UFDT[iCnt]);
            ureaobj.UFDT[iCnt] = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////
//
// Function Name  : fstat_file
// Description    : Used to display file information using file descriptor
// Input          : File Descriptor
// Output         : Returns EXECUTE_SUCCESS or error code
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////

// CVFS > fstat 3
int fstat_file(int fd)
{
    if(fd < 0 || fd >= MAX_OPENED_FILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(ureaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    PINODE temp = ureaobj.UFDT[fd]->ptrinode;

    printf("----------Statistical Information of File-------------\n");
    printf("File Descriptor    : %d\n", fd);
    printf("File Name          : %s\n", temp->FileName);
    printf("File Size On Disk  : %d\n", temp->FileSize);
    printf("Actual File Size   : %d\n", temp->ActualFileSize);
    printf("Link Count         : %d\n", temp->LinkCount);
    printf("Reference Count    : %d\n", temp->ReferenceCount);
    printf("File Permission    : ");
    if(temp->Permission == READ)
    {
        printf("Read\n");
    }
    else if(temp->Permission == WRITE)
    {
        printf("Write\n");
    }
    else if(temp->Permission == (READ | WRITE))
    {
        printf("Read + Write\n");
    }
    printf("File Type          : ");
    if(temp->FileType == REGULARFILE)
    {
        printf("Regular File\n");
    }
    else if(temp->FileType == SPECIALFILE)
    {
        printf("Special File\n");
    }
    printf("------------------------------------------------------\n");

    return EXECUTE_SUCCESS;
}

////////////////////////////////////////////////////////////////////
//
// Function Name  : truncate_file
// Description    : Used to remove all content from a file while keeping metadata
// Input          : File Name
// Output         : Returns EXECUTE_SUCCESS or error code
// Author         : Pradhumnya Changdev Kalsait
// Date           : 15/08/25
//
////////////////////////////////////////////////////////////////////

// CVFS > truncate demo.txt
int truncate_file(char *name)
{
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Try to find an open UFDT entry first
    int iCnt = 0;
    for(iCnt = 0; iCnt < MAX_OPENED_FILES; iCnt++)
    {
        if(ureaobj.UFDT[iCnt] != NULL)
        {
            if(strcmp(ureaobj.UFDT[iCnt]->ptrinode->FileName, name) == 0)
            {
                memset(ureaobj.UFDT[iCnt]->ptrinode->Buffer, 0, MAXFILESIZE);
                ureaobj.UFDT[iCnt]->ptrinode->ActualFileSize = 0;
                ureaobj.UFDT[iCnt]->ReadOffset = 0;
                ureaobj.UFDT[iCnt]->WriteOffset = 0;
                return EXECUTE_SUCCESS;
            }
        }
    }

    // File exists but is not open — find it in the DILB
    PINODE temp = head;
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0 && temp->FileType == REGULARFILE)
        {
            if(temp->Buffer != NULL)
            {
                memset(temp->Buffer, 0, MAXFILESIZE);
            }
            temp->ActualFileSize = 0;
            return EXECUTE_SUCCESS;
        }
        temp = temp->next;
    }

    return ERR_FILE_NOT_EXISTS;
}

////////////////////////////////////////////////////////////////////
// 
// Entry Point Function of The Project(Main)
//
////////////////////////////////////////////////////////////////////
int main()
{
    char str[80] = {'\0'};
    int iCount = 0;
    int iRet = 0;
    char Command[5][80];
    char Input_Buffer[MAXFILESIZE] = {'\0'};
    char *EmptyBuffer = NULL;


    StartAuxillaryDataInitialisation();

    cout<<"--------------------------------------------------------\n";
    cout<<"-----------CVFS Started Successfully---------\n";
    cout<<"--------------------------------------------------------\n";

    
//Operating System Shell --> The loop which takes input from user and Perform Operations and Ready to take next Input

    // Infinite Listening Loop 
    while(1)
    {
        fflush(stdin);

        strcpy(str,"");
        printf("\n CVFS > ");
        fgets(str, sizeof(str) , stdin);

        // Clear command array before each parse
        memset(Command, 0, sizeof(Command));
        iCount = sscanf(str ,"%s %s %s %s",Command[0],Command[1],Command[2],Command[3]);
       


        if(iCount == 1)
        {
            // CVFS > exit
            if(strcasecmp(Command[0],"exit") == 0)
            {
                printf("Thank You for Using CVFS \n");
                printf("Deallocating the Resources ...\n");
                break;
            }

            // CVFS > help
            else if(strcasecmp(Command[0],"help") == 0)
            {
                DisplayHelp();
            }

            // CVFS > clear
            else if(strcasecmp(Command[0],"clear") == 0)
            {
                system("clear");
            }

            // CVFS > ls
            else if(strcasecmp(Command[0],"ls") == 0)
            {
                ls_file();
            }

            // CVFS > closeall
            else if(strcasecmp(Command[0],"closeall") == 0)
            {
                CloseAllFiles();
                printf("All files closed successfully.\n");
            }

            else
            {
                printf("Command Not Found..\n");
                printf("Please refer Help option or Use man Command\n");
            }

        }   //End Of if(iCount == 1)
        else if(iCount == 2)
        {
            // CVFS > man creat
            if(strcasecmp(Command[0],"man") == 0)
            {
                ManPage(Command[1]);
            }
            
            // CVFS > rm demo.txt
            else if(strcasecmp(Command[0],"rm") == 0 || strcasecmp(Command[0],"unlink") == 0)
            {
                iRet = UnlinkFile(Command[1]);
                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File '%s' deleted successfully.\n", Command[1]);
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : Unable to delete file. File '%s' does not exist.\n", Command[1]);
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters.\n");
                    printf("Please check man page for more details.\n");
                }
            }

            // CVFS > stat demo.txt
            else if(strcasecmp(Command[0], "stat") == 0)
            {
                iRet = stat_file(Command[1]);
                
                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : Unable to display statistics. File '%s' does not exist.\n", Command[1]);
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters.\n");
                    printf("Please check man page for more details.\n");
                } 
            }

            // CVFS > write 3
            else if(strcasecmp(Command[0], "write") == 0)
            {
                printf("Please enter the data that you want to write into the file: ");
                fflush(stdout);
                memset(Input_Buffer, 0, sizeof(Input_Buffer));
                scanf("%[^\n]", Input_Buffer);
                // Consume the trailing newline left in the input stream
                int c;
                while((c = getchar()) != '\n' && c != EOF) {}

                iRet = write_file(atoi(Command[1]), Input_Buffer, strlen(Input_Buffer));
                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : Unable to write. File not found for the given descriptor.\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters.\n");
                    printf("Please check man page for more details.\n");
                } 
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied. You don't have write permission.\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Insufficient space in the file.\n");
                }
                else
                {
                    printf("%d bytes of data written into file successfully.\n", iRet);
                }
            }

            // CVFS > close 3
            else if(strcasecmp(Command[0], "close") == 0)
            {
                iRet = CloseFile(atoi(Command[1]));
                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File with descriptor %s closed successfully.\n", Command[1]);
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : No file is open with descriptor %s.\n", Command[1]);
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid file descriptor.\n");
                }
            }

            // CVFS > fstat 3
            else if(strcasecmp(Command[0], "fstat") == 0)
            {
                iRet = fstat_file(atoi(Command[1]));
                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : No file is open with descriptor %s.\n", Command[1]);
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid file descriptor.\n");
                }
            }

            // CVFS > truncate demo.txt
            else if(strcasecmp(Command[0], "truncate") == 0)
            {
                iRet = truncate_file(Command[1]);
                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File '%s' truncated successfully.\n", Command[1]);
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File '%s' not found.\n", Command[1]);
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters.\n");
                }
            }

            else
            {
                printf("Command Not Found..\n");
                printf("Please refer Help option or Use man Command\n");
            }

            
        }   //End Of if(iCount == 2)
        else if(iCount == 3)
        {
            // CVFS > creat demo.txt 3
            if(strcasecmp(Command[0],"creat") == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]));
                
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters.\n");
                    printf("Please check man page for more details.\n");
                }
                else if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create file. No free inodes available.\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("Error : File '%s' already exists.\n", Command[1]);
                }
                else if(iRet == ERR_NO_EMPTY_INODE_FOUND)
                {
                    printf("Error : Maximum number of open files reached.\n");
                }
                else
                {
                    printf("File '%s' created successfully. File Descriptor: %d\n", Command[1], iRet);
                }
            }

            // CVFS > open demo.txt 3
            else if(strcasecmp(Command[0],"open") == 0)
            {
                iRet = OpenFile(Command[1], atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters.\n");
                    printf("Please check man page for more details.\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error : File '%s' does not exist.\n", Command[1]);
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied. Requested mode exceeds file permissions.\n");
                }
                else if(iRet == ERR_NO_EMPTY_INODE_FOUND)
                {
                    printf("Error : Maximum number of open files reached.\n");
                }
                else
                {
                    printf("File '%s' opened successfully. File Descriptor: %d\n", Command[1], iRet);
                }
            }

            // CVFS > read 3 10
            else if(strcasecmp(Command[0],"read") == 0)
            {
                int readSize = atoi(Command[2]);
                EmptyBuffer = (char*)malloc(readSize + 1);
                if(EmptyBuffer == NULL)
                {
                    printf("Error : Memory allocation failed.\n");
                }
                else
                {
                    memset(EmptyBuffer, 0, readSize + 1);
                    iRet = read_file(atoi(Command[1]), EmptyBuffer, readSize);
                    if(iRet == ERR_FILE_NOT_EXISTS)
                    {
                        printf("Error : Unable to read. File not found for the given descriptor.\n");
                    }
                    else if(iRet == ERR_INVALID_PARAMETER)
                    {
                        printf("Error : Invalid parameters.\n");
                        printf("Please check man page for more details.\n");
                    } 
                    else if(iRet == ERR_PERMISSION_DENIED)
                    {
                        printf("Error : Permission denied. You don't have read permission.\n");
                    }
                    else if(iRet == ERR_INSUFFICIENT_DATA)
                    {
                        printf("Error : Insufficient data. Not enough bytes available to read.\n");
                    }
                    else
                    {
                        printf("%d bytes read from file.\n", iRet);
                        printf("Data : %s\n", EmptyBuffer);
                    }
                    free(EmptyBuffer);
                    EmptyBuffer = NULL;
                }
            }

            else
            {
                printf("Command Not Found..\n");
                printf("Please refer Help option or Use man Command\n");
            }

            
        }   //End Of if(iCount == 3)
        else if(iCount == 4)
        {
        
            
        }   //End Of if(iCount == 4)
        else
        {
            printf("Command Not Found..\n");
            printf("Please refer Help option or Use man Command\n");

        }   //End of Invalid Command Part

    }//End Of While(Custem Shell)

    return 0;
    
}//End Of Main







/*


Important Concepts/Notation/Functions Which is Used in The Program


1.fflush(stdin);
    Flush the keyboard before the taking new Input


2.fgets(str, sizeof(str) , stdin);
                Used to take Input
                stdin -> keyboard (standred Input)


3.tokanisation of str
    char Command[4][80];
        create demo.txt 3

        create =    [0][80]
        demo.txt =  [1][80]
        3 =         [2][80]

        for tokenization


4.stricmp(Command[0],"exit") == 0)
    to compare the string

5.End Of if(iCount == 4)
    Territory Marking Comments

6.else if(stricmp(Command[0],"clear") == 0)
            {
                system("cls");
            }

            the cls call be done through the system from program that included in header file
            #include<stdlib.h>

7. atoi = convert the string to integer


8. FileType And FileName =


9. line 680 == strncpy(offset , data , Size)
        write the data 

10.  offset = uarea.obj.UFDT[fd] -> ptrinode -> Buffer +  uarea.obj.UFDT[fd] -> writeOffset
*/