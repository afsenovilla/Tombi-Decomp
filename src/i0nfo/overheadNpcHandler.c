// FUNC 800e8420 548 I0NFO
// MATCHING 800e8420 548
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_80020078(TObj *o, int a);
extern void func_800202B4(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void func_800187E4(TObj *o);
extern unsigned char D_8009CDB8;
extern unsigned char D_8009CDF4[];
extern int SPR_DATA[];
typedef struct { unsigned short w1e; unsigned short w08; short spad; short pad; void **anims; } Ent89C;
extern Ent89C D_800EE89C[];

void overheadNpcHandler(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->w1e = D_800EE89C[o->subtype].w1e;
        o->w08 = D_800EE89C[o->subtype].w08;
        o->d3c = SPR_DATA[D_800EE89C[o->subtype].spad];
        o->anim = D_800EE89C[o->subtype].anims[S16(o, 0x74)];
        func_8001FE94(o, S16(o, 0x76));
        o->d8c = 0;
        o->b0a = 0xD;
        o->b0d = 0;
        if (D_8009CDF4[0] == 0xFF) {
            if (D_8009CDB8 == 1) {
                if (o->b0c == 0x1C || o->b0c == 0x22) {
                    o->w08 = 0x7F88;
                }
            } else if (o->b0c == 0x23 || o->b0c == 0x29) {
                o->w08 = 0x7F88;
            }
        }
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (o->b0c == 0x23) {
                func_80020078(o, 0x6C);
            } else {
        case 1: case 2: case 3: case 4: case 5:
                func_800202B4(o);
            }
            if (o->visible != 0) {
                func_8001FEC0(o);
            }
            break;
        case 6:
            break;
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_800187E4(o);
        break;
    }
}
