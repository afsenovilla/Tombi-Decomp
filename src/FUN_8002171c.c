// FUNC 8002171c 316 MAIN0
// MATCHING 8002171c 316
#include "raw7.h"
extern unsigned short c960[], c982;
typedef struct { unsigned short v; } W;
extern volatile unsigned short c962;
extern unsigned char flag;
extern short **tabs[];
void FUN_8002171c(char *o)
{
    short *p = (short *)((int)tabs[c960[0]][c962] + c982 * 8);
    S32(o, 0xec) = *p++ << 16;
    S32(o, 0xf0) = *p << 16;
    S32(o, 0xf4) = p[1] << 16;
    switch (c960[0]) {
    case 0:
        if (flag == 0 && *(unsigned short *)&c962 == 0) S16(o, 0xee) = 0x40;
        else if (*(unsigned short *)&c962 == 3) S16(o, 0xee) = 0xd2;
        break;
    case 2:
        if (*(unsigned short *)&c962 == 4) S16(o, 0xee) = 0x90;
        break;
    case 4:
        if (*(unsigned short *)&c962 == 0xf) S16(o, 0xee) = 0x90;
        break;
    case 10:
        if (*(unsigned short *)&c962 == 8) S16(o, 0xee) = 0x90;
        break;
    }
}
