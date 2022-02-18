#include "utils.h"

void KillErrorUser(const char* msg, const char* info)
{
    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(info, stderr);
    fputc('\n', stderr);
    exit(1);
}

void KillErrorSyst(const char* msg)
{
    perror(msg);
    exit(1);
}