// FUNC 80134190 376 X000
// MATCHING 80134190 376
#include "TOBJ.H"
extern int FUN_8001f9e0(void);
extern void FUN_8001fe6c(TObj *o);
extern void FUN_8001fec0(TObj *o);
extern void *PTR_DAT_8013b270;
extern void *PTR_DAT_8013b274;

void FUN_80134190(TObj *o)
{
    switch (o->step) {
    case 0:
        switch (o->state) {
        case 0:
            if (FUN_8001f9e0() & 1) {
                o->anim = PTR_DAT_8013b270;
                FUN_8001fe6c(o);
                o->timer = 0x40;
                o->state = 2;
                break;
            }
            o->anim = PTR_DAT_8013b274;
            FUN_8001fe6c(o);
            o->timer = 0x20;
            o->state = 1;
            break;
        case 1:
            FUN_8001fec0(o);
            if (--o->timer == 0)
                o->state = 0;
            break;
        case 2:
            FUN_8001fec0(o);
            if (--o->timer == 0)
                o->state = 0;
            break;
        }
        break;
    case 1:
        switch (o->state) {
        case 0:
            switch (FUN_8001f9e0() & 3) {
            case 0:
            case 1:
                o->anim = PTR_DAT_8013b270;
                FUN_8001fe6c(o);
                o->timer = 0x40;
                o->state = 1;
                break;
            case 2:
                o->anim = PTR_DAT_8013b274;
                FUN_8001fe6c(o);
                o->timer = 0x20;
                o->state = 1;
                break;
            case 3:
                o->anim = PTR_DAT_8013b274;
                FUN_8001fe6c(o);
                o->timer = 0x20;
                o->state = 1;
                break;
            }
            break;
        case 1:
            FUN_8001fec0(o);
            if (--o->timer == 0)
                o->state = 0;
            break;
        }
        break;
    }
}
