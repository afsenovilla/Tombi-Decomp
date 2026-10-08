// FUNC 800eb29c 248 X005
// MATCHING 800eb29c 248
#include "raw7.h"
extern int adv(void *);
extern void h(int, int);
void FUN_800eb29c(char *o)
{
    switch (U8(o, 3)) {
    case 0:
        switch (U8(o, 6)) {
        case 0:
            U8(o, 6)++;
            break;
        case 1:
            adv(o);
            break;
        }
        break;
    case 1:
        switch (U8(o, 6)) {
        case 0:
            h(0xe, 4);
            U8(o, 6)++;
            break;
        case 1:
            if (adv(o)) U8(o, 4) = 2;
            break;
        }
        break;
    case 2:
        switch (U8(o, 6)) {
        case 0:
            h(0xe, 0);
            U8(o, 6)++;
            break;
        case 1:
            if (adv(o)) U8(o, 4) = 2;
            break;
        }
        break;
    }
}
