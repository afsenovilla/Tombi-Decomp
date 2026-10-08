// FUNC 8011e54c 1668 X006
// MATCHING 8011e54c 1668
#include "TOBJ.H"
extern unsigned char D_8009D0FB, D_8009D0FC, D_8009D0FD;
extern unsigned char D_8009D0A4[], D_8009CDA4[];
extern unsigned char D_8009C93A;
extern unsigned char D_8009C93F;
extern void *D_1F8002D4;
extern void *D_80122C14[], *D_80122C18[], *D_80122C1C[];
extern char D_80077CDC[];
extern short *D_8012037C[];
extern Fix16 *D_800A6078;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern short D_800A60EA;
extern int func_8011E364(void);
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern int FUN_8002dc50(int, int, int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_80026c50(int, int, int);
extern void FUN_80026f4c(void);
extern void FUN_8004d620(int, int);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_80018790(TObj *);

void func_8011E54C(TObj *o)
{
    short *e;
    Fix16 *p;
    char pad[16];

    switch (o->b04) {
    case 0:
        o->wb4 = 6;
        switch (func_8011E364()) {
        case 0:
            if (D_8009D0FD) o->wb4 = 5;
            else o->wb4 = 4;
            break;
        case 1:
            if (D_8009D0FC) o->wb4 = 3;
            else o->wb4 = 2;
            break;
        case 2:
            if (D_8009D0FB) o->wb4 = 1;
            else o->wb4 = 0;
            break;
        default:
            if (D_8009D0FB) o->wb4 = 1;
            if (D_8009D0FC) o->wb4 = 3;
            if (D_8009D0FD) o->wb4 = 5;
            break;
        }
        o->active = 2;
        o->w1e = 8;
        o->wb6 = 0;
        o->b0d = 1;
        o->w08 = FUN_8005e420(0x90, 0x1e0);
        {
            int d = (int)D_1F8002D4;
            void *a = D_80122C14[0];
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
        e = D_8012037C[(unsigned short)o->wb4];
        e += (unsigned short)o->wb6 * 2;
        switch (e[0]) {
        case 0:
            break;
        case 1:
            if (o->timer == 0) {
                o->timer = e[1];
            } else if (--o->timer == 0) {
                goto next;
            }
            break;
        case 2:
            switch (o->step) {
            case 0:
                o->anim = D_80122C18[0];
                FUN_8001fe6c(o);
                o->animFrame = 1;
                o->step++;
            case 1:
                if (D_8009C93A != 1) break;
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 3:
            switch (o->step) {
            case 0:
                o->anim = D_80122C18[0];
                FUN_8001fe6c(o);
                o->animFrame = 1;
                o->step++;
            case 1:
                if (o->h->p.whole < 0xc4) o->animFrame = 0;
                if (o->h->p.whole >= 0xf5) o->animFrame = 1;
                FUN_8001fa88(o, o->animFrame);
                p = D_800A6078;
                if (p->p.whole < 0x91) break;
                p->p.whole = 0x90;
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 4:
            o->anim = D_80122C1C[0];
            o->animFrame = 1;
            FUN_8001fe6c(o);
            o->d90 = FUN_8002dc50(2, e[1], 0xdc, 0x7e);
            o->wb6++;
            break;
            break;
        case 5:
            o->anim = D_80122C14[0];
            o->animFrame = 1;
            FUN_8001fe6c(o);
            o->d90 = FUN_8002dc50(2, e[1], 0xdc, 0x7e);
            o->wb6++;
            break;
            break;
        case 6:
            if (((unsigned char *)o->d90)[4] < 2) break;
            o->anim = D_80122C14[0];
            FUN_8001fe6c(o);
            ((unsigned char *)o->d90)[4]++;
            goto next;
            break;
        case 7:
            FUN_80026c50(e[1], 1, 1);
            o->wb6++;
            break;
            break;
        case 8:
            FUN_80026f4c();
            FUN_8004d620(e[1], 3);
            goto next;
            break;
        case 9:
            FUN_8005a8a8(e[1], 0, 0);
            o->wb6++;
            break;
            break;
        case 10:
            switch (o->step) {
            case 0:
                if (D_8009D0A4[e[1]] != 0) {
                    goto next;
                    break;
                }
                switch (e[1]) {
                case 0x57:
                    FUN_8005a8a8(0x85, 0, 1);
                    break;
                case 0x58:
                    FUN_8005a8a8(0x86, 0, 1);
                    break;
                case 0x59:
                    FUN_8005a8a8(0x87, 0, 1);
                    break;
                }
                o->timer = 0x168;
                o->step++;
                break;
            case 1:
                if (--o->timer == 0) {
                    o->step = 0;
                    o->wb6++;
                }
                break;
            }
            break;
        case 11:
            FUN_8005a9a4(e[1], 0);
            goto next;
            break;
        case 12:
            switch (o->step) {
            case 0:
                if (D_8009CDA4[e[1]] == 0xff) {
                    goto next;
                    break;
                }
                FUN_8005a9a4(e[1], 0);
                o->timer = 0x168;
                o->step++;
                break;
            case 1:
                if (--o->timer == 0) {
                    o->step = 0;
                    o->wb6++;
                }
                break;
            }
            break;
        case 13:
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = 1;
            goto next;
            break;
        case 14:
            D_800A60EA = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = 0;
        next:
            o->wb6++;
            break;
        case 15:
            p = D_800A6078;
            if (p->p.whole >= 0x69) p->p.whole = 0x68;
            break;
        }
        if ((unsigned short)o->wb4 != 6) {
            if (FUN_800202b4(o)) FUN_8001fec0(o);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
