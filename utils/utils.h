#ifndef UTILS_INCL
#define UTILS_INCL

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief file includes basic utility functions for implementing socket applications.
 * 
 */

/**
 * @brief function for handling errors caused by user, e.g. providing invalid IP address. uses fputs.
 * 
 * @param msg states what the error is.
 * @param info states possible reason for why the error occured.
 */
void KillErrorUser(const char* msg, const char* info);

/**
 * @brief function for handling system errors, e.g. failed to bind socket. uses errno and perror.
 * 
 * @param msg states what the error is.
 */
void KillErrorSyst(const char* msg);

#endif /* UTILS_INCL */