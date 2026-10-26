// FUNC 800f969c 728 I0NFO2
// MATCHING 800f969c 728
#include "TOBJ.H"
typedef struct { short s[6]; } Pos6;
extern void func_80026C50(int a, int b, int c);
extern TObj *func_8002DCC8(int a, int b, Pos6 *p);
extern void func_8005A9A4(int a, int b);
extern void topdownActorSetDirAnim(TObj *o, int a, int b);
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern TObj *D_8009F308;
extern unsigned char D_800A603C;
extern unsigned char D_800A603D;
extern unsigned char D_800A603E;
extern TObj PLAYER;

void func_800F969C(TObj *o)
{
    Pos6 p;
    TObj *n = D_8009F308;
    TObj *m;
    switch (o->state) {
    case 0:
        o->state++;
    case 1:
        if (PLAYER.y.p.whole < 0xC6) {
            if (PLAYER.h->p.whole < n->h->p.whole) n->animFrame = 2;
            else n->animFrame = 0;
        } else {
            n->animFrame = 3;
        }
        topdownActorSetDirAnim(n, 8, 0);
        n->step = 2;
        n->state = 0;
        D_800A603D = 4;
        D_800A603E = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 2:
        p = *(Pos6 *)&n->a;
        n->d90 = (int)func_8002DCC8(2, 0x21, &p);
        o->state++;
    case 3:
        m = (TObj *)n->d90;
        if (m->b04 == 2) {
            m->b04 = 3;
            func_8005A9A4(0xA7, 1);
            func_80026C50(0x68, 1, 1);
            n->animFrame = 3;
            topdownActorSetDirAnim(n, 0, 0);
            o->w08 = 0xC8;
            o->state++;
        }
        break;
    case 4:
        if (--o->w08 <= 0) {
            o->w08 = 0x20;
            n->animFrame = 2;
            topdownActorSetDirAnim(n, 4, 0);
            o->state++;
        }
        break;
    case 5:
        n->h->p.whole--;
        if (--o->w08 <= 0) {
            n->animFrame = 3;
            topdownActorSetDirAnim(n, 4, 0);
            o->w08 = 0xE6;
            o->state++;
        }
        break;
    case 6:
        n->y.p.whole++;
        if (--o->w08 <= 0) {
            n->b04 = 3;
            o->state = 9;
        }
        break;
    case 9:
        n->b68 = 0;
        n->b6a = 0;
        n->step = 0;
        n->state = 0;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
