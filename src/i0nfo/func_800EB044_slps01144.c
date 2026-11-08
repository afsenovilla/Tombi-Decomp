// VERSION SLPS_01144 info 800EB044 952
// JP version of PAL i0nfo 800e9870 (overheadPlayerMainStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern void func_80020E38(TObj *o);
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void overheadPlayerWalkStep(TObj *o);
extern void overheadPlayerItemUseStep(TObj *o);
typedef struct { char p[0x2C]; unsigned short w2c; } P330;
extern P330 *D_8009DEE0;
extern unsigned char D_8009E4E2;
extern unsigned char D_8009E51D;
extern unsigned char D_8009EE58[];
extern volatile unsigned short D_8009F200[];

static __inline__ void setDir(TObj *o)
{
    D_8009EE58[0] = 1;
    o->animFrame &= 7;
    if (D_8009F200[0] & 0x1000) {
        if (D_8009F200[0] & 0x2000) o->animFrame = 1;
        else if (D_8009F200[0] & 0x8000) o->animFrame = 3;
        else o->animFrame = 2;
    } else if (D_8009F200[0] & 0x4000) {
        if (D_8009F200[0] & 0x2000) o->animFrame = 7;
        else if (D_8009F200[0] & 0x8000) o->animFrame = 5;
        else o->animFrame = 6;
    } else if (D_8009F200[0] & 0x8000) {
        o->animFrame = 4;
    } else if (D_8009F200[0] & 0x2000) {
        o->animFrame = 0;
    } else {
        o->animFrame |= 8;
    }
}

void func_800EB044(TObj *o)
{
    *((unsigned char *)o + 0xD2) = 0;
    switch (o->step) {
    case 0:
        setDir(o);
        if (D_8009E4E2 != 0) {
            switch (o->state) {
            case 0:
                o->state++;
            case 1:
                overheadPlayerSetMoveAnim(o);
                func_80020E38(o);
                if (!(o->animFrame & 8)) {
                    o->step = 1;
                    o->state = 0;
                }
                break;
            }
            if (*(unsigned char *)&o->wac >= 2) {
                *(unsigned char *)&o->wac = 0;
                o->step = 2;
                o->state = 0;
                return;
            }
            if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C8) {
                *(unsigned char *)&o->wac = 1;
                o->animFrame |= 8;
                return;
            }
            *(unsigned char *)&o->wac = 0;
        }
        break;
    case 1:
        setDir(o);
        overheadPlayerWalkStep(o);
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->state++;
            break;
        }
        break;
    case 0x30:
        overheadPlayerItemUseStep(o);
        break;
    case 0x63:
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            D_8009DEE0->w2c = o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->state++;
        }
        if (D_8009E51D == 0) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            D_8009E4E2 = 1;
        }
        break;
    }
}
#endif
