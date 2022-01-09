#include <iostream> // for everything
#include <windows.h> // for API
#include <string> // for tring usage
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#define _WIN32_WINNT 0x0501

using namespace std;

//void copyFile(HANDLE firstHandle, HANDLE secondHandle, DWORD blockSize, int operationsCount);

// õýíäëû íà ôàéëû, ó÷àâñòâóþùèå â îïåðàöèè êîïèðîâàíèÿ
HANDLE firstHandle;
HANDLE secondHandle;
// òåêóùèé ñäâèã ñîñòîÿíèÿ êîïèðîâàíèÿ ôàéëà äëÿ îïåðàöèé ÷òåíèÿ è çàïèñè ñîîòâåòñòâåííî
LARGE_INTEGER shiftRead;
LARGE_INTEGER shiftWrite;





int LocalFileGenerator (string localOldFilePath, unsigned long long localBytesRequest);
void CopyPaste (string localOldFilePath, string localNewFilePath);
DWORD PreparingCopyPaste(string localOldFilePath, string localNewFilePath, unsigned long long localBlockSize, unsigned long long localOverlappedIOSize);
DWORD LocalCopyPaste (HANDLE in, HANDLE out, unsigned long long fileSize, unsigned long long bs, unsigned long long localOverlappedIOSize);
DWORD LocalDriveSectorSize ();
DWORD LocalDriveSectorSize (DWORD &localSectorsPerCluster);
//unsigned long long DWORDS2ULL(DWORD l, DWORD h);
//void ULL2DWORDS(unsigned long long value, DWORD* l, DWORD* h);
void CALLBACK CompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

//Certutil -hashfile file

unsigned long long callback;
unsigned long long globalFirstAdress;
unsigned long long oneSize;
unsigned long long callLeft;
unsigned long long copyTime = 0;

unsigned long long bs = 20;

void LocalReadWrite(long long fileSize, DWORD blockSize, int operationsCount, OVERLAPPED* overlappeds, CHAR** buffer, HANDLE fileHandle, char f)
{
    int operations_counter = 0;
    for (int i=0; i<operationsCount; i++)
    {
        if (fileSize>0)
        {
            operations_counter++;
            if (f == 'r')
            {
            	ReadFileEx(fileHandle, buffer[i], blockSize, &overlappeds[i], CompletionRoutine);
            }
            else if (f == 'w')
            {
            	WriteFileEx(fileHandle, buffer[i], blockSize, &overlappeds[i], CompletionRoutine);
            }
            fileSize -= blockSize;
        }
    }
    while (callback < operations_counter)
    {
        SleepEx(-1, true);
    }
    for (int i=0; i<operationsCount; i++)
    {
    	if (f == 'r')
    	{
    		overlappeds[i].Offset = shiftRead.LowPart;
        	overlappeds[i].OffsetHigh = shiftRead.HighPart;
        	shiftRead.QuadPart += blockSize;
    	}
    	else if (f == 'w')
    	{
	        overlappeds[i].Offset = shiftWrite.LowPart;
	        overlappeds[i].OffsetHigh = shiftWrite.HighPart;
	        shiftWrite.QuadPart += blockSize;
    	}
    }
    callback = 0;
}

// ---------- MAIN ----------

int main(int argc, char **argv)
{
    if (argc == 4)
    {
    	const unsigned long oldFileBytes = atoi(argv[2]);
        const string oldFilePath(argv[1]);
        const string newFilePath(argv[3]);
        LocalFileGenerator (oldFilePath, oldFileBytes);
        cout << "Created file number of bytes: " << oldFileBytes << " Created file path: " << oldFilePath << "\n";
        CopyPaste(oldFilePath, newFilePath);
        cout << "Copied file path: " << oldFilePath << "\nPasted file path: " << newFilePath << "\n";

        return 0;
    }
    else
    {
    	cout << "Incorrect start of executable file. Please, check your flags!\n";
        return -1;
    }
}

// ---------- FILE GENERATION FOR COPY TESTS ----------

int LocalFileGenerator (string localOldFilePath, unsigned long long localBytesRequest)
{
	unsigned long long i;
    ofstream localFile (localOldFilePath, ios :: out | ios :: binary | ios :: app);
    srand (time(NULL));
    char localByte[10];
    for (i = 0; i < localBytesRequest; i++)
    {
        localByte[0] = (unsigned char)(rand() % 256);
        localFile.write (localByte, sizeof(localByte));
    }
    localFile.close();
    return 0;
}

// ---------- COPY AND PASTE MAIN PROCESS ----------

void CopyPaste (string localOldFilePath, string localNewFilePath)
{
    //DWORD time;
    DWORD sectorsPerCluster;
    DWORD localSectorSize = LocalDriveSectorSize (sectorsPerCluster);
    unsigned long long localBlockSize; // size of the data block I will copy
    unsigned long long localOverlappedIOSize; // number of operations for Overlapped I/O

    cout << "Drive sector size: " << localSectorSize << " bytes\n";
    cout << "Drive cluster size: " << sectorsPerCluster << " sectors (" << localSectorSize*sectorsPerCluster << " bytes).\n";

    int localBlockSizeIteration = 0; // iterator
    
    localOverlappedIOSize = 1; // overlapped IO number

    localBlockSize = localSectorSize*bs;
    copyTime = 0;
    copyTime = PreparingCopyPaste(localOldFilePath, localNewFilePath, localBlockSize, 1);
    cout << localBlockSizeIteration << ".\n"
    << "Current sector size: " << localSectorSize << ";\n"
    << "Current sector multiplition: " << bs << ";\n"
    << "Current block size to copy: " << localBlockSize << ";\n"
    << "Current overlapped IO number: " << localOverlappedIOSize << ";\n"
    << "Overlapped copy time: " << copyTime << ".\n";
}

// ---------- PREPARING FOR COPY AND PASTE ACTIONS ----------

DWORD PreparingCopyPaste(string localOldFilePath, string localNewFilePath, unsigned long long localBlockSize, unsigned long long localOverlappedIOSize)
{
    DWORD localActionTime = -1;
    
    HANDLE localOldFileHandle = CreateFileA(localOldFilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL); // copied file path
    HANDLE localNewFileHandle = CreateFileA(localNewFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL); // new file path
    DWORD lpFileSizeHigh; // lpdword filesize high
    DWORD getFileSize;
    //unsigned long long fileSize;

    if (localOldFileHandle == NULL || localOldFileHandle == INVALID_HANDLE_VALUE || localNewFileHandle == NULL || localNewFileHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Problem with opening files!\n";
        cout << "\nError message: " << GetLastError() << "\n";
    }
    else
    {
    	DWORD blockSize = 4096*5;
        getFileSize = GetFileSize(localOldFileHandle, &lpFileSizeHigh);
        //fileSize = getFileSize | ((unsigned long long)lpFileSizeHigh << 32);

	    DWORD high = 0;
	    LARGE_INTEGER fileSizeStruct; // where file size will be wrote
	    long long fileSize; // number where we write
	    GetFileSizeEx(localOldFileHandle, &fileSizeStruct); // file size (for input as LARGE_INTEGER we use it!!!)
	    fileSize = fileSizeStruct.QuadPart; // uniting fields one number
	    long long curSize = fileSize;

	    CHAR** buffer = new CHAR*[localOverlappedIOSize]; // buffer of the data
	    for (int i=0; i < localOverlappedIOSize; i++)
	    {
	        buffer[i] = new CHAR[(int)blockSize];
	    }
	    // äîëæíû îñòàâàòüñÿ äîïóñòèìûìè äëÿ äëèòåëüíîé îïåðàöèè ÷òåíèÿ.
	    OVERLAPPED* over_1 = new OVERLAPPED[localOverlappedIOSize]; // var for handling pointers (starting bytes)
	    OVERLAPPED* over_2 = new OVERLAPPED[localOverlappedIOSize]; // new file

	    shiftRead.QuadPart = 0; // how many rode
	    shiftWrite.QuadPart = 0; // how many write

	    for (int i=0; i < localOverlappedIOSize; i++)
	    {
	        over_1[i].Offset = over_2[i].Offset = shiftRead.LowPart; // FIRST PART OF THE STRUCTURE
	        over_1[i].OffsetHigh = over_2[i].OffsetHigh = shiftRead.HighPart; // SEONDN PART (^$ bit
	        shiftRead.QuadPart += blockSize;
	        shiftWrite.QuadPart += blockSize;
	    }

	    do
	    {
	    	LocalReadWrite(curSize, blockSize, localOverlappedIOSize, over_1, buffer, localOldFileHandle, 'r');
	        LocalReadWrite(curSize, blockSize, localOverlappedIOSize, over_2, buffer, localNewFileHandle, 'w');
	        curSize -= (long long)(blockSize*localOverlappedIOSize);
	    }
	    while (curSize > 0);

	    SetFilePointerEx(localNewFileHandle, fileSizeStruct, NULL, FILE_BEGIN);
	    SetEndOfFile(localNewFileHandle);

        //copyFile(localOldFileHandle, localNewFileHandle, localBlockSize, localOverlappedIOSize);
        //copyFile(localOldFileHandle, localNewFileHandle, 4096*5, localOverlappedIOSize);
    }

    // Checking handle and closing the file
    if (localOldFileHandle != NULL && localOldFileHandle != INVALID_HANDLE_VALUE) // old file checking
    {
        if (CloseHandle(localOldFileHandle) == false)
        {
            cout << "EROOR WHILE CLOSING FILE \"" << localOldFileHandle << "\". " << endl;
        }
    }
    if (localNewFileHandle != NULL && localNewFileHandle != INVALID_HANDLE_VALUE) // new file checking
    {
        if (CloseHandle(localNewFileHandle) == false)
        {
            cout << "ERROR WHILE CLOSING FILE \"" << localNewFileHandle << "\". " << endl;
        }
    }

    return localActionTime;
}

/*void copyFile(HANDLE firstHandle, HANDLE secondHandle, DWORD blockSize, int operationsCount)
{
    DWORD high = 0;
    LARGE_INTEGER fileSizeStruct; // where file size will be wrote
    long long fileSize; // number where we write
    GetFileSizeEx(firstHandle, &fileSizeStruct); // file size (for input as LARGE_INTEGER we use it!!!)
    fileSize = fileSizeStruct.QuadPart; // uniting fields one number
    long long curSize = fileSize;

    CHAR** buffer = new CHAR*[operationsCount]; // buffer of the data
    for (int i=0; i < operationsCount; i++)
    {
        buffer[i] = new CHAR[(int)blockSize];
    }
    // äîëæíû îñòàâàòüñÿ äîïóñòèìûìè äëÿ äëèòåëüíîé îïåðàöèè ÷òåíèÿ.
    OVERLAPPED* over_1 = new OVERLAPPED[operationsCount]; // var for handling pointers (starting bytes)
    OVERLAPPED* over_2 = new OVERLAPPED[operationsCount]; // new file

    shiftRead.QuadPart = 0; // how many rode
    shiftWrite.QuadPart = 0; // how many write

    for (int i=0; i < operationsCount; i++)
    {
        over_1[i].Offset = over_2[i].Offset = shiftRead.LowPart; // FIRST PART OF THE STRUCTURE
        over_1[i].OffsetHigh = over_2[i].OffsetHigh = shiftRead.HighPart; // SEONDN PART (^$ bit
        shiftRead.QuadPart += blockSize;
        shiftWrite.QuadPart += blockSize;
    }

    do
    {
    	LocalReadWrite(curSize, blockSize, operationsCount, over_1, buffer, firstHandle, 'r');
        LocalReadWrite(curSize, blockSize, operationsCount, over_2, buffer, secondHandle, 'w');
        curSize -= (long long)(blockSize*operationsCount);
    }
    while (curSize > 0);

    SetFilePointerEx(secondHandle, fileSizeStruct, NULL, FILE_BEGIN);
    SetEndOfFile(secondHandle);
}*/

// ---------- GET DRIVE SECTOR SIZE ----------

DWORD LocalDriveSectorSize()
{
    DWORD localBytes = -1;
    DWORD localValue = -1;
    return localBytes = LocalDriveSectorSize(localValue);
}

// ---------- GET DRIVE SECTOR SIZE (MAIN OVERLOAD FUNCTION) ----------

DWORD LocalDriveSectorSize (DWORD &localSectorsPerCluster)
{
    DWORD localNumberOfClusters = -1;
    DWORD localBytesPerSector = -1;
    DWORD localNumberOfFreeClusters = -1;
    if (GetDiskFreeSpaceA(NULL, &localSectorsPerCluster, &localBytesPerSector, &localNumberOfFreeClusters, &localNumberOfClusters) == false)
    {
        cout << "\nERROR GETTING DRIVE SECTORS\n";
    }
    return localBytesPerSector;
}

// ---------- AFTER READING FILE NEED TO MAKE THIS FUNCTION ----------

void CALLBACK CompletionRoutine (DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	callback = callback + 1;
}