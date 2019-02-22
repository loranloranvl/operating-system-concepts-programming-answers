#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define BUFFER_SIZE 255

int main(int argc, char **argv) {
    if (argc <= 2) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }
    char* FileSrc = argv[1];
    char* FileDst = argv[2];

    HANDLE ReadHandle, WriteHandle, FileHandle;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    /* set up security attributes allowing pipes to be inherited */
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
    /* allocate memory */
    ZeroMemory(&pi, sizeof(pi));
    /* create the pipe */
    if (!CreatePipe(&ReadHandle, &WriteHandle, &sa, 0)) {
        fprintf(stderr, "Create Pipe Failed");
        return 1;
    }
    /* establish the START INFO structure for the child process */
    GetStartupInfo(&si);

    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    /* redirect standard input to the read end of the pipe */
    si.hStdInput = ReadHandle;
    si.dwFlags = STARTF_USESTDHANDLES;
    /* don't allow the child to inherit the write end of pipe */
    SetHandleInformation(WriteHandle, HANDLE_FLAG_INHERIT, 0);
    
    FileHandle = CreateFileA(
        FileSrc,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    ); 
    
    /* create the child process */
    char cmd[255];
    DWORD FileSize = GetFileSize(FileHandle, NULL);
    sprintf(cmd, "ex3_27_child.exe %s %d", FileDst, FileSize);
    if (!CreateProcess(NULL, cmd, NULL, NULL,
            TRUE, /* inherit handles */
            0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "Create subprocess failed");
        return 1;
    }
    /* close the unused end of the pipe */
    CloseHandle(ReadHandle);
    BYTE buffer[BUFFER_SIZE];
    DWORD NumberOfBytesRead;
    DWORD NumberOfBytesIntoPipe;

    for (int i = 0; i < FileSize / BUFFER_SIZE + 1; ++i) {
        ReadFile(
            FileHandle,
            buffer,
            BUFFER_SIZE,
            &NumberOfBytesRead,
            NULL
        );
        WriteFile(
            WriteHandle,
            buffer,
            NumberOfBytesRead,
            &NumberOfBytesIntoPipe,
            NULL
        );
    }

    CloseHandle(FileHandle);
    CloseHandle(WriteHandle);

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}