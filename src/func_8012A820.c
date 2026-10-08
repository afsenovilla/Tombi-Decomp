// FUNC 8012a820 1532 X000
// MATCHING 8012a820 1532
#include "TOBJ.H"
extern char D_80077CF4[];
extern unsigned char D_800A603E;
extern unsigned char D_800A60E4[];
extern void *D_8013A1EC[];
extern void *D_8013A1F0[];
extern void *D_8013A1F4[];
extern void *D_8013A1F8[];
extern unsigned char D_80138FD8[];
extern void FUN_80020aec(int);
extern void FUN_80026bfc(int, int);
extern void func_8004065C(TObj *, short, short, int);
extern int FUN_801274cc(TObj *);
extern void FUN_8001fb20(TObj *);
extern void FUN_8001faf4(TObj *);
extern TObj *ObjAlloc();
extern short D_1F80027E;
extern short DAT_1f80027e;

#define SETBOX(o) \
    { \
        unsigned char *b = D_80138FD8 + ((unsigned short *)o->anim)[1] * 4; \
        o->box0 = *b++; \
        o->box1 = *b++; \
        o->box2 = b[0]; \
        o->box3 = b[1]; \
    }
#define ANIMT(o) o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff

#define COLLIDE(o) \
    func_8004065C(o, o->h->p.whole + 8, o->y.p.whole + 8, 0); \
    func_8004065C(o, o->h->p.whole - 8, o->y.p.whole + 8, 1);

void func_8012A820(TObj *o)
{
    TObj *n;
    short a;
    int off;

    switch (o->state) {
    case 0:
        o->b9c = 0;
        o->category |= 0x80;
        if ((unsigned)(o->subtype - 9) < 2) {
            FUN_80020aec(o->b6b);
        }
        FUN_80026bfc(0, 6);
        o->movetab = D_80077CF4;
        *(signed char *)&o->b0f = -7;
        o->b68 = 0;
        o->d8c = 0;
        o->velV = 0;
        o->state++;
    case 1:
        o->velV += 0x40;
        if (o->velV > 0x500) {
            o->velV = 0x500;
        }
        COLLIDE(o);
        o->y.raw += o->velV << 8;
        if (FUN_801274cc(o)) {
            o->velV = 0;
        }
        break;
    case 2:
        D_800A603E = 2;
        o->timer = 4;
        o->wac = 0x23;
        o->state++;
        goto set23;
    case 3:
        if (--o->timer == -1) {
            o->timer = 8;
            o->wac = 0x24;
            o->state++;
            o->anim = D_8013A1F0[0];
            SETBOX(o);
            ANIMT(o);
            n = ObjAlloc();
            if (n != 0) {
                n->active = 1;
                n->type = 0x10;
                n->subtype = 0;
                off = -16;
                if (o->animFrame & 1) off = 16;
                n->a.p.whole = o->a.p.whole + off;
                n->y.p.whole = o->y.p.whole;
                n->b.p.whole = o->b.p.whole;
                n->animFrame = o->animFrame & 1;
            }
        }
        FUN_8001fb20(o);
        COLLIDE(o);
        FUN_801274cc(o);
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0x25;
            o->anim = D_8013A1F4[0];
            SETBOX(o);
            ANIMT(o);
            o->state++;
            o->timer = 0xe;
        }
        FUN_8001fb20(o);
        COLLIDE(o);
        FUN_801274cc(o);
        break;
    case 5:
        FUN_8001faf4(o);
        o->y.p.whole += 3;
        COLLIDE(o);
        if (FUN_801274cc(o)) {
            a = D_1F80027E;
            if (a < 0) a = -a;
            if (a > 8) a = 8;
            if (DAT_1f80027e < 0) a = -a;
            o->d8c = -a & 0xff;
            o->wb2 = a;
            o->wb6 = (-a << 2) & 0xff;
            o->wae = *(unsigned short *)0x1F800284;
            o->b9c = 0;
            if ((*(unsigned short *)0x1F800282 >> 5) & 8) {
                o->w98 = 0;
            }
        }
        if (--o->timer == -1) {
            D_800A60E4[0] = 3;
            o->wac = 0x23;
            o->anim = D_8013A1EC[0];
            SETBOX(o);
            ANIMT(o);
            o->state++;
        }
        break;
    case 6:
        o->wac = 0x23;
    set23:
        o->anim = D_8013A1EC[0];
        SETBOX(o);
        ANIMT(o);
        FUN_8001fb20(o);
        COLLIDE(o);
        FUN_801274cc(o);
        break;
    case 7:
        o->wac = 0x26;
        o->anim = D_8013A1F8[0];
        SETBOX(o);
        ANIMT(o);
        o->state++;
        break;
    case 8:
        break;
    }
}
