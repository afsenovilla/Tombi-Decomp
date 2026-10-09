// FUNC 80119178 440 X002
// MATCHING 80119178 440
#include "TOBJ.H"

extern unsigned char D_8009CFEA;
extern void *D_8011FB60[];
extern void *D_8009F32C[];
extern void FUN_8001fe6c(TObj *);
extern void func_80116928(void *, int, int);

void func_80119178(TObj *o)
{
    void *e;

    switch (o->step) {
    case 0:
        if (D_8009CFEA == 1) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        switch (o->state) {
        case 0:
            o->anim = D_8011FB60[o->subtype];
            FUN_8001fe6c(o);
            o->state++;
            break;
        case 1:
            if (D_8009CFEA == 2) {
                o->step = 2;
                o->state = 0;
            }
            break;
        }
        break;
    case 2:
        e = D_8009F32C[o->subtype];
        switch (o->state) {
        case 0:
            if (--o->timer > 0) break;
            o->velY = 0;
            o->state++;
            break;
        case 1:
            o->y.raw += o->velY << 8;
            o->velY -= 0x10;
            if (o->velY == -0x80) func_80116928(e, 1, 0);
            if (o->velY < -0x600) o->velY = -0x600;
            if (o->visible == 0) o->b04 = 3;
            break;
        }
        break;
    }
}
