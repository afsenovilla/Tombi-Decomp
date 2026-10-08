// FUNC 80129fb4 1776 X000
// MATCHING 80129fb4 1776
#include "TOBJ.H"
typedef struct { unsigned char b0, b1; char p2[8]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077CF4[];
extern unsigned char D_80138FB8[];
extern unsigned char D_80138FC8[];
extern unsigned char D_80138FD8[];
extern unsigned short *D_8013A160b[]; /* same table as D_8013A160: a second name keeps case 3 from being cross-jumped into case 4 */
extern unsigned short *D_8013A160[], *D_8013A178[], *D_8013A17C[], *D_8013A188[], *D_8013A200[];
extern unsigned short DAT_1f80016a;
extern short DAT_1f80016e;
extern unsigned short DAT_1f800172;
extern unsigned short D_1F800172[];
extern int FUN_8001f9e0(void);
extern void FUN_8001fb20(TObj *);
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001e560(int, int);
extern void FUN_80127380(TObj *);
extern int FUN_801274cc(TObj *);
extern int FUN_801275e4(TObj *);
extern unsigned char FUN_80127720(TObj *);
extern int AnimAdvanceWithBox(TObj *);

void FUN_80129fb4(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    unsigned char *p;
    unsigned short *a;
    short s;
    short t;
    unsigned char r;
    unsigned char c;
    int k;
    int m;

    switch (o->substep) {
    case 12:
        o->substep = 0;
        o->animFrame = 1 - o->animFrame;
    case 0:
        o->movetab = D_80077CF4;
        x->b1 = 0;
        o->substep++;
    case 1:
        x->w0e = 0;
        o->b69 = 0;
        o->b9d = 0;
        o->timer = 0x38;
        o->substep++;
        if (D_80138FB8[FUN_8001f9e0() & 0xf] != 0) o->timer = 0x60;
        o->wac = 7;
        a = D_8013A17C[0];
        goto common;
    case 2:
        if (--o->timer == -1) o->substep++;
        FUN_80127380(o);
        o->y.p.whole += 4;
        if (FUN_801274cc(o) == 0 && x->w0e++ > 3) {
            x->b0a = 1;
            x->b0b = 2;
            o->state = 7;
            o->substep = 0;
            break;
        }
        k = FUN_801275e4(o);
        if (k != 0) {
            if (k == 1) {
                o->substep = 8;
                break;
            }
            o->substep = 8;
            if ((unsigned short)(o->d->p.whole - DAT_1f800172 + 0x2d) > 0x5a) break;
            if ((unsigned short)(o->h->p.whole - DAT_1f80016a + 0x60) > 0xc0) break;
            if (DAT_1f80016e < o->y.p.whole + 0x10) break;
            o->substep = 13;
            o->timer = 0x50;
            o->wac = 0x28;
            a = D_8013A200[0];
            goto common;
        }
        r = FUN_80127720(o);
        if (r != 0xff) {
            o->state = r;
            o->substep = 0;
        }
        break;
    case 3:
        c = x->b1;
        if (c == 3) {
            o->substep = 6;
            break;
        }
        x->b1 = c + 1;
        o->substep++;
        x->b0 = 0;
        m = D_80138FC8[FUN_8001f9e0() & 0xf];
        o->wac = m;
        a = D_8013A160b[m];
        goto common;
    case 4:
        FUN_8001fb20(o);
        FUN_801274cc(o);
        if (o->animTimer == 1) {
            k = AnimAdvanceWithBox(o);
            s = ((unsigned short *)o->anim)[2];
            if (s != 0) FUN_8001e560(s >> 8, s & 0xff);
        } else {
            k = AnimAdvanceWithBox(o);
        }
        if (k == 0) break;
        if (x->b0 == 0) {
            r = FUN_80127720(o);
            if (r != 0xff) {
                o->state = r;
                o->substep = 0;
            } else {
                o->substep = 1;
            }
            break;
        }
        o->wac = 3;
        o->substep++;
        if (x->b0 == 1) o->wac = 5;
        a = D_8013A160[o->wac];
        goto common;
    case 5:
        FUN_8001fb20(o);
        FUN_801274cc(o);
        if (o->animTimer == 1) {
            k = AnimAdvanceWithBox(o);
            s = ((unsigned short *)o->anim)[2];
            if (s != 0) FUN_8001e560(s >> 8, s & 0xff);
        } else {
            k = AnimAdvanceWithBox(o);
        }
        if (k == 0) break;
        r = FUN_80127720(o);
        if (r != 0xff) {
            o->state = r;
            o->substep = 0;
        } else {
            o->substep = 1;
        }
        break;
    case 6:
        t = 0x5a;
        goto c10;
    case 7:
        if (--o->timer == -1) {
            FUN_8001f8e4(o);
            o->state = 0;
            o->substep = 0;
            r = FUN_80127720(o);
            if (r == 0xff) break;
            o->state = r;
        }
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        FUN_801274cc(o);
        break;
    case 8:
        o->wac = 6;
        o->substep++;
        a = D_8013A178[0];
        goto common;
    case 9:
        if (o->animTimer == 1) {
            k = AnimAdvanceWithBox(o);
            s = ((unsigned short *)o->anim)[2];
            if (s != 0) FUN_8001e560(s >> 8, s & 0xff);
        } else {
            k = AnimAdvanceWithBox(o);
        }
        if (k != 0) o->substep++;
        FUN_8001fb20(o);
        FUN_801274cc(o);
        break;
    case 10:
        t = 0x3c;
    c10:
        o->timer = t;
        o->b9d = 0;
        o->wac = 10;
        o->substep++;
        a = D_8013A188[0];
    common:
        o->anim = a;
        p = &D_80138FD8[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 11:
        if (--o->timer == -1) {
            o->substep = 12;
            r = FUN_80127720(o);
            if (r == 0xff) break;
            o->state = r;
            o->substep = 0;
        }
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        FUN_801274cc(o);
        break;
    case 13:
        AnimAdvanceWithBox(o);
        if (--o->timer != -1) break;
        o->animFrame = 0;
        o->substep = 0;
        if ((unsigned short)(o->d->p.whole - D_1F800172[0] + 0x2d) > 0x5a) break;
        if ((unsigned short)(o->h->p.whole - DAT_1f80016a + 0x40) > 0x80) break;
        if (DAT_1f80016e < o->y.p.whole + 0x10) break;
        o->animFrame = 1;
        x->b0a = 1;
        x->b0b = 2;
        o->state = 6;
        o->substep = 0;
        break;
    }
}
