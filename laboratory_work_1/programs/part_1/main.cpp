/*

Saint Petersburg Electrotechnical University "LETI" (ETU "LETI"),
Faculty of Computer Science and Technology "FKTI",
Department of Computer Science and Engineering,
Computer Systems Engineering and Informatics (09.03.01) program.

OS labortory work 1 version 0_6 dated 2021_09_22

This software is under MIT License (X11 License).
You can see a detailed description in "LICENSE.md" file.

Copyight (c) 2021 Sobolev Matvey Sergeevich

*/

/*

Win32 API (WinAPI) is a set of functions in the library <windows.h>
API means "Application Programming Interface"

*/

/*

No Hope for Sudden joy,
Gatekeepers stand, the grass is swinging.
How many lifes you will decoy?
The Shine for You, for Me is Madness!

What's Power of Your Force,
My Dear Friend, My Dear Darkness?
No Matter how I Strive, this Path,
It Seems so Long and Endless...

*/

#include <windows.h> // for WinAPI functions
#include <math.h> // for double making
#include <exception> // for exceptions
#include <iostream> // just for working
#include <string> // for the "string" type using
#include <vector> // for the "vector" type using
#include <algorithm> // for the "find" function using

using namespace std;

string currentPath = "c:\\"; // the current working path is disc "c:/" by default!

// ---------- FUNCTION DECLARATION ----------

string GetDiskName();
void MainMenu ();
void Info ();
void Poem ();
void LocalGetLogicalDrives ();
void LocalGetDriveType ();
void LocalGetVolumeInformation ();
void LocalGetDiskFreeSpaceEx ();
void LocalGetDiskFreeSpace ();
string GetPathKernel (char localFlag);
string GetPathShell (char localFunctionFlag, char localFlagOne, char localFlagTwo, string localMessage);
void LocalCreateRemoveDirectory (char actionCreateRemove);
void LocalCreateFile();
void LocalCopyMoveFile(char actionCopyMove);
void LocalGetFileAttributes ();

// ---------- MAIN ----------

int main (int argc, char* argv[]) // i've finally understood what it means (argc -- number of arguments, argv -- strings of arguments (including -<word> and --<word>))
{
	// "GET CURRENT DIRECTORY", "SET CURRENT DIRECTORY"

	int flag = -1; // "-1" for incorrect input continue the program

	do
	{
		MainMenu();
		cin >> flag;
		cout << "\n";
		switch (flag)
		{
			case 0:
				cout << "Goodbye!";
				break;
			case 1:
				cout << "Your current working path is: \"" << currentPath << "\" (c:\\ by default).\n";
				break;
			case 2:
				currentPath = GetPathKernel('c');
				break;
			case 3:
				Info();
				break;
			case 4:
				Poem();
				break;
			case 11:
				LocalGetLogicalDrives();
				break;
			case 21:
				LocalGetDriveType();
				break;
			case 22:
				LocalGetVolumeInformation();
				break;
			case 23:
				LocalGetDiskFreeSpace();
				break;
			case 231:
				LocalGetDiskFreeSpaceEx();
				break;
			case 31:
				LocalCreateRemoveDirectory ('c');
				break;
			case 32:
				LocalCreateRemoveDirectory ('r');
				break;
			case 41:
				LocalCreateFile();
				break;
			case 51:
				LocalCopyMoveFile('c');
				break;
			case 52:
				LocalCopyMoveFile('m');
				break;
			case 53:
				LocalCopyMoveFile('e');
				break;
			case 61:
				LocalGetFileAttributes();
				break;
			default:
				cout << "Incorrect input! Try again.";
				break;
		}
	}
	while (flag != 0);

	return 0;
}

// ---------- 0 -- GET DISK NAME ----------

string GetDiskName ()
{
	string localDisk;

	cout << "Please, input disk name you want:\n";
	cin >> localDisk;
	localDisk = localDisk + ":\\";

	return localDisk;
}

// ---------- 0 -- MAIN MENU ----------

void MainMenu ()
{
	cout << "\n";
	cout << "Please, choose the menu item:\n";
	cout << "0 -- COMMON:\n";
	cout << "0 -- Quit\n";
	cout << "1 -- Output current directory\n";
	cout << "2 -- Change current working directory\n";
	cout << "3 -- Info\n";
	cout << "4 -- \"Can machine write a poem?\"\n";
	cout << "1 -- DRIVES LIST:\n";
	cout << "11 -- Show all avaliable disk drives\n";
	cout << "2 -- INFORMATION ABOUT DRIVES:\n";
	cout << "21 -- Show the drive type\n";
	cout << "22 -- Show the volume information\n";
	cout << "23 -- Show the disk free space\n";
	cout << "3 -- DIRECTORIES:\n";
	cout << "31 -- Create new directory\n";
	cout << "32 -- Remove old directory\n";
	cout << "4 -- CREATE/OPEN FILES:\n";
	cout << "41 -- Create new file\n";
	cout << "5 -- COPY/MOVE FILES\n";
	cout << "51 -- Copy file\n";
	cout << "52 -- Move file\n";
	cout << "53 -- Move file (extended properties)\n";
	cout << "6 -- ATTRIBUTES\n";
	cout << "61 -- Get file attributes\n";
	cout << "\n";
}

// ---------- 0 -- INFO ----------

void Info ()
{
	cout << "Saint Petersburg Electrotechnical University \"LETI\" (ETU \"LETI\"),\n"
	<< "Faculty of Computer Science and Technology \"FKTI\",\n"
	<< "Department of Computer Science and Engineering,\n"
	<< "Computer Systems Engineering and Informatics (09.03.01) program.\n\n"
	<< "OS labortory work 1 version 0_6 dated 2021_09_22\n\n"
	<< "This software is under MIT License (X11 License).\n"
	<< "You can see a detailed description in \"LICENSE.md\" file.\n\n"
	<< "Copyight (c) 2021 Sobolev Matvey Sergeevich\n";
}

// ---------- 0 -- POEM ----------

void Poem ()
{
	cout << "No Hope for Sudden joy,\n"
	<< "Gatekeepers stand, the grass is swinging.\n"
	<< "How many lifes you will decoy?\n"
	<< "The Shine for You, for Me is Madness!\n\n"
	<< "What's Power of Your Force,\n"
	<< "My Dear Friend, My Dear Darkness?\n"
	<< "No Matter how I Strive, this Path,\n"
	<< "It Seems so Long and Endless...\n";
}

// ---------- 1 -- GET LOGICAL DRIVES ----------

void LocalGetLogicalDrives()
{
	int localDriveNumber = 1;
	int localDiskDetection;
	char localDriveLetter[4];
	DWORD dr = GetLogicalDrives();

	for(int i = 0; i < 26; i++)
	{
		localDiskDetection = ((dr >> i) & 0x00000001);
		if (localDiskDetection == 1)
		{
			localDriveLetter[0] = char(65 + i);
			localDriveLetter[1] = ':';
			localDriveLetter[2] = '\\';
			localDriveLetter[3] = 0;
			cout << localDriveNumber << ". Available disk drive: " << localDriveLetter << endl;
			localDriveNumber = localDriveNumber + 1; // next number
		}
	}
}

// ---------- 2 -- GET DRIVE TYPE ----------

void LocalGetDriveType()
{
	// i need to check, if i use the "uint" namespace

	int d;
	string n;

	n = GetDiskName();

	// <string variable>.c_str() means that you convert to <const char * type>, because "" isn't <const char *> type

	d = GetDriveType(n.c_str()); // i just want to know what's going one if i choose d or f!
	cout << n << " is";
	//cout << n + " is";

	if (d == DRIVE_UNKNOWN)
	{
		cout << " UNKNOWN" << endl;
	}
	if (d == DRIVE_NO_ROOT_DIR)
	{
		cout << " DRIVE NO ROOT DIR" << endl;
	}
	if (d == DRIVE_REMOVABLE)
	{
		cout << " REMOVABLE" << endl;
	}
	if (d == DRIVE_FIXED)
	{
		cout << " FIXED" << endl;
	}
	if (d == DRIVE_REMOTE)
	{
		cout << " REMOTE" << endl;
	}
	if (d == DRIVE_CDROM)
	{
		cout << " CDROM" << endl;
	}
	if (d == DRIVE_RAMDISK)
	{
		cout << " RAMDISK" << endl;
	}
}

// ---------- 2 -- GET VOLUME INFORMATION ----------

void LocalGetVolumeInformation ()
{
	char VolumeNameBuffer[100];
	char FileSystemNameBuffer[100];
	string n;
	unsigned long VolumeSerialNumber;

	n = GetDiskName();

	BOOL GetVolumeInformationFlag = GetVolumeInformationA(
		n.c_str(),
		VolumeNameBuffer,
		100,
		&VolumeSerialNumber,
		NULL, //&MaximumComponentLength,
		NULL, //&FileSystemFlags,
		FileSystemNameBuffer,
		100
	);

	/*BOOL GetVolumeInformationFlag = GetVolumeInformationA(
		"d:\\",
		VolumeNameBuffer,
		100,
		&VolumeSerialNumber,
		NULL, //&MaximumComponentLength,
		NULL, //&FileSystemFlags,
		FileSystemNameBuffer,
		100
	);*/

	if (GetVolumeInformationFlag != 0) 
	{
		cout << " Volume Name is " << VolumeNameBuffer << endl;
		cout << " Volume Serial Number is " << VolumeSerialNumber << endl;
		cout << " File System is " << FileSystemNameBuffer << endl;
	}
	else
	{
		cout << " Not Present (GetVolumeInformation)" << endl;
	}
}

// ---------- 2 -- GET DISK FREE SPACE EX ----------

void LocalGetDiskFreeSpaceEx ()
{
	DWORD FreeBytesAvailable;
	DWORD TotalNumberOfBytes;
	DWORD TotalNumberOfFreeBytes;

	BOOL GetDiskFreeSpaceFlag = GetDiskFreeSpaceEx("c:\\", // directory name
	(PULARGE_INTEGER)&FreeBytesAvailable, // bytes available to caller
	(PULARGE_INTEGER)&TotalNumberOfBytes, // bytes on disk
	(PULARGE_INTEGER)&TotalNumberOfFreeBytes  // free bytes on disk
	);
	if(GetDiskFreeSpaceFlag != 0)
	{
		//double d = double(unsigned long(TotalNumberOfFreeBytes))/1024/1024/1024;
		//cout << d;
		cout << " Total Number Of Free Bytes = " << (unsigned long)TotalNumberOfFreeBytes << "( " << (double)(((unsigned long)(TotalNumberOfFreeBytes/1024))/1000) << " Mb )" << endl;
		cout << " Total Number Of Bytes = " << (unsigned long)TotalNumberOfBytes << "( " << (double)(((unsigned long)(TotalNumberOfBytes/1024))/1000) << " Mb )" << endl;
		cout << " Total Number Of Bytes = " << (unsigned long)TotalNumberOfBytes << "( " << TotalNumberOfBytes << " Mb )" << endl;
		unsigned long tmp_1 = TotalNumberOfBytes;
		tmp_1 = tmp_1/1024;
		cout << tmp_1;
		double tmp_2 = tmp_1;
		tmp_2 = tmp_2/1000;
		cout << tmp_2;
		cout << TotalNumberOfBytes/1024000;
	}
	else
	{
		cout << " Not Present (GetDiskFreeSpace)" << endl;
	}
}

// ---------- 2 -- GET DISK FREE SPACE ----------

void LocalGetDiskFreeSpace ()
{
	/*long unsigned int * secPerClus;
	long unsigned int * bytePerSec;
	long unsigned int * freeClus;
	long unsigned int * totalClus;*/
	/*LPDWORD secPerClus;
	LPDWORD bytePerSec;
	LPDWORD freeClus;
	LPDWORD totalClus;*/

	string localDiskName = GetDiskName();

	//const char diskNameCC[4] = {'e', ':', '\\'}; // you can do this
	//string diskNameS = "e:\\"; // and you can do this

	DWORD secPerClus;
	DWORD bytePerSec;
	DWORD freeClus;
	DWORD totalClus;
	//int gdfs = GetDiskFreeSpace(diskNameCC, &secPerClus, &bytePerSec, &freeClus, &totalClus); // const char* explicitly
	//int gdfs = GetDiskFreeSpace(diskNameS.c_str(), &secPerClus, &bytePerSec, &freeClus, &totalClus); // const char* from string (with c_str() method)
	int gdfs = GetDiskFreeSpace(localDiskName.c_str(), &secPerClus, &bytePerSec, &freeClus, &totalClus);

	if (gdfs != 0)
	{
		cout << "Disk free space information about disk " << localDiskName << "!\n";
		cout << "---------- NET NUMBERS ----------\n";
		cout << "Total Number Of Sectors per Cluster: " << (unsigned long long)secPerClus << "\nTotal Number Of Bytes per Sector: " << (unsigned long long)bytePerSec << "\nTotal Number Of Free Clusters: " << (unsigned long long)freeClus << "\nTotal Number Of Clusters: " << (unsigned long long)totalClus << "\nReturned value: " << (unsigned long long)gdfs << "\n";
		cout << "---------- CALCULATED NUMBERS ----------\n";
		cout << "Total Number Of Bytes per Cluster: " << (unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\nTotal Number Of Free Sectors: " << (unsigned long long)freeClus*(unsigned long long)secPerClus;
		cout << "\nTotal Number Of Sectors: " << (unsigned long long)totalClus*(unsigned long long)secPerClus << "\nTotal Number Of Free Bytes: " << (unsigned long long)freeClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\nTotal Number of Bytes: " << (unsigned long long)totalClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\n";
		cout << "Total Number Of Used Clusters: " << (unsigned long long)totalClus - (unsigned long long)freeClus;
		cout << "\nTotal Number Of Used Sectors: " << (unsigned long long)totalClus*(unsigned long long)secPerClus - (unsigned long long)freeClus*(unsigned long long)secPerClus << "\nTotal Number Of Used Bytes: " << (unsigned long long)totalClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec - (unsigned long long)freeClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\n";
	}
	else
	{
		cout << "Returned value: " << (unsigned long long)gdfs << "\nThere is no such disk as " << localDiskName << "!\n";
	}
}

// ---------- 3 -- CREATE DIRECTORY ----------

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		return false; // something is wrong with your path!
	}
	else if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true; // this is a directory!
	}
	else
	{
		return false; // this is not a directory!
	}
}

// ---------- 3 -- GET PATH KERNEL ----------

// 'c' -- current (working) directory path
// 'a' -- absolute directory path
// 'r' -- relative directory path
// 'f' -- absolute (full) file path
// 's' -- relative (short) file path
string GetPathKernel (char localFlag)
{
	char localCommit = 'n';
	string localPath;
	string localOldPath = currentPath;
	while (localCommit != 'y')
	{
		if (localFlag == 'a') // 'a' means "absolute path"
		{
			cout << "Please, input absolute directory path:\n";
			cin >> localPath;
		}
		else if (localFlag == 'r') // 'r' means "relative path"
		{
			cout << "Your current path is \"" << currentPath << "\". Please, input relative directory path:\n";
			cin >> localPath;
			localPath = currentPath + localPath;
		}
		else if (localFlag == 'f') // 'f' means "full file path"
		{
			cout << "Please, input absolute file path:\n";
			cin >> localPath;
		}
		else if (localFlag == 's') // 's' means "short file path"
		{
			cout << "Your current path is \"" << currentPath << "\". Please, input relative file path:\n";
			cin >> localPath;
			localPath = currentPath + localPath;
		}
		else if (localFlag == 'c') // 'c' means "current"
		{
			cout << "Please, input current working directory path:\n";
			cin >> localPath;
			if (localPath.length() > 0)
			{
				if (localPath.c_str()[localPath.length() - 1] != '\\')
				{
					localPath = localPath + "\\";
				}
			}
		}
		else // current working path by default
		{
			cout << "Please, input current working directory path:\n";
			cin >> localPath;
		}
		if (localFlag != 'a' && localFlag != 'r' && localFlag != 'f' && localFlag != 's' && dirExists(localPath))
		{
			if (localFlag == 'c') // 'c' means "current"
			{
				cout << "This is your new current working path: " << localPath << "\n";
			}
			else // current working path by default
			{
				cout << "This is your new current working path: " << localPath << "\n";
			}
			cout << "Commit changes? [y/n]\n";
			cin >> localCommit;
		}
		else if (localFlag == 'a' || localFlag == 'r') // for directories
		{
			if (localFlag == 'a') // 'a' means "absolute path"
			{
				cout << "This is your new absolute directory path: " << localPath << "\n";
			}
			else if (localFlag == 'r') // 'r' means "relative path"
			{
				cout << "This is your new relative directory path: " << localPath << "\n";
			}
			cout << "Commit changes? [y/n]\n";
			cin >> localCommit;
		}
		else if (localFlag == 'f' || localFlag == 's') // for files
		{
			if (localFlag == 'f') // 'f' means "full file path"
			{
				cout << "This is your new absolute file path: " << localPath << "\n";
			}
			else if (localFlag == 's') // 's' means "shorth file path"
			{
				cout << "This is your new relative file path: " << localPath << "\n";
			}
			cout << "Commit changes? [y/n]\n";
			cin >> localCommit;
		}
		else
		{
			cout << "Your new path \"" << localPath << "\"isn't valid (exist)! Try again (if no, old path will be returned)? [y/n]: " << "\n";
			cin >> localCommit;
			if (localCommit == 'y')
			{
				localCommit = 'n';
			}
			else if (localCommit == 'n')
			{
				localCommit = 'y';
				localPath = localOldPath;
			}
		}
	}
	if (localFlag != 'a' && localFlag != 'r' && localFlag != 'f' && localFlag != 's')
	{
		currentPath = localPath;
	}
	// because for setting current directory it doesn't matter, but for creating/removing it matters,
	// so to unificate the program, i make local path a current only if it's not for creating or deleting,
	// but i return the local path (for every flags it will be right, only consist different things)
	return localPath;
}

// ---------- 3 -- GET PATH SHELL ----------

string GetPathShell (char localFlagOne, char localFlagTwo, string localMessageOne, string localMessageTwo)
{
	//"Path to the FILE, which ATTRIBUTES you WANT TO GET.\n"
	char localFunctionFlag = '!'; // just random symbol
	string localFunctionPath;
	while (localFunctionFlag != localFlagOne && localFunctionFlag != localFlagTwo)
	{
		cout << localMessageOne << localMessageTwo;
		// checking for the flag
		cin >> localFunctionFlag;
		if (localFunctionFlag != localFlagOne && localFunctionFlag != localFlagTwo)
		{
			cout << "Try again!\n";
		}
	}
	if (localFunctionFlag == localFlagOne) // full file path situation
	{
		localFunctionPath = GetPathKernel(localFunctionFlag); // set new absolute path
	}
	else if (localFunctionFlag == localFlagTwo) // short file path situation
	{
		char localChange = 'n';
		cout << "Do you want to change current working path? [y/n]\n";
		cin >> localChange;
		if (localChange == 'y')
		{
			currentPath = GetPathKernel('c'); // changing current directory
		}
		localFunctionPath = GetPathKernel(localFunctionFlag); // set new relative path
	}
	return localFunctionPath;
}

// ---------- 3 -- LOCAL CREATE DIRECTORY ----------

void LocalCreateRemoveDirectory (char actionCreateRemove) // if 'c' -- creating directory, if 'r' -- removing directory, creating by default
{
	char localPathFlag = 'y'; // just another letter, not 'a' or 'r', so you need input it anyway
	string localDirectory; // directory path you input

	/*while (localPathFlag != 'a' && localPathFlag != 'r')
	{
		cout << "Do you want to input absolute path of the directory or relative? [a/r]\n";
		cin >> localPathFlag;
		if (localPathFlag != 'a' && localPathFlag != 'r')
		{
			cout << "Try again!\n";
		}
	}

	if (localPathFlag == 'a')
	{
		localDirectory = GetPathKernel(localPathFlag); // set new absolute path
	}
	else if (localPathFlag == 'r')
	{
		char localChange = 'n';
		cout << "Do you want to change current working path? [y/n]\n";
		cin >> localChange;
		if (localChange == 'y')
		{
			currentPath = GetPathKernel('c'); // changing current directory
		}
		localDirectory = GetPathKernel(localPathFlag); // set new relative path
	}*/

	localDirectory = GetPathShell('a', 'r', "", "Do you want to input absolute path of the directory or relative? [a/r]\n");

	if (actionCreateRemove == 'r') // 'r'
	{
		if (RemoveDirectory(localDirectory.c_str()))
		{
			cout << "The directory \"" << localDirectory << "\" has been successfully removed!\n";
		}
		else
		{
			cout << "Something wrong! The directory \"" << localDirectory << "\" hasn't been removed!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n";
		}
	}
	else // 'c' and default
	{
		if (CreateDirectory(localDirectory.c_str(), NULL))
		{
			cout << "The directory \"" << localDirectory << "\" has been successfully created!\n";
		}
		else
		{
			cout << "Something wrong! The directory \"" << localDirectory << "\" hasn't been created!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n";
		}
	}
}

// ---------- 4 -- LOCAL CREATE FILE ----------

void LocalCreateFile () // A WISE FACT: THERE IS NO "OPEN FILE" FINCTION, THERE IS "CREATE FILE" FUNCTION WITH SPECIAL FLAG TO OPEN FILE!
{
	int localChoose = 0; // to start a loop
	unsigned long localCreationDisposition; // DWORD = unsigned long, localChoose = number that has been written in specification
	char localPathFlag = 'y'; // just another letter, not 'f' or 's', so you need input it anyway
	string localFilePath; // file path you input

	/*while (localPathFlag != 'f' && localPathFlag != 's')
	{
		cout << "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n";
		cin >> localPathFlag;
		if (localPathFlag != 'f' && localPathFlag != 's')
		{
			cout << "Try again!\n";
		}
	}

	if (localPathFlag == 'f') // full file path situation
	{
		localFilePath = GetPathKernel(localPathFlag); // set new absolute path
	}
	else if (localPathFlag == 's') // short file path situation
	{
		char localChange = 'n';
		cout << "Do you want to change current working path? [y/n]\n";
		cin >> localChange;
		if (localChange == 'y')
		{
			currentPath = GetPathKernel('c'); // changing current directory
		}
		localFilePath = GetPathKernel(localPathFlag); // set new relative path
	}*/

	localFilePath = GetPathShell('f', 's', "Path to the FILE, which you WANT TO CREATE (OPEN).\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");

	while (localChoose < 1 || localChoose > 5)
	{
		// because "CREATE_NEW" by default (1 is number for "CREATE_NEW")
		cout << "Please, choose the creation disposition (CREATE_NEW by default):\n" << "1 -- CREATE_NEW\n" << "2 -- CREATE_ALWAYS\n" << "3 -- OPEN_EXISTING\n"
		<< "4 -- OPEN_ALWAYS\n" << "5 -- TRUNCATE_EXISTING\n"; // DWORD = unsigned long, localChoose = number that has been written in specification
		cin >> localChoose;
		if (localChoose < 1 || localChoose > 5)
		{
			cout << "Try again!\n";
		}
	}

	switch(localChoose)
	{
		case 1:
			localCreationDisposition = (unsigned long)localChoose; // DWORD = unsigned long, localChoose = number that has been written in specification
			break;
		case 2:
			localCreationDisposition = (unsigned long)localChoose;
			break;
		case 3:
			localCreationDisposition = (unsigned long)localChoose;
			break;
		case 4:
			localCreationDisposition = (unsigned long)localChoose;
			break;
		case 5:
			localCreationDisposition = (unsigned long)localChoose;
			break;
		default:
			localCreationDisposition = (unsigned long)localChoose;
			break;
	}

	// Open a handle to the file
	HANDLE localFile = CreateFile(
		localFilePath.c_str(), // Filename (<path to the file>)
		GENERIC_WRITE, // Desired access (0/GENERIC_READ/GENERIC_WRITE)
		FILE_SHARE_READ, // Share mode (FILE_SHARE_DELETE/FILE_SHARE_READ/FILE_SHARE_WRITE)
		NULL, // Security attributes (NULL/<structure SECURITY_ATTRIBUTES adress>)
		(DWORD)localCreationDisposition, // Creates a new file, only if it doesn't already exist (CREATE_ALWAYS/CREATE_NEW/OPEN_ALWAYS/OPEN_EXISTING/TRUNCATE_EXISTING)
		FILE_ATTRIBUTE_NORMAL, // Flags and attributes (FILE_ATTRIBUTE_NORMAL/FILE_ATTRIBUTE_ARCHIVE/FILE_ATTRIBUTE_ENCRYPTED/FILE_ATTRIBUTE_SYSTEM/
		// FILE_ATTRIBUTE_HIDDEN/FILE_ATTRIBUTE_NOT_CONTENT_INDEXED/FILE_ATTRIBUTE_OFFLINE/FILE_ATTRIBUTE_READONLY/FILE_ATTRIBUTE_SYSTEM/FILE_ATTRIBUTE_TEMPORARY/
		// FILE_FLAG_BACKUP_SEMANTICS/FILE_FLAG_DELETE_ON_CLOSE/FILE_FLAG_NO_BUFFERING/FILE_FLAG_OPEN_NO_RECALL/FILE_FLAG_OPEN_REPARSE_POINT/FILE_FLAG_OVERLAPPED/
		// FILE_FLAG_POSIX_SEMANTICS/FILE_FLAG_RANDOM_ACCESS/FILE_FLAG_SEQUENTIAL_SCAN/FILE_FLAG_WRITE_THROUGH/SECURITY_ANONYMOUS/SECURITY_CONTEXT_TRACKING/
		// SECURITY_DELEGATION/SECURITY_EFFECTIVE_ONLY/SECURITY_IDENTIFICATION/SECURITY_IMPERSONATION)
		NULL); // Template file handle (NULL/<template file descripter>)
	if (localFile != INVALID_HANDLE_VALUE)
	{
		cout << "The file \"" << localFilePath << "\" has been successfully created (opened, rewrited)!\n";
	}
	else
	{
		cout << "Something wrong! The file \"" << localFilePath << "\" hasn't been created (opened, rewrited)!\n";
		cout << "Last error code is \"" << GetLastError() << "\"\n";
	}
}

// ---------- 5 -- LOCAL COPY MOVE FILE ----------

void LocalCopyMoveFile (char actionCopyMove) // 'c' for copy, 'm' for moving, 'e' for extended moving (MoveFileEx) ATTENTION: 'c' -- copy -- is default if there is other letter!
{
	// specification of "CopyFile"
	/*BOOL CopyFile(
		LPCTSTR lpExistingFileName, // current file you want to copy
		LPCTSTR lpNewFileName, // new file, where you wnat to copy the old one
		BOOL bFailIfExists // TRUE means STOP IF NEW FILE EXIST, FALSE means OWERWRITE FILE ANYWAY
	);*/

	int localChoose = 0; // to start a loop
	bool localFailIfExists; // for existing file reaction
	char localPathFlag = 'y'; // just another letter, not 'f' or 's', so you need input it anyway
	char localSameNameFlag = 'a'; // random letter
	string localOldFilePath; // old (copied) file path you input
	string localNewFilePath; // new (pasted) file path you input

	// OLD FILE PATH INPUT (INCLUDING SITUATIONS "COPY" AND "MOVE")

	if (actionCopyMove == 'm' || actionCopyMove == 'e') // 'm' is for "moving", 'e' is for "extended moving"
	{
		localOldFilePath = GetPathShell('f', 's', "Path to the FILE, which you WANT TO MOVE (CUT).\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");
	}
	else // 'c' is for "copy", "copy" is default
	{
		localOldFilePath = GetPathShell('f', 's', "Path to the FILE, which you WANT TO COPY.\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");
	}

	/*while (localPathFlag != 'f' && localPathFlag != 's')
	{
		// "copy" and "move" situations
		if (actionCopyMove == 'm' || actionCopyMove == 'e') // 'm' is for "moving", 'e' is for "extended moving"
		{
			cout << "Path to the FILE, which you WANT TO MOVE (CUT).\n" << "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n";
		}
		else // 'c' is for "copy", "copy" is default
		{
			cout << "Path to the FILE, which you WANT TO COPY.\n" << "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n";
		}
		// checking for the flag
		cin >> localPathFlag;
		if (localPathFlag != 'f' && localPathFlag != 's')
		{
			cout << "Try again!\n";
		}
	}

	if (localPathFlag == 'f') // full file path situation
	{
		localOldFilePath = GetPathKernel(localPathFlag); // set new absolute path
	}
	else if (localPathFlag == 's') // short file path situation
	{
		char localChange = 'n';
		cout << "Do you want to change current working path? [y/n]\n";
		cin >> localChange;
		if (localChange == 'y')
		{
			currentPath = GetPathKernel('c'); // changing current directory
		}
		localOldFilePath = GetPathKernel(localPathFlag); // set new relative path
	}*/

	// SAME NAME SITUATION

	while (localSameNameFlag != 'y' && localSameNameFlag != 'n')
	{
		cout << "Do you want the SAME NAME AS COPIED/CUT file (including extension)? [y/n]\n";
		// checking for the flag
		cin >> localSameNameFlag;
		if (localSameNameFlag != 'y' && localSameNameFlag != 'n') // 'n' -- no -- by default
		{
			cout << "Try again!\n";
		}
	}	

	// NEW FILE PATH INPUT

	//localPathFlag = 'y'; // random letter again to prevent ignoring if construction

	if (localSameNameFlag == 'y') // same name of the file -- choosing full directory path instead of the filename
	{
		string localFilename = string(localOldFilePath);
		// Remove directory if present.
		// Do this before extension removal incase directory has a period character.
		const size_t last_slash_idx = localFilename.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
		{
		    localFilename.erase(0, last_slash_idx + 1);
		}
		// Remove extension if present.
		/*const size_t period_idx = localFilename.rfind('.');
		if (std::string::npos != period_idx)
		{
		    localFilename.erase(period_idx);
		}*/

		if (actionCopyMove == 'm' || actionCopyMove == 'e') // 'm' is for "moving", 'e' is for "extended moving"
		{
			localNewFilePath = GetPathShell('a', 'r', "Path to the NEW DIRECTORY, where you WANT to MOVE (PASTE) the CUT FILE.\n", "Do you want to input absolute path of the directory or relative? [a/r]\n") + localFilename;
		}
		else // 'c' is for "copy", "copy" is DEFAULT
		{
			localNewFilePath = GetPathShell('a', 'r', "Path to the NEW DIRECTORY, where you WANT to PASTE the COPIED FILE.\n", "Do you want to input absolute path of the directory or relative? [a/r]\n") + localFilename;
		}
	}
	else // 'n', different (as user choosed) name of the file -- choosing full file path (not the same name is DEFAULT)
	{
		if (actionCopyMove == 'm' || actionCopyMove == 'e') // 'm' is for "moving", 'e' is for "extended moving"
		{
			localNewFilePath = GetPathShell('f', 's', "Path to the NEW FILE, where you WANT to MOVE (PASTE) the CUT FILE.\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");
		}
		else // 'c' is for "copy", "copy" is DEFAULT
		{
			localNewFilePath = GetPathShell('f', 's', "Path to the NEW FILE, where you WANT to PASTE the COPIED FILE.\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");
		}
	}

	/*while (localPathFlag != 'f' && localPathFlag != 's')
	{
		// "copy" and "move" situations
		if (actionCopyMove == 'm' || actionCopyMove == 'e') // 'm' is for "moving", 'e' is for "extended moving"
		{
			cout << "Path to the NEW FILE, where you WANT to MOVE (PASTE) the CUT FILE.\n" << "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n";
		}
		else // 'c' is for "copy", "copy" is DEFAULT
		{
			cout << "Path to the NEW FILE, where you WANT to PASTE the COPIED FILE.\n" << "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n";
		}
		// checking for the flag
		cin >> localPathFlag;
		if (localPathFlag != 'f' && localPathFlag != 's')
		{
			cout << "Try again!\n";
		}
	}

	if (localPathFlag == 'f') // full file path situation
	{
		string localFilename = string(localOldFilePath);
		// Remove directory if present.
		// Do this before extension removal incase directory has a period character.
		const size_t last_slash_idx = localFilename.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
		{
		    localFilename.erase(0, last_slash_idx + 1);
		}

		if (localSameNameFlag == 'y') // same name of the file -- choosing full directory path instead of the filename
		{
			localNewFilePath = GetPathKernel('a') + localFilename; // set new absolute path, 'a' means "absolute"
		}
		else // 'n', different (as user choosed) name of the file -- choosing full file path (not the same name is DEFAULT)
		{
			localNewFilePath = GetPathKernel(localPathFlag); // set new absolute path, 'f' means "full file path"
		}
		//localNewFilePath = GetPathKernel(localPathFlag); // set new absolute path
	}
	else if (localPathFlag == 's') // short file path situation
	{
		string localFilename = string(localOldFilePath);
		// Remove directory if present.
		// Do this before extension removal incase directory has a period character.
		const size_t last_slash_idx = localFilename.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
		{
		    localFilename.erase(0, last_slash_idx + 1);
		}

		char localChange = 'n';
		cout << "Do you want to change current working path? [y/n]\n";
		cin >> localChange;
		if (localChange == 'y')
		{
			currentPath = GetPathKernel('c'); // changing current directory
		}

		if (localSameNameFlag == 'y') // same name of the file -- choosing relative directory path instead of the filename
		{
			localNewFilePath = GetPathKernel('r') + localFilename; // set new relative path, 'r' means "relative"
		}
		else // 'n', different (as user choosed) name of the file -- choosing short file path (not the same name is DEFAULT)
		{
			localNewFilePath = GetPathKernel(localPathFlag); // set new relative path, 's' means "short file path"
		}
		//localNewFilePath = GetPathKernel(localPathFlag); // set new relative path
	}*/

	// COPY and MOVE

	if (actionCopyMove == 'c' || (actionCopyMove != 'm' && actionCopyMove != 'e')) // copy
	{
		// IF NEW FILE EXISTS

		while (localChoose < 1 || localChoose > 2)
		{
			// because "CREATE_NEW" by default (1 is number for "CREATE_NEW")
			cout << "Please, choose the possible outcome for you IF NEW FILE EXISTS:\n" << "1 -- DON'T PASTE AND DON'T REWRITE ANY FILE (TRUE)\n"
			<< "2 -- REWRITE THE FILE (FALSE)\n";
			cin >> localChoose;
			if (localChoose < 1 || localChoose > 2)
			{
				cout << "Try again!\n";
			}
		}

		switch(localChoose)
		{
			case 1:
				localFailIfExists = true;
				break;
			case 2:
				localFailIfExists = false;
				break;
			default:
				localFailIfExists = true; // true -- file rewriting protection -- by default
				break;
		}

		// don't forget about c_str()!
		if (CopyFile(localOldFilePath.c_str(), localNewFilePath.c_str(), localFailIfExists)) // copy file and watching the result immediatly
		{
			cout << "The file \"" << localOldFilePath << "\" has been successfully copied to file \"" << localNewFilePath << "\"!\n";
		}
		else
		{
			cout << "Something wrong! The file \"" << localOldFilePath << "\" hasn't been copied to file \"" << localNewFilePath << "\"!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n";
		}
	}
	else if (actionCopyMove == 'm') // move
	{
		// don't forget about c_str()!
		if (MoveFile(localOldFilePath.c_str(), localNewFilePath.c_str())) // move file and watching the result immediatly
		{
			cout << "The file \"" << localOldFilePath << "\" has been successfully moved to file \"" << localNewFilePath << "\"!\n";
		}
		else
		{
			cout << "Something wrong! The file \"" << localOldFilePath << "\" hasn't been moved to file \"" << localNewFilePath << "\"!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n";
		}
	}
	else if (actionCopyMove == 'e') // extended move
	{
		string localChooseTwo = "2"; // because default
		// next parameters presented in decimal code, but can be translated at once (for example "111111" is all of the flags (but all is uncorrect)!)
		// MOVEFILE_COPY_ALLOWED = 2 (copy, than delete old, normal work), cannot be used with MOVEFILE_DELAY_UNTIL_REBOOT
		// MOVEFILE_CREATE_HARDLINK = 16 ("Reserved for future use", i don't know what does it means)
		// MOVEFILE_DELAY_UNTIL_REBOOT = 4 (waiting util reboot), cannot be used with MOVEFILE_COPY_ALLOWED
		// MOVEFILE_FAIL_IF_NOT_TRACKABLE = 32 ("function fails, if the file is the lik source")
		// MOVEFILE_REPLACE_EXISTING = 1 (replacing file)
		// MOVEFILE_WRITE_THROUGH = 8 (doesn't return anything until it's ACTUALLY MOVE SOMETHING!)
		unsigned long inFunctionNumber = 0;
		while (inFunctionNumber == 0)
		{
			// because "CREATE_NEW" by default (1 is number for "CREATE_NEW")
			cout << "Please, choose the possible flags for moving file (you CAN CHOOSE MANY -- JUST SPLIT NUMBERS BY SPACE):\n"
			<< "1 -- MOVEFILE_REPLACE_EXISTING (replacing existing, it it exists)\n"
			<< "2 -- MOVEFILE_COPY_ALLOWED (classic move) -- DEFAULT\n"
			<< "4 -- MOVEFILE_DELAY_UNTIL_REBOOT (moving after rebooting the system)\n"
			<< "8 -- MOVEFILE_WRITE_THROUGH (moving file, than returning value)\n"
			<< "16 -- MOVEFILE_CREATE_HARDLINK\n"
			<< "32 -- MOVEFILE_FAIL_IF_NOT_TRACKABLE\n";
			fflush(stdin);
			std::getline(std::cin, localChooseTwo);

			// spit the string
			std::string s = string(localChooseTwo);
			std::string delimiter = " ";

			int i = 0;
			size_t pos = 0;
			std::string token;
			std::vector<string> v;
			std::vector<int> vect{1, 2, 4, 8, 16, 32};
			while ((pos = s.find(delimiter)) != std::string::npos)
			{
				int tmpNumber = 0;
			    token = s.substr(0, pos);
			    v.push_back(token);
			    tmpNumber = std::stoi(token);
			    if (std::find(vect.begin(), vect.end(), tmpNumber) != vect.end())
			    {
			    	inFunctionNumber = inFunctionNumber + tmpNumber;
			    	vect.erase(std::remove(vect.begin(), vect.end(), tmpNumber), vect.end());
			    }
			    //std::cout << token << std::endl;
			    s.erase(0, pos + delimiter.length());
			}

			int newTMPNumber = std::stoi(s);
			if (std::find(vect.begin(), vect.end(), newTMPNumber) != vect.end())
			{
			    inFunctionNumber = inFunctionNumber + newTMPNumber;
			    vect.erase(std::remove(vect.begin(), vect.end(), newTMPNumber), vect.end());
			}

			//cout << inFunctionNumber;

			//std::cout << s << std::endl;
			// end split of the string

			if (inFunctionNumber == 0)
			{
				cout << "Try again!\n";
			}
		}
		// don't forget about c_str()!
		if (MoveFileEx(localOldFilePath.c_str(), localNewFilePath.c_str(), (DWORD)inFunctionNumber)) // extended move file and watching the result immediatly
		{
			cout << "The file \"" << localOldFilePath << "\" has been successfully moved to file \"" << localNewFilePath << "\"!\n";
		}
		else
		{
			cout << "Something wrong! The file \"" << localOldFilePath << "\" hasn't been moved to file \"" << localNewFilePath << "\"!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n"; // here i need to insert last error text string
		}
	}
}

// ---------- 6 -- LOCAL GET FILE ATTRIBUTES ----------

void LocalGetFileAttributes ()
{
	// specification of "GetFileAttributesA"
	/*DWORD GetFileAttributesA(
		LPCSTR lpFileName // file name, which i want to get the file attributes
	);*/

	DWORD localFileAttributes = 0;
	string localFilePath; // file path you input

	// FILE PATH INPUT

	localFilePath = GetPathShell('f', 's', "Path to the FILE or DIRECTORY, which ATTRIBUTES you WANT TO GET.\n", "Do you want to input absolute (full) path of the file (directory) or relative (short)? [f/s]\n");

	localFileAttributes = GetFileAttributes(localFilePath.c_str());

	cout << "File \"" << localFilePath << "\" attributes:\n";
	if (localFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
	{
		cout << "Archive (FILE_ATTRIBUTE_ARCHIVE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
	{
		cout << "Compressed (FILE_ATTRIBUTE_COMPRESSED)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_DEVICE)
	{
		cout << "Device (FILE_ATTRIBUTE_DEVICE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		cout << "Directory (FILE_ATTRIBUTE_DIRECTORY)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
	{
		cout << "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_HIDDEN)
	{
		cout << "Hidden (FILE_ATTRIBUTE_HIDDEN)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
	{
		cout << "Data stream configured with integrity (FILE_ATTRIBUTE_INTEGRITY_STREAM)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_NORMAL)
	{
		cout << "Normal (FILE_ATTRIBUTE_NORMAL)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
	{
		cout << "Not indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
	{
		cout << "Data stream not to be read by the data integrity scanner (FILE_ATTRIBUTE_NO_SCRUB_DATA)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_OFFLINE)
	{
		cout << "Don't avaliable immediatly (FILE_ATTRIBUTE_OFFLINE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_READONLY)
    {
		cout << "Read-only (FILE_ATTRIBUTE_READONLY)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
	{
		cout << "Data is not fully presented locally (FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN)
	{
		cout << "Data hasn't physical representation on system (FILE_ATTRIBUTE_RECALL_ON_OPEN)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
	{
		cout << "Reparse point/representation link (FILE_ATTRIBUTE_REPARSE_POINT)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
	{
		cout << "Sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_SYSTEM)
	{
		cout << "System used (FILE_ATTRIBUTE_SYSTEM)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
	{
		cout << "Temporary storage (FILE_ATTRIBUTE_TEMPORARY)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
	{
		cout << "Reserved for system (FILE_ATTRIBUTE_VIRTUAL)\n";
	}
}

// ---------- 6 -- LOCAL SET FILE ATTRIBUTES ----------

void LocalSetFileAttributes ()
{
	// specification of "SetFileAttributesA"
	/*BOOL SetFileAttributesA(
		LPCSTR lpFileName, // filename
		DWORD  dwFileAttributes //attributes
	);*/

	DWORD localFileAttributes = 0;
	string localFilePath; // file path you input

	// FILE PATH INPUT

	localFilePath = GetPathShell('f', 's', "Path to the FILE or DIRECTORY, which ATTRIBUTES you WANT TO SET.\n", "Do you want to input absolute (full) path of the file (directory) or relative (short)? [f/s]\n");

	localFileAttributes = GetFileAttributes(localFilePath.c_str());

	cout << "File \"" << localFilePath << "\" attributes:\n";
	if (localFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
	{
		cout << "Archive (FILE_ATTRIBUTE_ARCHIVE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
	{
		cout << "Compressed (FILE_ATTRIBUTE_COMPRESSED)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_DEVICE)
	{
		cout << "Device (FILE_ATTRIBUTE_DEVICE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		cout << "Directory (FILE_ATTRIBUTE_DIRECTORY)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
	{
		cout << "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_HIDDEN)
	{
		cout << "Hidden (FILE_ATTRIBUTE_HIDDEN)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
	{
		cout << "Data stream configured with integrity (FILE_ATTRIBUTE_INTEGRITY_STREAM)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_NORMAL)
	{
		cout << "Normal (FILE_ATTRIBUTE_NORMAL)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
	{
		cout << "Not indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
	{
		cout << "Data stream not to be read by the data integrity scanner (FILE_ATTRIBUTE_NO_SCRUB_DATA)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_OFFLINE)
	{
		cout << "Don't avaliable immediatly (FILE_ATTRIBUTE_OFFLINE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_READONLY)
    {
		cout << "Read-only (FILE_ATTRIBUTE_READONLY)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
	{
		cout << "Data is not fully presented locally (FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN)
	{
		cout << "Data hasn't physical representation on system (FILE_ATTRIBUTE_RECALL_ON_OPEN)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
	{
		cout << "Reparse point/representation link (FILE_ATTRIBUTE_REPARSE_POINT)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
	{
		cout << "Sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_SYSTEM)
	{
		cout << "System used (FILE_ATTRIBUTE_SYSTEM)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
	{
		cout << "Temporary storage (FILE_ATTRIBUTE_TEMPORARY)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
	{
		cout << "Reserved for system (FILE_ATTRIBUTE_VIRTUAL)\n";
	}
}