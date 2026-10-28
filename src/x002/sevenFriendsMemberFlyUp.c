// FUNC 80119088 240 X002
// MATCHING 80119088 240
#include "TOBJ.H"
extern void *D_8009F32C[];
extern void sevenFriendsMemberSetAnim(void *, int, int);

void sevenFriendsMemberFlyUp(TObj *o)
{
    void *e = D_8009F32C[o->subtype];

    switch (o->state) {
    case 0:
        if (--o->timer <= 0) {
            o->velY = 0;
            o->state++;
        }
        break;
    case 1:
        o->y.raw += o->velY << 8;
        o->velY -= 0x10;
        if (o->velY == -0x80) {
            sevenFriendsMemberSetAnim(e, 1, 0);
        }
        if (o->velY < -0x600) {
            o->velY = -0x600;
        }
        if (o->visible == 0) {
            o->b04 = 3;
        }
        break;
    }
}
