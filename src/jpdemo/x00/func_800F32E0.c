// JPDEMO x00 800F32E0 284
// Demo version of PAL x000 800F6C34 (playerSwingHandlePad.c): d-pad bits 0x1000-0x8000 (PAL 0x10-0x80). fixed pad bit 0x40; letting go keeps the timer and spin.
#include "TOBJ.H"
extern unsigned short DAT_8009d670[];
extern unsigned short DAT_1f8003c6;
extern unsigned char DAT_8009d2b0;
extern unsigned char *DAT_8009c330;
extern unsigned char *DAT_8009f0ec;

void playerSwingHandlePad(TObj *o)
{
    int a;
    short s;
    int s1;
    unsigned short u;
    Fix16 *h;
    if (DAT_8009d670[0] & 0x40) {
        DAT_8009c330[9] = 0;
        o->step = 0xb;
        o->state = 0;
        DAT_8009f0ec[0x69] = 0;
    }
    if (DAT_8009d670[0] & 0x1000) {
        DAT_8009c330[9] = 0;
        o->step = 0xb;
        o->state = 0;
        DAT_8009f0ec[0x69] = 0;
    }
    if (DAT_8009d670[0] & 0x4000) {
        DAT_8009c330[9] = 0;
        *(unsigned char *)&o->wac = 1;
        u = o->animFrame;
        h = o->h;
        o->b9e = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d30 = 0;
        o->d34 = 0;
        o->velX = 0;
        o->velY = 0;
        s1 = h->p.whole;
        if (u & 1)
            s = s1 + 0xe;
        else
            s = s1 - 0xe;
        h->p.whole = s;
        o->step = 2;
        o->state = 3;
        o->d8c = 0;
        DAT_8009f0ec[0x69] = 0;
    }
}
