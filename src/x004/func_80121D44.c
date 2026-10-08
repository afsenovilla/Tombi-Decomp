// FUNC 80121d44 1480 X004
// MATCHING 80121d44 1480
#include "TOBJ.H"
typedef struct { unsigned char b0, b1; char p2[8]; unsigned char b0a, b0b; char p0c[2]; short w0e; } X;
extern char D_80077CF4[];
extern unsigned char D_80130FB4[];
extern unsigned char D_80130FC4[];
extern unsigned char D_80130FD4[];
extern unsigned short *D_80133B5Cb[]; /* same table as D_80133B5C: a second name keeps case 3 from being cross-jumped into case 4 (as in X000 FUN_80129fb4) */
extern unsigned short *D_80133B5C[]; /* same table as D_80133B5C: a second name keeps case 3 from being cross-jumped into case 4 */
extern unsigned short *D_80133B5Cb[]; /* same table as D_80133B5C: a second name keeps case 3 from being cross-jumped into case 4 (as in X000 FUN_80129fb4) */
extern unsigned short *D_80133B5C[], *D_80133B74[], *D_80133B78[], *D_80133B84[];
extern int FUN_8001f9e0(void);
extern void FUN_8001fb20(TObj *);
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001e560(int, int);
extern void func_8011FC88(TObj *);
extern int func_8011FDD4(TObj *);
extern int func_8011FEF0(TObj *);
extern unsigned char FUN_8011ffdc(TObj *);
extern int AnimAdvanceWithBox(TObj *);

void func_80121D44(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    unsigned char *p;
    unsigned short *a;
    short s;
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
        if (D_80130FB4[FUN_8001f9e0() & 0xf] != 0) o->timer = 0x60;
        o->wac = 7;
        a = D_80133B78[0];
        goto common;
    case 2:
        if (--o->timer == -1) o->substep++;
        func_8011FC88(o);
        o->y.p.whole += 4;
        if (func_8011FDD4(o) == 0 && x->w0e++ > 3) {
            x->b0b = 6;
            x->b0a = 0;
            o->state = 7;
            o->substep = 0;
            break;
        }
        if (func_8011FEF0(o)) {
            o->substep = 8;
            break;
        }
        r = FUN_8011ffdc(o);
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
        m = D_80130FC4[FUN_8001f9e0() & 0xf];
        o->wac = m;
        a = D_80133B5Cb[m];
        goto common;
    case 4:
        FUN_8001fb20(o);
        func_8011FDD4(o);
        if (o->animTimer == 1) {
            k = AnimAdvanceWithBox(o);
            s = ((unsigned short *)o->anim)[2];
            if (s != 0) FUN_8001e560(s >> 8, s & 0xff);
        } else {
            k = AnimAdvanceWithBox(o);
        }
        if (k == 0) break;
        if (x->b0 == 0) {
            r = FUN_8011ffdc(o);
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
        a = D_80133B5C[o->wac];
        goto common;
    case 5:
        FUN_8001fb20(o);
        func_8011FDD4(o);
        if (o->animTimer == 1) {
            k = AnimAdvanceWithBox(o);
            s = ((unsigned short *)o->anim)[2];
            if (s != 0) FUN_8001e560(s >> 8, s & 0xff);
        } else {
            k = AnimAdvanceWithBox(o);
        }
        if (k == 0) break;
        r = FUN_8011ffdc(o);
        if (r != 0xff) {
            o->state = r;
            o->substep = 0;
        } else {
            o->substep = 1;
        }
        break;
    case 6:
        o->timer = 0x5a;
        o->wac = 10;
        o->movetab = D_80077CF4;
        o->b9d = 0;
        o->b9c = 0;
        o->substep++;
        a = D_80133B5C[o->wac];
        goto common;
    case 7:
        if (--o->timer == -1) {
            FUN_8001f8e4(o);
            o->state = 0;
            o->substep = 0;
            r = FUN_8011ffdc(o);
            if (r == 0xff) break;
            o->state = r;
        }
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        func_8011FDD4(o);
        break;
    case 8:
        o->wac = 6;
        o->substep++;
        a = D_80133B74[0];
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
        func_8011FDD4(o);
        break;
    case 10:
        o->timer = 0x3c;
        o->b9d = 0;
        o->wac = 10;
        o->substep++;
        a = D_80133B84[0];
    common:
        o->anim = a;
        p = &D_80130FD4[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 11:
        if (--o->timer == -1) {
            o->substep = 12;
            r = FUN_8011ffdc(o);
            if (r == 0xff) break;
            o->state = r;
            o->substep = 0;
        }
        AnimAdvanceWithBox(o);
        FUN_8001fb20(o);
        func_8011FDD4(o);
        break;
    }
}
