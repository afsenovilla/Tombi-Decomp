// FUNC 8005c098 116 MAIN0
// MATCHING 8005c098 116
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libc2/strstr.c, strstr); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"

char* strstr(const char* str, const char* substr)
{
    const char* s = str;
    bool in_match = false;
    const char* subs = substr;
    const char* candidate = str;

    while (*s) {
        if (*s == *subs) {
            subs++;
            if (*subs == '\0') {
                // Found
                return (char*)candidate; // cast away const
            }
            if (!in_match) {
                candidate = s;
                in_match = true;
            }
        } else {
            subs = substr;
            in_match = false;
        }
        s++;
    }

    // Not found
    return NULL;
}
