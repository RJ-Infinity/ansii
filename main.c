#define UNICODE

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>


int main(int argc, char** argv){
	if (argc < 2){
		printf(
			"usage %s:\n"
			"\tenables the ansii escape sequences for a program paseed in\n"
			"arguments\n"
			"\t[required] program name\n"
			"\t[optional][list] arguments for the program",
			argv[0]
		);
		return 1;
	}
	LPWSTR cmdLineTmp = GetCommandLine();
	size_t cmdLineLen = wcslen(cmdLineTmp);
	LPWSTR cmdLine = calloc(sizeof(cmdLineTmp[0]), cmdLineLen+1);
	memcpy(cmdLine, cmdLineTmp, sizeof(cmdLineTmp[0])*cmdLineLen);
	bool inStr = false;
	LPWSTR arguments = NULL;
	for (size_t i = 0; i < cmdLineLen; i++)
	{
		if (cmdLine[i] == '"'){inStr=!inStr;}
		if (!inStr && cmdLine[i] == ' ' && cmdLine[i+1] != ' '){
			arguments = cmdLine+i+1;
			break;
		}
	}

	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD initialMode;
	GetConsoleMode(stdOut, &initialMode);
	SetConsoleMode(stdOut, initialMode|ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	STARTUPINFO info={sizeof(info)};
	PROCESS_INFORMATION processInfo;
	if (!CreateProcessW(
		NULL,//lpApplicationName
		arguments, //lpCommandLine
		NULL, //lpProcessAttributes
		NULL, //lpThreadAttributes
		true, //bInheritHandles
		0, //dwCreationFlags
		NULL, //lpEnvironment
		NULL, //lpCurrentDirectory
		&info, //pStartupInfo
		&processInfo //lpProcessInformation
	)){
		printf("CreateProcess failed (%d).\n", GetLastError());
		return 2;
	}
	WaitForSingleObject(processInfo.hProcess, INFINITE);
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	SetConsoleMode(stdOut, initialMode);
	DWORD ec;
	GetExitCodeProcess(processInfo.hProcess, &ec);
	free(cmdLine);
	return ec;
}