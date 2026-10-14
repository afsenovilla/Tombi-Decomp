// FUNC 8011e54c 1668 X006
// MATCHING 8011e54c 1668
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern unsigned char SCRIPT_FLAGS[];
extern void *D_80122C14[], *D_80122C18[], *D_80122C1C[];
extern char D_80077CDC[];
extern short *D_8012037C[];
extern int func_8011E364(void);
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern int showMessageBox(int, int, int, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void FUN_80026c50(int, int, int);
extern void increaseMaxHealth(void);
extern void printInfoMessage(int, int);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern void freeObjectLayer2(TObj *);

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
            if (GCD.bd0fd) o->wb4 = 5;
            else o->wb4 = 4;
            break;
        case 1:
            if (GCD.bd0fc) o->wb4 = 3;
            else o->wb4 = 2;
            break;
        case 2:
            if (GCD.bd0fb) o->wb4 = 1;
            else o->wb4 = 0;
            break;
        default:
            if (GCD.bd0fb) o->wb4 = 1;
            if (GCD.bd0fc) o->wb4 = 3;
            if (GCD.bd0fd) o->wb4 = 5;
            break;
        }
        o->active = 2;
        o->w1e = 8;
        o->wb6 = 0;
        o->b0d = 1;
        o->w08 = FUN_8005e420(0x90, 0x1e0);
        {
            int d = (int)(*(void **)&SPAD.i2d4);
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
                if (GVARS.b3a != 1) break;
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
                applyAnimVelocityX(o, o->animFrame);
                p = (*(Fix16 **)&PLAYER.obj.h);
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
            o->d90 = showMessageBox(2, e[1], 0xdc, 0x7e);
            o->wb6++;
            break;
            break;
        case 5:
            o->anim = D_80122C14[0];
            o->animFrame = 1;
            FUN_8001fe6c(o);
            o->d90 = showMessageBox(2, e[1], 0xdc, 0x7e);
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
            increaseMaxHealth();
            printInfoMessage(e[1], 3);
            goto next;
            break;
        case 9:
            setEventStarted(e[1], 0, 0);
            o->wb6++;
            break;
            break;
        case 10:
            switch (o->step) {
            case 0:
                if (((unsigned char*)&GCD.bd0a4)[e[1]] != 0) {
                    goto next;
                    break;
                }
                switch (e[1]) {
                case 0x57:
                    setEventStarted(0x85, 0, 1);
                    break;
                case 0x58:
                    setEventStarted(0x86, 0, 1);
                    break;
                case 0x59:
                    setEventStarted(0x87, 0, 1);
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
            setEventComplete(e[1], 0);
            goto next;
            break;
        case 12:
            switch (o->step) {
            case 0:
                if (SCRIPT_FLAGS[e[1]] == 0xff) {
                    goto next;
                    break;
                }
                setEventComplete(e[1], 0);
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
            PLAYER.obj.b04 = 5;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            GVARS.b3f = 1;
            goto next;
            break;
        case 14:
            PLAYER.obj.wb2 = 0;
            PLAYER.obj.b04 = 1;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            GVARS.b3f = 0;
        next:
            o->wb6++;
            break;
        case 15:
            p = (*(Fix16 **)&PLAYER.obj.h);
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
        freeObjectLayer2(o);
        break;
    }
}
