// FUNC 801236f0 812 X000
/* wip (score 22): covers func_801236F0 + func_801237E4 (jump-table split, 812 B). Only the velH/velV block of
   b04=1/step=0 differs: the game loads animFrame twice up front (first for D_80138F8C, second for D_80138F8E),
   stores b68/b9c, velH, then step++ and velV; here the volatile read and the order don't line up. */
#include "TOBJ.H"

typedef struct { short h, v; } VV;
extern void FUN_8003c980(TObj *);
extern void FUN_800188e0(TObj *);
extern void FUN_80020aec(int);
extern int ObjCullRegister(TObj *);
extern void ObjApplyVelocity(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern TObj *D_8009C948[];
extern short D_80138F8C[];
extern short D_80138F8E[];
extern unsigned char D_8009CE4D;
extern unsigned char D_8009C93F[], D_8009C93E[], D_8009C942[], D_800A4553[];
extern int D_800A4568[];
extern unsigned char D_8007A7F0[];
extern void (*D_8007A890[])(TObj *);

void func_801236F0(TObj *o)
{
    int f, g;

    switch (o->b04) {
    case 0:
        FUN_8003c980(o);
        o->active = 1;
        o->wb4 = 0;
        D_8009C948[0] = o;
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (ObjCullRegister(o) && (o->b68 & 1)) {
                f = o->animFrame;
                g = *(volatile unsigned short *)&o->animFrame;
                o->b68 = 0;
                o->velV = D_80138F8E[g * 2];
                o->velH = D_80138F8C[f * 2];
                o->b9c = 0;
                o->step++;
                if (D_8009CE4D == 0 && o->animFrame == 0) {
                    o->wb4 = 1;
                    D_8009C93F[0] = 1;
                    D_8009C93E[0] = 1;
                    D_8009C942[0] = 1;
                    D_800A4553[0] = 3;
                    D_800A4568[0] = 0;
                    *(int *)0x1F800190 = o->y.raw;
                    *(int *)0x1F80018C = o->h->raw;
                }
            }
            break;
        case 1:
            ObjApplyVelocity(o);
            o->velV += 0x20;
            if (o->velV > 0) o->b9c = 0;
            if ((o->b69 & 1) || TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 8))) {
                if (o->velV < 0x101) {
                    o->velV = 0;
                    o->active = 2;
                    o->step++;
                } else {
                    o->b9c = 1;
                    o->b69 = 0;
                    o->velV = -o->velV / 4;
                }
            }
        case 2:
            if (*(unsigned short *)&o->wb4) {
                *(int *)0x1F800190 = o->y.raw;
                *(int *)0x1F80018C = o->h->raw;
            }
            ObjCullRegister(o);
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            D_8009C948[0] = 0;
            D_8007A890[D_8007A7F0[o->subtype]](o);
            break;
        case 1:
            ObjCullRegister(o);
            FUN_80020aec(o->b6b);
            break;
        }
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
