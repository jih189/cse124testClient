/* programmer: jiaming hu
 * filename: errorMessage.c
 * description: this file defines all error message.
 */

#include <stdio.h>
#include <stdlib.h>

void printErrorMessage(const char*msg, const char*detail) {

    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(detail, stderr);
    fputc('\n', stderr);
    exit(1);
}


