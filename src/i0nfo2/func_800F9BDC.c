// FUNC 800f9bdc 1544 I0NFO2
// MATCHING 800f9bdc 1544
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
extern TObj PLAYER;
extern TObj *SCRIPT_OBJECTS;
extern TObj *D_8009F2DC;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CE4F;
extern unsigned char D_8009CF2B;
extern unsigned char D_8009CF2C[];
extern unsigned char D_800A60E4;
extern void topdownActorSetAnim(TObj *o, int a, int b);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventStarted(int, int, int);

void func_800F9BDC(TObj *o)
{
    B12 v;
    TObj *a = SCRIPT_OBJECTS;
    TObj *b = D_8009F2DC;

    switch (o->state) {
    case 0:
        a->step = 2;
        a->state = 0;
        b->step = 2;
        b->state = 0;
        PLAYER.step = 2;
        PLAYER.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        D_8009C93E = 1;
        o->state++;
        if (D_8009CF2C[0] == 0) {
            D_8009CF2C[0] = 1;
        } else {
            o->state = 11;
        }
        break;
    case 2:
        v = *(B12 *)&a->a;
        a->animFrame = 3;
        a->d90 = (int)showMessageBoxDirect(2, 0x13, &v);
        topdownActorSetAnim(a, 0xB, 0);
        topdownActorSetAnim(b, 3, 0);
        o->state++;
        break;
    case 3:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->animFrame = 3;
            b->d90 = (int)showMessageBoxDirect(2, 0x14, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state++;
        }
        break;
    case 4:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0x15, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 5:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->animFrame = 3;
            b->d90 = (int)showMessageBoxDirect(2, 0x16, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state++;
        }
        break;
    case 6:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0x17, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 7:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->animFrame = 3;
            b->d90 = (int)showMessageBoxDirect(2, 0x18, &v);
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 0xB, 0);
            o->state++;
        }
        break;
    case 8:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0x19, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 9:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            topdownActorSetAnim(a, 3, 0);
            o->state++;
            if (D_8009CE4F != 0xFF) {
                o->w08 = 0xC8;
                setEventStarted(0xAB, 0, 0);
            } else {
                o->state = 11;
            }
        }
        break;
    case 10:
        if (--o->w08 == 0) {
            o->state++;
        }
        break;
    case 11:
        v = *(B12 *)&b->a;
        b->animFrame = 3;
        b->d90 = (int)showMessageBoxDirect(2, 0x1A, &v);
        topdownActorSetAnim(a, 3, 0);
        topdownActorSetAnim(b, 0xB, 0);
        o->state++;
        break;
    case 12:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0x1B, &v);
            topdownActorSetAnim(a, 0xB, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state++;
        }
        break;
    case 13:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            topdownActorSetAnim(a, 3, 0);
            topdownActorSetAnim(b, 3, 0);
            o->state = 20;
        }
        break;
    case 20:
        a->b68 = 0;
        b->b68 = 0;
        a->step = 0;
        a->state = 0;
        b->step = 0;
        b->state = 0;
        D_8009CF2B = 0;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A60E4 = 0;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
