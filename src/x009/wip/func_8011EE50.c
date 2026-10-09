// FUNC 8011ee50 728 X009
/* score 2: only the state==2 compare differs: game compares with a1 (=2, the constant also
   used as call arg in the delay slot) where gcc reuses v1 (b04 switch value known to be 2).
   Tried: if/else instead of switch. */
#include "TOBJ.H"
extern int D_1F8002D4[];
extern char D_80077D6C[];
extern void *D_8012E04C;
extern void *D_8012E050;
extern unsigned char D_8009CEF6;
extern unsigned char D_8009CFE5[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018744(TObj *);
extern void FUN_8001e4f0(int);
extern void FUN_800e9f74(int, int, int, int);
extern short FUN_800411cc(TObj *, short, short);
extern int func_8011ECC8(TObj *);
extern void func_8004D620(int, int);

static __inline__ void setbox(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

void func_8011EE50(TObj *o)
{
    unsigned char *c;

    switch (o->b04) {
    case 0:
        o->b04++;
        setbox(o, 8, 0x10, 8, 0x10);
        o->w1e = 9;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8012E04C;
        FUN_8001fe6c(o);
        o->d8c = 0;
        o->active = 2;
        o->ba5 = 0;
        o->w98 = 0;
        o->movetab = D_80077D6C;
        o->b0a = 2;
        break;
    case 1:
        FUN_8001fec0(o);
        if (o->animFrame & 1)
            o->d8c = (o->d8c + 0x14) & 0xff;
        else
            o->d8c = (o->d8c - 0x14) & 0xff;
        o->y.raw += 0x50000;
        if (func_8011ECC8(o) || FUN_800411cc(o, o->h->p.whole, o->y.p.whole + 8)) {
            o->active = 2;
            o->ba5 = 0;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        }
        if (!FUN_800202b4(o)) {
            o->b04 = 2;
            o->step = 1;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            switch (o->state) {
            case 0:
                o->b0b = 1;
                o->d8c = 0;
                o->b0f = 4;
                o->ba5 = 0;
                o->y.p.whole -= 10;
                FUN_8001e4f0(7);
                FUN_800e9f74(0x1f4, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                o->anim = D_8012E050;
                FUN_8001fe6c(o);
                o->state++;
                break;
            case 2:
                if (FUN_8001fec0(o)) {
                    o->state = 0;
                    o->step++;
                }
                break;
            }
            if (!FUN_800202b4(o))
                o->step++;
            break;
        case 1:
            c = &D_8009CEF6;
            func_8004D620(*c + 0x29, 2);
            (*c)++;
            D_8009CFE5[o->b0c] = 1;
            o->b04++;
            break;
        }
        break;
    case 3:
        FUN_80018744(o);
        break;
    }
}
