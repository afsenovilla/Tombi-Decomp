// FUNC 8003c980 696 MAIN0
// MATCHING 8003c980 696
#include "TOBJ.H"
#include "raw7.h"
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7;
    short cx, cy;
    unsigned char bx0, bx1, bx2, bx3;
    void **anims;
} Desc;
typedef struct { short x, y, p0, p1; } ClutPos;
extern unsigned char DAT_8007b084[];
extern char D_8007A8F0[];
extern char D_8007A904[];
extern char D_8007A918[];
extern char D_8007A92C[];
extern char D_8007A940[];
extern char D_8007A954[];
extern char D_8007A968[];
extern char D_8007A97C[];
extern char D_8007A990[];
extern char D_8007A9A4[];
extern char D_8007A9B8[];
extern char D_8007A9CC[];
extern char D_8007A9E0[];
extern char D_8007A9F4[];
extern char D_8007AA08[];
extern char D_8007AA1C[];
extern char D_8007AA30[];
extern char D_8007AA44[];
extern char D_8007AA58[];
extern char D_8007AA6C[];
extern char D_8007AA80[];
extern char D_8007AA94[];
extern char D_8007AAA8[];
extern char D_8007AABC[];
extern char D_8007AAD0[];
extern char D_8007AAE4[];
extern char D_8007AAF8[];
extern char D_8007AB0C[];
extern char D_8007AB20[];
extern char D_8007AB34[];
extern char D_8007AB48[];
extern char D_8007AB5C[];
extern char D_8007AB70[];
extern char D_8007AB84[];
extern char D_8007AB98[];
extern char D_8007ABAC[];
extern char D_8007ABC0[];
extern char D_8007ABD4[];
extern char D_8007ABE8[];
extern char D_8007ABFC[];
extern char D_8007AC10[];
extern char D_8007AC24[];
extern char D_8007AC38[];
extern char D_8007AC4C[];
extern char D_8007AC60[];
extern char D_8007AC74[];
extern char D_8007AC88[];
extern char D_8007AC9C[];
extern char D_8007ACB0[];
extern char D_8007ACC4[];
extern char D_8007ACD8[];
extern char D_8007ACEC[];
extern char D_8007AD00[];
extern char D_8007AD14[];
extern char D_8007AD28[];
extern char D_8007AD3C[];
extern char D_8007AD50[];
extern char D_8007AD64[];
extern char D_8007AD78[];
extern char D_8007AD8C[];
extern char D_8007ADA0[];
extern char D_8007ADB4[];
extern char D_8007ADC8[];
extern char D_8007ADDC[];
extern char D_8007ADF0[];
extern char D_8007AE04[];
extern char D_8007AE18[];
extern char D_8007AE2C[];
extern char D_8007AE40[];
extern char D_8007AE54[];
extern char D_8007AE68[];
extern char D_8007AE7C[];
extern char D_8007AE90[];
extern char D_8007AEA4[];
extern char D_8007AEB8[];
extern char D_8007AECC[];
extern char D_8007AEE0[];
extern char D_8007AEF4[];
extern char D_8007AF08[];
extern char D_8007AF1C[];
extern char D_8007AF30[];
static Desc *DAT_8007b14c[81] = {
    (Desc *)D_8007A8F0, (Desc *)D_8007A904, (Desc *)D_8007A918, (Desc *)D_8007A92C,
    (Desc *)D_8007A940, (Desc *)D_8007A954, (Desc *)D_8007A968, (Desc *)D_8007A97C,
    (Desc *)D_8007A990, (Desc *)D_8007A9A4, (Desc *)D_8007A9B8, (Desc *)D_8007A9CC,
    (Desc *)D_8007A9E0, (Desc *)D_8007A9F4, (Desc *)D_8007AA08, (Desc *)D_8007AA1C,
    (Desc *)D_8007AA30, (Desc *)D_8007AA44, (Desc *)D_8007AA58, (Desc *)D_8007AA6C,
    (Desc *)D_8007AA80, (Desc *)D_8007AA94, (Desc *)D_8007AAA8, (Desc *)D_8007AABC,
    (Desc *)D_8007AAD0, (Desc *)D_8007AAE4, (Desc *)D_8007AAF8, (Desc *)D_8007AB0C,
    (Desc *)D_8007AB20, (Desc *)D_8007AB34, (Desc *)D_8007AB48, (Desc *)D_8007AB5C,
    (Desc *)D_8007AB70, (Desc *)D_8007AB84, (Desc *)D_8007AB98, (Desc *)D_8007ABAC,
    (Desc *)D_8007ABC0, (Desc *)D_8007ABD4, (Desc *)D_8007ABE8, (Desc *)D_8007ABFC,
    (Desc *)D_8007AC10, (Desc *)D_8007AC24, (Desc *)D_8007AC38, (Desc *)D_8007AC4C,
    (Desc *)D_8007AC60, (Desc *)D_8007AC74, (Desc *)D_8007AC88, (Desc *)D_8007AC9C,
    (Desc *)D_8007ACB0, (Desc *)D_8007ACC4, (Desc *)D_8007ACD8, (Desc *)D_8007ACEC,
    (Desc *)D_8007AD00, (Desc *)D_8007AD14, (Desc *)D_8007AD28, (Desc *)D_8007AD3C,
    (Desc *)D_8007AD50, (Desc *)D_8007AD64, (Desc *)D_8007AD78, (Desc *)D_8007AD8C,
    (Desc *)D_8007ADA0, (Desc *)D_8007ADB4, (Desc *)D_8007ADC8, (Desc *)D_8007ADDC,
    (Desc *)D_8007ADF0, (Desc *)D_8007AE04, (Desc *)D_8007AE18, (Desc *)D_8007AE2C,
    (Desc *)D_8007AE40, (Desc *)D_8007AE54, (Desc *)D_8007AE68, (Desc *)D_8007AE7C,
    (Desc *)D_8007AE90, (Desc *)D_8007AEA4, (Desc *)D_8007AEB8, (Desc *)D_8007AECC,
    (Desc *)D_8007AEE0, (Desc *)D_8007AEF4, (Desc *)D_8007AF08, (Desc *)D_8007AF1C,
    (Desc *)D_8007AF30,
};
extern ClutPos DAT_8007b2d4[];
extern unsigned char DAT_8009ce5b;
extern unsigned char DAT_8009d0c1;
extern unsigned char DAT_8009d0bc;
extern int SPR_DATA[];
extern unsigned short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);

void initItemObject(TObj *o)
{
    Desc *d;
    int v;

    if (o->subtype == 100 && DAT_8009ce5b == 0) {
        o->b04 = 2;
        return;
    }
    o->b68 = 0;
    o->b69 = 0;
    d = DAT_8007b14c[DAT_8007b084[o->subtype]];
    o->b0a = d->b0;
    o->w1e = d->b2;
    (*(signed char*)&o->b0f) = -9;
    o->b0d = d->b5;
    o->box0 = d->bx0;
    o->box1 = d->bx1;
    o->box2 = d->bx2;
    o->box3 = d->bx3;
    o->d3c = SPR_DATA[d->b4];
    o->animFrame = 1;
    if (o->subtype == 2) {
        if ((o->b0c & 0x7f) >= 4) {
            o->d64 = 0x2000;
            o->box0 = d->bx0 * 2;
            o->box1 = d->bx1 * 2;
            o->box2 = d->bx2 * 2;
            o->box3 = d->bx3 * 2;
        } else {
            o->d64 = 0x1000;
        }
    }
    switch (d->b6) {
    case 0:
        o->w08 = FUN_8005e420(d->cx, d->cy);
        break;
    case 1:
        o->w08 = FUN_8005e420(d->cx, d->cy + (o->b0c & 0x7f));
        break;
    case 2:
        o->w08 = FUN_8005e420(DAT_8007b2d4[o->b0c & 0x7f].x, DAT_8007b2d4[o->b0c & 0x7f].y);
        break;
    case 3:
        if (DAT_8009d0c1 == 0)
            v = 0;
        else if (DAT_8009d0bc == 0)
            v = 1;
        else
            v = 2;
        o->w08 = FUN_8005e420(d->cx, d->cy + v);
        break;
    }
    switch (d->b7) {
    case 0:
        o->anim = d->anims[0];
        break;
    case 1:
        o->anim = d->anims[o->b0c & 0x7f];
        break;
    }
    FUN_8001fe6c(o);
}
