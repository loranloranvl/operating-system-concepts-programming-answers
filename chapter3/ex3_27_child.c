#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define BUFFER_SIZE 255

int main(int argc, char **argv) {
    if (argc <= 2) {
       fprintf(stderr, "Not enough arguments\n");
       return 1; 
    }
    char* FileDst = argv[1];
    DWORD FileSize = atoi(argv[2]);
    printf("%s\n", argv[2]);

    HANDLE ReadHandle, FileHandle;
    BYTE buffer[BUFFER_SIZE];
    DWORD NumberOfBytesFromPipe;
    DWORD NumberOfBytesWritten;

    ReadHandle = GetStdHandle(STD_INPUT_HANDLE);
    FileHandle = CreateFileA(
        FileDst,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    printf("%d\n", FileSize);
    for (int i = 0; i < FileSize / BUFFER_SIZE + 1; ++i) {
        ReadFile(
            ReadHandle,
            buffer,
            BUFFER_SIZE,
            &NumberOfBytesFromPipe,
            NULL
        );
        WriteFile(
            FileHandle,
            buffer,
            NumberOfBytesFromPipe,
            &NumberOfBytesWritten,
            NULL
        );
    }

    CloseHandle(FileHandle);
}