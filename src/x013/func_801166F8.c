// FUNC 801166f8 1508 X013
// MATCHING 801166f8 1508
#include "TOBJ.H"
extern int D_1F8002D4[], D_1F8002DC[], D_1F8002E0[], D_1F8002E4[];
extern short D_1F800176[], D_1F800186[];
extern void *D_8011A8D8, *D_8011A8DC, *D_8011A8E0[], *D_8011A8EC, *D_8011A8F0, *D_8011A8F4[], *D_8011A8F8;
extern void *D_8011A8FC, *D_8011A904, *D_8011A924, *D_8011A92C, *D_8011A934, *D_8011A93C, *D_8011A944, *D_8011A94C;
extern unsigned char D_8009D088;
extern short D_800A604A;
extern unsigned short D_800A6066;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern unsigned short GetClut(int, int);

void func_801166F8(TObj *o)
{
    unsigned char *f;
    unsigned char k;
    int c;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0d = 0x80;
        o->animFrame = 1;
        o->w1e = 0xb;
        o->b0a = 7;
        o->b.p.whole = 0xc8;
        o->b0f = 0;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->d3c = D_1F8002D4[0];
        switch (o->subtype) {
        case 0:
            o->anim = D_8011A8D8;
            break;
        case 1:
            o->b0d = 1;
            o->anim = D_8011A8DC;
            o->timer = 10;
            o->w22 = 0;
            break;
        case 2:
            o->anim = D_8011A8E0[o->b0c];
            break;
        case 3:
            o->b0d = 0x81;
            o->anim = D_8011A8EC;
            o->timer = 10;
            o->w22 = 0;
            break;
        case 4:
            break;
        case 5:
            o->b.p.whole = 0;
            o->anim = D_8011A8F0;
            o->b0d = 1;
            o->w08 = GetClut(0x80, 0x1ff);
            o->animFrame = 1;
            o->w1e = 7;
            o->b0f = 10;
            break;
        case 6:
            o->b.p.whole = 0;
            o->anim = D_8011A8F4[0];
            o->b0d = 1;
            o->w08 = GetClut(0x80, 0x1ff);
            o->animFrame = 1;
            o->w1e = 7;
            o->b0f = 10;
            break;
        case 7:
            o->anim = D_8011A8F8;
            o->b0d = 1;
            o->w08 = GetClut(0x80, 0x1ff);
            o->animFrame = 1;
            o->w1e = 7;
            break;
        case 8:
            o->w1e = 1;
            o->b0d = 1;
            o->animFrame = 0;
            o->w08 = GetClut(0xc0, 0x1e0);
            o->anim = D_8011A8FC;
            o->d3c = D_1F8002D4[0];
            break;
        case 9:
            o->b0d = 0;
            o->anim = D_8011A924;
            o->w1e = 9;
            o->animFrame = 1;
            o->d3c = D_1F8002DC[0];
            break;
        case 10:
            o->b0d = 0;
            o->anim = D_8011A934;
            o->w1e = 10;
            o->animFrame = 1;
            o->d3c = D_1F8002E0[0];
            break;
        case 11:
            o->b0d = 0;
            o->anim = D_8011A944;
            o->w1e = 0xc;
            o->animFrame = 1;
            o->d3c = D_1F8002E4[0];
            break;
        }
        AnimLoadDuration(o);
        break;
    case 1:
        f = &D_8009D088;
        if (*f == 0xff) break;
        o->visible = 1;
        o->a.p.whole = o->d30 - D_1F800176[0];
        o->y.p.whole = o->d34 - D_1F800186[0];
        ObjListPush_1F800220(o);
        k = *f & 0xf;
        switch (o->subtype) {
        case 1:
            if (--o->timer != 0) break;
            c = 0xd0;
            goto clut;
        case 3:
            if (--o->timer != 0) break;
            c = 0xe0;
        clut:
            o->timer = 10;
            o->w22 ^= 1;
            o->w08 = GetClut(c, o->w22 + 0x1ed);
            break;
        case 8:
            if (o->a.p.whole + 0x20 < D_800A604A) o->animFrame = 0;
            if (D_8009D088 & 0x10) break;
            if (k == 0) o->anim = D_8011A904;
            else o->anim = D_8011A8FC;
            { unsigned char *g = &D_8009D088; *g |= 0x10; }
            AnimLoadDuration(o);
            break;
        case 9:
            if (D_800A6066 == 0 && o->a.p.whole + 0x20 < D_800A604A) o->animFrame = 0;
            if (D_8009D088 & 0x20) break;
            if (k == 1) o->anim = D_8011A92C;
            else o->anim = D_8011A924;
            { unsigned char *g = &D_8009D088; *g |= 0x20; }
            AnimLoadDuration(o);
            break;
        case 10:
            if (o->a.p.whole + 0x20 < D_800A604A) o->animFrame = 0;
            if (D_8009D088 & 0x40) break;
            if (k == 2) o->anim = D_8011A93C;
            else o->anim = D_8011A934;
            { unsigned char *g = &D_8009D088; *g |= 0x40; }
            AnimLoadDuration(o);
            break;
        case 11:
            if (o->a.p.whole + 0x20 < D_800A604A) o->animFrame = 0;
            if (D_8009D088 & 0x80) break;
            if (k == 3) o->anim = D_8011A94C;
            else o->anim = D_8011A944;
            { unsigned char *g = &D_8009D088; *g |= 0x80; }
            AnimLoadDuration(o);
            break;
        }
        AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
