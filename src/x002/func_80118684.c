// FUNC 80118684 2412 X002
// MATCHING 80118684 2412
#include "TOBJ.H"
typedef struct { short p0, x, p4, y, p8, z; } Pos;
extern unsigned char D_8009CE41, D_8009CE34, D_8009C971;
extern unsigned char D_8009C970_[];
#define D_8009C970 D_8009C970_[0]
extern unsigned char D_8009C93F_[];
#define D_8009C93F D_8009C93F_[0]
extern unsigned char D_8009C93B_[];
#define D_8009C93B D_8009C93B_[0]
extern unsigned char D_8009C93A_[];
#define D_8009C93A D_8009C93A_[0]
extern unsigned char D_8009C975_[];
#define D_8009C975 D_8009C975_[0]
extern signed char D_8009D2B0_[];
#define D_8009D2B0 D_8009D2B0_[0]
extern void *D_8011F868[], *D_8011F86C[], *D_8011F870[], *D_8011F874[];
extern void *D_1F8002D4_[];
#define D_1F8002D4 D_1F8002D4_[0]
extern char D_80077CDC[];
extern short *D_8011C8E8[];
extern Fix16 *D_800A6078;
extern unsigned short D_1F8003C4, D_1F8001FC;
extern TObj D_800A6038;
extern unsigned char D_800A603E_[];
#define D_800A603E D_800A603E_[0]
extern unsigned char D_800A603D_[];
#define D_800A603D D_800A603D_[0]
extern unsigned char D_800A603C_[];
#define D_800A603C D_800A603C_[0]
extern unsigned char D_1F8001CD_[];
#define D_1F8001CD D_1F8001CD_[0]
extern unsigned char D_1F8001CC_[];
#define D_1F8001CC D_1F8001CC_[0]
extern short D_1F8001C6_[];
#define D_1F8001C6 D_1F8001C6_[0]
extern short D_800A60EA_[];
#define D_800A60EA D_800A60EA_[0]
extern short D_800A604A_[];
#define D_800A604A D_800A604A_[0]
extern short D_800A60D2_[];
#define D_800A60D2 D_800A60D2_[0]
extern short D_800A60D0_[];
#define D_800A60D0 D_800A60D0_[0]
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern int FUN_8002dc50(int, int, int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_8001f110(int);
extern void FUN_800171b8(int, void *);
extern void FUN_8001d6a4();
extern void FUN_8001eb64(void);
extern void FUN_8003e918(TObj *, Pos *, int);
extern void FUN_8003fd78(TObj *, int, int);
extern void FUN_800eea7c(TObj *, short, short);
extern void FUN_80026c50(int, int, int);
extern void FUN_8001e4f0(int);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_80018790(TObj *);

void func_80118684(TObj *o)
{
    short *e;
    TObj *q;
    Pos s;

    switch (o->b04) {
    case 0:
        if (D_8009CE41 != 0) {
            o->b04 = 3;
            break;
        }
        if (D_8009CE34 == 0) o->wb4 = 0;
        else if (D_8009C970 < D_8009C971) o->wb4 = 2;
        else o->wb4 = 1;
        o->active = 2;
        o->wb6 = 0;
        o->w1e = 7;
        o->b0d = 0;
        {
            int d = (int)D_1F8002D4;
            void *a = D_8011F868[0];
            o->d3c = d;
            o->anim = a;
        }
        FUN_8001fe6c(o);
        o->b0a = 0;
        o->b0f = 0;
        o->animFrame = 0;
        o->movetab = D_80077CDC;
        o->timer = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        e = D_8011C8E8[(unsigned short)o->wb4];
        e += (unsigned short)o->wb6 * 2;
        q = &D_800A6038;
        switch (e[0]) {
        case 0:
            break;
        case 1:
            if (o->timer == 0) {
                o->timer = e[1];
            } else if (--o->timer == 0) {
                o->wb6++;
            }
            break;
        case 2:
            switch (o->step) {
            case 0:
                o->anim = D_8011F86C[0];
                FUN_8001fe6c(o);
                o->animFrame = 1;
                D_8009C93F = 1;
                q->b04 = 5;
                q->step = 100;
                q->velX = 0x100;
                q->state = 0;
                q->animFrame = 0;
                q->velY = 0x400;
                FUN_800eea7c(q, 1, 0);
                o->step++;
            case 1:
                if (o->h->p.whole < 200) o->animFrame = 0;
                if (o->h->p.whole >= 241) o->animFrame = 1;
                FUN_8001fa88(o, o->animFrame);
                q->a.raw += q->velX << 8;
                q->y.raw += q->velX << 8;
                FUN_8003fd78(q, 0, 0);
                if (o->h->p.whole - D_800A6078->p.whole < 40) {
                    D_8009C93A = 1;
                    o->step = 0;
                    o->wb6++;
                }
                break;
            }
            break;
        case 3:
            switch (o->step) {
            case 0:
                o->anim = D_8011F86C[0];
                FUN_8001fe6c(o);
                o->animFrame = 1;
                o->step++;
            case 1:
                if (o->h->p.whole < 200) o->animFrame = 0;
                if (o->h->p.whole >= 241) o->animFrame = 1;
                FUN_8001fa88(o, o->animFrame);
                if (D_8009D2B0 == 3) break;
                if (o->h->p.whole - q->h->p.whole >= 40) break;
                if (!(D_1F8001FC & D_1F8003C4)) break;
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 4:
            o->anim = D_8011F870[0];
            if (o->a.p.whole > D_800A604A) o->animFrame = 1;
            else o->animFrame = 0;
            FUN_8001fe6c(o);
            o->d90 = FUN_8002dc50(7, e[1], 0xdc, 0x9c);
            o->wb6++;
            break;
        case 5:
            o->anim = D_8011F868[0];
            if (o->a.p.whole > D_800A604A) o->animFrame = 1;
            else o->animFrame = 0;
            FUN_8001fe6c(o);
            o->d90 = FUN_8002dc50(7, e[1], 0xdc, 0x9c);
            o->wb6++;
            break;
        case 6:
            if (((unsigned char *)o->d90)[4] < 2) break;
            o->anim = D_8011F868[0];
            FUN_8001fe6c(o);
            ((unsigned char *)o->d90)[4]++;
            o->wb6++;
            break;
        case 7:
            FUN_8005a8a8(e[1], 0, 0);
            o->wb6++;
            break;
        case 8:
            FUN_8005a9a4(e[1], 0);
            o->wb6++;
            break;
        case 9:
            D_8009C975 = 3;
            o->wb6++;
            break;
        case 10:
            if (D_8009C975 == 1) o->wb6++;
            break;
        case 11:
            D_8009C975 = 4;
            o->wb6++;
            break;
        case 12:
            if (D_8009C975 == 0) o->wb6++;
            break;
        case 13:
            D_1F8001CC = 1;
            D_1F8001CD = 3;
            D_1F8001C6 = 2;
            FUN_8001f110(0);
            FUN_800171b8(1, FUN_8001d6a4);
            o->wb6++;
            break;
        case 14:
            FUN_8001eb64();
            o->wb6++;
            break;
        case 15:
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = 1;
            o->wb6++;
            break;
        case 16:
            D_800A60EA = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = 0;
            o->wb6++;
            break;
        case 17:
            if (D_800A6078->p.whole > 104) D_800A6078->p.whole = 104;
            break;
        case 18:
            switch (o->step) {
            case 0:
                o->animFrame = 0;
                o->anim = D_8011F86C[0];
                FUN_8001fe6c(o);
                o->movetab = D_80077CDC;
                o->step++;
            case 1:
                FUN_8001fa88(o, 0);
                if (o->a.p.whole < 261) break;
                o->anim = D_8011F868[0];
                D_8009C93B = 1;
                FUN_8001fe6c(o);
                o->w22 = 60;
                o->wb8 = 0;
                o->step++;
                break;
            case 2:
                if (--o->w22 == 0) {
                    s.x = o->h->p.whole;
                    s.y = o->y.p.whole;
                    s.z = o->d->p.whole;
                    FUN_8003e918(o, &s, 1);
                    o->step = 0;
                    o->animFrame = 1;
                    o->wb6++;
                }
                break;
            }
            break;
        case 19:
            switch (o->step) {
            case 0:
                o->animFrame = 1;
                o->anim = D_8011F86C[0];
                FUN_8001fe6c(o);
                o->movetab = D_80077CDC;
                o->wb8 = 1;
                o->step++;
            case 1:
                FUN_8001fa88(o, 1);
                if (o->a.p.whole >= 176) break;
                o->anim = D_8011F874[0];
                FUN_8001fe6c(o);
                o->w22 = 60;
                o->wb8 = 2;
                o->step++;
                break;
            case 2:
                if (--o->w22 == 0) {
                    o->step = 0;
                    o->animFrame = 1;
                    o->wb6++;
                }
                break;
            }
            break;
        case 20:
            FUN_80026c50(0x2d, 1, 1);
            o->wb6++;
            break;
        case 21:
            {
                int v = D_8009C971;
                D_800A60D0 = v;
                D_800A60D2 = v;
                D_8009C970 = v;
            }
            FUN_8001e4f0(10);
            o->wb6++;
            break;
        case 22:
            o->wb6++;
            o->step = 0;
            o->wba = o->wb6;
            break;
        case 23:
            o->wb6 = o->wba;
            break;
        }
        if (FUN_800202b4(o)) FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
