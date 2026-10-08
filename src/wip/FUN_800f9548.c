// FUNC 800f9548 316 X000
#include "TOBJ.H"
extern short FUN_8001fe3c(int, int);
extern short FUN_8001fe0c(int, int);
extern char *DAT_8009c330;

void FUN_800f9548(TObj *o, int b, int c)
{
    int p;
    short s;
    if (o->animFrame & 1) {
        s = FUN_8001fe3c((0x1bf - c) & 0xff, DAT_8009c330[1]);
        o->h->p.whole = s + (short)o->d30;
        p = (int)DAT_8009c330;
        s = FUN_8001fe0c((0x1bf - c) & 0xff, *(unsigned char *)(p + 1));
        o->d84 = 0;
        o->d8c = 0x100 - (c & 0xff);
        s = s + (short)o->d34;
    } else {
        s = FUN_8001fe3c((c + 0xc0) & 0xff, DAT_8009c330[1]);
        o->h->p.whole = s + (short)o->d30;
        p = (int)DAT_8009c330;
        s = FUN_8001fe0c((c + 0xc0) & 0xff, *(unsigned char *)(p + 1));
        o->d84 = 0;
        o->d8c = c & 0xff;
        s = s + (short)o->d34;
    }
    o->y.p.whole = s;
    if (DAT_8009c330[8] == 0)
        s = *(short *)(DAT_8009c330 + 2);
    else
        s = -*(short *)(DAT_8009c330 + 2);
    *(short *)(DAT_8009c330 + 0xe) = *(short *)(DAT_8009c330 + 0xe) + s;
}
