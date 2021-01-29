#include "main.h"

// a sample exported function
int DLL_EXPORT ReadTextFromFile(char* filename, char** _buf, long* _fileSize)
{
    long fileSize = 0;
    char* buf = NULL;

    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fputs("Error opening file", stderr);
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buf = (char*) malloc(sizeof(char) * fileSize + 1);
    size_t result = fread(buf, 1, fileSize, fp);
    buf[fileSize] = '\0';
    if (result != fileSize)
    {
        fputs("Error reading", stderr);
        exit (3);
    }
    fclose(fp);

    *_buf = buf;
    *_fileSize = fileSize + 1;
    return 1;
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
