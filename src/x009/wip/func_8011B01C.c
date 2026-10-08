// FUNC 8011b01c 908 X009
/* score 145: whole function (starts with the D_800A6039 load before addiu sp, which already matches; covers csv
   pieces 8011B170/8011B330). Left: the case-0 flag n should share s0 with the angle (game s0, ours a0); the
   (signed char)p->c reads must stay lbu+sll/sra 24 (ours lb or a short copy); a = v & 0xff then sll/sra 16 in a
   shared tail; several global loads ordered after stores (some globals already [0]). */
#include "TOBJ.H"
typedef struct { unsigned char a, b, c; signed char d; } P4;
extern unsigned char D_800A6039;
extern unsigned char D_8009C940, D_8009C941;
extern unsigned char D_8009CFF8;
extern unsigned short *D_800A605C;
extern signed char D_80011EB4[];
extern P4 D_80011E5C[];
extern void *D_8012E024;
extern int D_800A60C4[];
extern unsigned short D_800A6066;
extern Fix16 *D_800A6078[0], *D_800A607C;
extern unsigned short D_800A604E[];
extern unsigned char D_800A6047;
extern short FUN_8001fddc(short, int);
extern short FUN_8001fdac(short, int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001e4f0(int);
extern void FUN_80026e0c(int, int);
extern void FUN_80026c50(int, int, int);
extern void FUN_8005a9a4(int, int);

void func_8011B01C(TObj *o)
{
    P4 *p;
    short a, dx, dy;
    int v;
    short c;

    o->visible = D_800A6039;
    switch (o->state) {
    case 0:
        a = 0;
        if (D_8009C940 != 0) {
            switch (D_8009C941) {
            case 5:
            case 7:
            case 0xe:
            case 0x7c:
            case 0x97:
            case 0x98:
                a++;
            }
        }
        if (a) {
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        p = &D_80011E5C[D_80011EB4[*D_800A605C]];
        if (D_8009CFF8 == 0) {
            o->b04 = 3;
            break;
        }
        o->anim = D_8012E024;
        c = (signed char)p->c;
        if (o->animFrame & 1) {
            v = D_800A60C4[0] + 0x80;
            v -= c;
        } else
            v = c + D_800A60C4[0];
        a = v & 0xff;
        dx = FUN_8001fddc(a, p->d);
        dy = FUN_8001fdac(a, p->d);
        o->visible = D_800A6039;
        o->animFrame = D_800A6066 & 1;
        o->h->p.whole = D_800A6078[0]->p.whole + dx;
        o->y.p.whole = D_800A604E[0] + dy;
        o->d->p.whole = D_800A607C->p.whole;
        o->d8c = D_800A60C4[0];
        o->category |= 0x80;
        o->b0f = D_800A6047 + p->b;
        FUN_8001fe6c(o);
        break;
    case 1:
        p = &D_80011E5C[D_80011EB4[*D_800A605C]];
        c = (signed char)p->c;
        if (o->animFrame & 1) {
            v = D_800A60C4[0] + 0x80;
            v -= c;
        } else
            v = c + D_800A60C4[0];
        a = v & 0xff;
        o->h->p.whole = D_800A6078[0]->p.whole + FUN_8001fddc(a, p->d);
        o->y.p.whole = D_800A604E[0] + FUN_8001fdac(a, p->d);
        o->d->p.whole = D_800A607C->p.whole;
        o->d8c = D_800A60C4[0];
        o->b0f = D_800A6047 + p->b;
        FUN_8001e4f0(9);
        o->b04 = 3;
        FUN_80026e0c(0xc, 1);
        FUN_80026c50(0xd, 1, 1);
        FUN_8005a9a4(0x28, 1);
        o->state = 3;
        break;
    }
}
