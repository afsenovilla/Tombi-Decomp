// FUNC 80129ef8 2024 X009
// MATCHING 80129ef8 2024
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"

typedef struct V { char c[12]; } V;
extern TObj *SCRIPT_OBJECTS;
extern TObj PLAYER;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned short D_800A6066[];
extern Fix16 *D_800A6078;
extern short D_800A60EA[];
extern TObj *showMessageBoxDirect(int, int, V *);
extern void FUN_800eea7c(TObj *, short, short);
extern void func_80128AF8(TObj *, int, int);
extern void playSFX(int);
extern void SfxPlay2(int, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);

void aspiringActorGuardTalk(TObj *o)
{
    TObj *p = SCRIPT_OBJECTS;
    V v;

    switch (o->state) {
    case 0:
        if (((unsigned char*)&GCD.bce54)[0] == 0xff) {
            o->b04 = 3;
        }
        o->state++;
        break;
    case 1:
        if (p->b68 == 0)
            break;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        PLAYER.b04 = 5;
        D_800A603D[0] = 0x64;
        D_800A603E[0] = 0;
        p = &PLAYER;
        FUN_800eea7c(p, 0, 0);
        o->state++;
        if (((unsigned char*)&GCD.bce53)[0] == 1 && ((unsigned char*)&GV2.bb1)[0] == 1) {
            playSFX(0x20);
            FUN_800eea7c(p, 0x28, 0);
            o->w08 = 0x78;
            o->state = 4;
        }
        if (((unsigned char*)&GCD.bce53)[0] == 0xff) {
            o->state = 6;
            if (((unsigned char*)&GCD.bce54)[0] == 1 && ((unsigned char*)&GV2.bb1)[0] == 2) {
                SfxPlay2(0x25, 0x28);
                FUN_800eea7c(&PLAYER, 0x29, 0);
                o->w08 = 0x78;
                o->state = 8;
            }
        }
        break;
    case 2:
        v = *(V *)&p->a;
        p->d90 = (int)showMessageBoxDirect(2, 0, &v);
        func_80128AF8(p, 0x20, 0);
        FUN_800eea7c(&PLAYER, 0, 0);
        o->state++;
        break;
    case 3: {
        TObj *q = (TObj *)p->d90;

        if (q->b04 != 2)
            break;
        q->b04 = 3;
        o->w08 = 1;
        if (((unsigned char*)&GCD.bce53)[0] == 0) {
            o->w08 = 0xfa;
            setEventStarted(0xaf, 0, 0);
        }
        o->state = 0x14;
        if (((unsigned char*)&GV2.bb1)[0] == 1) {
            playSFX(0x20);
            FUN_800eea7c(&PLAYER, 0x28, 0);
            o->w08 = 0xc8;
            o->state = 0x12;
        }
        func_80128AF8(p, 0x14, 0);
        break;
    }
    case 4:
        if (--o->w08 > 0)
            break;
        FUN_800eea7c(&PLAYER, 0, 0);
        v = *(V *)&p->a;
        p->d90 = (int)showMessageBoxDirect(2, 1, &v);
        func_80128AF8(p, 0x20, 0);
        o->state++;
        break;
    case 5: {
        TObj *q = (TObj *)p->d90;

        if (q->b04 != 2)
            break;
        q->b04 = 3;
        setEventComplete(0xaf, 0);
        o->w08 = 0x12c;
        o->state = 0x14;
        func_80128AF8(p, 0x14, 0);
        break;
    }
    case 6:
        v = *(V *)&p->a;
        p->d90 = (int)showMessageBoxDirect(2, 2, &v);
        func_80128AF8(p, 0x20, 0);
        o->state++;
        break;
    case 7: {
        TObj *q = (TObj *)p->d90;

        if (q->b04 != 2)
            break;
        q->b04 = 3;
        o->w08 = 1;
        if (((unsigned char*)&GCD.bce54)[0] == 0) {
            o->w08 = 0xfa;
            setEventStarted(0xb0, 0, 0);
        }
        o->state = 0x14;
        if (((unsigned char*)&GV2.bb1)[0] == 2) {
            SfxPlay2(0x25, 0x28);
            FUN_800eea7c(&PLAYER, 0x29, 0);
            o->w08 = 0xc8;
            o->state = 0x13;
        }
        func_80128AF8(p, 0x14, 0);
        break;
    }
    case 8:
        if (--o->w08 > 0)
            break;
        FUN_800eea7c(&PLAYER, 0, 0);
        v = *(V *)&p->a;
        p->d90 = (int)showMessageBoxDirect(2, 3, &v);
        o->state++;
        func_80128AF8(p, 0x20, 0);
        break;
    case 9: {
        TObj *q = (TObj *)p->d90;

        if (q->b04 != 2)
            break;
        q->b04 = 3;
        setEventComplete(0xb0, 0);
        o->w08 = 0x78;
        o->state++;
        func_80128AF8(p, 0x14, 0);
        break;
    }
    case 10:
        if (--o->w08 > 0)
            break;
        p->active = 2;
        p->step = 1;
        p->state = 0;
        o->w08 = 0x50;
        o->state++;
        break;
    case 11:
        if (--o->w08 > 0)
            break;
        v = *(V *)&p->a;
        p->d90 = (int)showMessageBoxDirect(2, 4, &v);
        p->animFrame = 0;
        p->step = 0;
        p->state = 0;
        func_80128AF8(p, 0x20, 0);
        o->state++;
        break;
    case 12: {
        TObj *q = (TObj *)p->d90;

        if (q->b04 != 2)
            break;
        q->b04 = 3;
        o->w08 = 0xc8;
        p->step = 1;
        p->state = 0;
        o->state++;
        break;
    }
    case 13:
        if (--o->w08 <= 0) {
            o->w08 = 1;
            o->state = 0x14;
        }
        break;
    case 18:
        if (--o->w08 > 0)
            break;
        FUN_800eea7c(&PLAYER, 0, 0);
        o->w08 = 100;
        o->state = 4;
        break;
    case 19:
        if (--o->w08 > 0)
            break;
        FUN_800eea7c(&PLAYER, 0, 0);
        o->w08 = 100;
        o->state = 8;
        break;
    case 20:
        if (--o->w08 > 0)
            break;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        D_800A60EA[0] = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        p->b68 = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    case 21:
        D_800A6066[0] = p->h->p.whole < D_800A6078->p.whole;
        if (p->visible == 0) {
            D_800A603C[0] = 1;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            ((unsigned char*)&GVARS.b3e)[0] = 0;
            D_800A60EA[0] = 0;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            p->b04 = 3;
            o->b04 = 3;
        }
        break;
    }
}
