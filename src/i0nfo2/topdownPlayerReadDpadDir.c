// FUNC 800ea44c 236 I0NFO2
// MATCHING 800ea44c 236
#include "TOBJ.H"
extern volatile unsigned short PAD_PRESSED[];

void topdownPlayerReadDpadDir(TObj *o)
{
    if (*(unsigned char *)&o->wac < 2) {
        o->animFrame &= 7;
        if (PAD_PRESSED[0] & 0x10) {
            if (PAD_PRESSED[0] & 0x20) o->animFrame = 1;
            else if (PAD_PRESSED[0] & 0x80) o->animFrame = 3;
            else o->animFrame = 2;
        } else if (PAD_PRESSED[0] & 0x40) {
            if (PAD_PRESSED[0] & 0x20) o->animFrame = 7;
            else if (PAD_PRESSED[0] & 0x80) o->animFrame = 5;
            else o->animFrame = 6;
        } else if (PAD_PRESSED[0] & 0x80) {
            o->animFrame = 4;
        } else if (PAD_PRESSED[0] & 0x20) {
            o->animFrame = 0;
        } else {
            o->animFrame |= 8;
        }
    }
}
