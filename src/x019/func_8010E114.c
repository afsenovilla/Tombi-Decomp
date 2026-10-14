// FUNC 8010e114 532 X019
// MATCHING 8010e114 532
#include "spad.h"
#include "gvars.h"
#include "player.h"
typedef struct P { short x; short y; } P;
typedef struct S {
    unsigned char b0; char p0; unsigned char type; char p1[0x16 - 3];
    unsigned short w16; char p2[0x40 - 0x18]; P *pos; P *pos2; char p3[0x68 - 0x48];
    unsigned char b68; char p4[0x6e - 0x69]; unsigned short w6e; char p5[2]; unsigned short w72;
} S;
extern unsigned short D_1F80019E;
extern short D_1F80019Es;
extern P *D_800A607C;
extern P *D_800A6078;
int func_8010E114(void)
{
    S **list = (*(S ***)&SPAD.p260);
    S *p;
    int r; short c;
    unsigned int f;
    unsigned short h, w, d;
    unsigned short n = SPAD.s250;
    D_1F80019E = n;
    r = 0;
    if ((SPAD.s1fc & SPAD.s3c4) == 0) return 0;
    if (n != 0)
    do {
        p = list[0];
        D_1F80019E--;
        list++;
        if (!(p->b0 & 2))
            continue;
        switch (p->type) {
        case 0x18: case 0x23: case 0x32: case 0x37: case 0x4d: case 0x55: case 0x58: case 0x59: case 0x5a:
            if (p->b68 != 0)
                break;
            if (((unsigned char*)&GVARS.b3a)[0] == 0 || ((unsigned char*)&GVARS.b3a)[8] != 0 || p->pos2->y != D_800A607C->y) {
                p->b68 = r;
                r = p->b68; /* extra refs raise r's global-alloc priority over d */
                break;
            }
            h = p->w72 + 10;
            c = (unsigned short)(h + (((unsigned short*)&PLAYER.obj.y.p.whole)[0] - p->w16)) < (short)h * 2;
            w = p->w6e + 10;
            if (c) {
                d = w + (D_800A6078->y - p->pos->y);
                if (d < (short)w * 2) {
                    f = ((unsigned short*)&PLAYER.obj.y.p.whole)[12] & 1;
                    if ((short)w < (short)d) {
                        if ((f ^ 1) != 0)
                            goto done;
                    } else {
                        if (f)
                            goto done;
                    }
                    if ((short)d < (short)w)
                        r = 5;
                    else
                        r = 4;
                }
            }
        done:
            (*(int*)&PLAYER.obj.w5c) = 1;
            p->b68 = r;
            if (r != 0)
                D_1F80019E = 0;
            break;
        }
    } while (D_1F80019Es != 0);
    return r;
}
