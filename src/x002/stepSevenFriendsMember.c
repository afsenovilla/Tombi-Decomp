// FUNC 80119178 440 X002
// MATCHING 80119178 440
#include "gcd.h"
#include "TOBJ.H"

extern char D_8011FC08[];
extern char D_8011FC10[];
extern char D_8011FC18[];
extern char D_8011FC20[];
extern char D_8011FC28[];
extern char D_8011FC30[];
extern char D_8011FC38[];
static void *D_8011FB60[7] = {
    D_8011FC08, D_8011FC10, D_8011FC18, D_8011FC20,
    D_8011FC28, D_8011FC30, D_8011FC38,
};
extern void *D_8009F32C[];
extern void FUN_8001fe6c(TObj *);
extern void sevenFriendsMemberSetAnim(void *, int, int);

void stepSevenFriendsMember(TObj *o)
{
    void *e;

    switch (o->step) {
    case 0:
        if (GCD.bcfea == 1) {
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
            if (GCD.bcfea == 2) {
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
            if (o->velY == -0x80) sevenFriendsMemberSetAnim(e, 1, 0);
            if (o->velY < -0x600) o->velY = -0x600;
            if (o->visible == 0) o->b04 = 3;
            break;
        }
        break;
    }
}
