// VERSION SLPS_01144 info 800E9EA4 408
// JP version of PAL i0nfo 800e8420 (overheadNpcHandler.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
extern void func_80020E0C(TObj *o, int n);
extern void func_8002122C(TObj *o);
extern void func_80020E38(TObj *o);
extern void func_80017DB8(TObj *o);
extern int SPR_DATA[];
typedef struct { unsigned short w1e; unsigned short w08; short spad; short pad; void **anims; } Ent89C;
extern Ent89C D_800EFD5C[], D_800EFD5E[], D_800EFD60[], D_800EFD64[];

void func_800E9EA4(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->w1e = D_800EFD5C[o->subtype].w1e;
        o->w08 = D_800EFD5E[o->subtype].w1e;
        o->d3c = SPR_DATA[((short *)&D_800EFD60[o->subtype])[0]];
        o->anim = ((void***)&D_800EFD64[o->subtype])[0][S16(o, 0x74)];
        func_80020E0C(o, S16(o, 0x76));
        o->d8c = 0;
        o->b0a = 0xD;
        o->b0d = 0;
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (o->step != 0) {
            } else {
        case 1: case 2: case 3: case 4: case 5:
                func_8002122C(o);
            }
            if (o->visible != 0) {
                func_80020E38(o);
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
        func_80017DB8(o);
        break;
    }
}
#endif
