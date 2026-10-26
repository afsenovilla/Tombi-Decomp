// FUNC 800fc468 988 I0NFO2
// MATCHING 800fc468 988
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
extern TObj PLAYER;
extern TObj *SCRIPT_OBJECTS;
extern TObj *D_8009F2DC;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDAB[];
extern unsigned char D_800A60E4;
extern void topdownA17ActorSetDirAnim(TObj *o, int a, int b);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventStarted(int, int, int);

void func_800FC468(TObj *o)
{
    B12 v;
    TObj *a = SCRIPT_OBJECTS;
    TObj *b = D_8009F2DC;

    switch (o->state) {
    case 0:
        o->w08 = 0x3C;
        v = *(B12 *)&a->a;
        o->state++;
        PLAYER.step = 2;
        PLAYER.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        break;
    case 1:
        if (--o->w08 == 0) {
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 0, &v);
            topdownA17ActorSetDirAnim(a, 8, 0);
            o->state++;
        }
        break;
    case 2:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->d90 = (int)showMessageBoxDirect(2, 1, &v);
            topdownA17ActorSetDirAnim(b, 8, 0);
            topdownA17ActorSetDirAnim(a, 0, 0);
            o->state++;
        }
        break;
    case 3:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 2, &v);
            topdownA17ActorSetDirAnim(a, 8, 0);
            topdownA17ActorSetDirAnim(b, 0, 0);
            o->state++;
        }
        break;
    case 4:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            v = *(B12 *)&b->a;
            b->d90 = (int)showMessageBoxDirect(2, 3, &v);
            topdownA17ActorSetDirAnim(b, 8, 0);
            topdownA17ActorSetDirAnim(a, 0, 0);
            o->state++;
        }
        break;
    case 5:
        if (((TObj *)b->d90)->b04 == 2) {
            ((TObj *)b->d90)->b04 = 3;
            v = *(B12 *)&a->a;
            a->d90 = (int)showMessageBoxDirect(2, 4, &v);
            topdownA17ActorSetDirAnim(a, 8, 0);
            topdownA17ActorSetDirAnim(b, 0, 0);
            o->state++;
        }
        break;
    case 6:
        if (((TObj *)a->d90)->b04 == 2) {
            ((TObj *)a->d90)->b04 = 3;
            D_8009CDAB[0]++;
            o->state++;
            topdownA17ActorSetDirAnim(a, 0, 0);
            setEventStarted(0x4C, 0, 0);
        }
        break;
    case 7:
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
