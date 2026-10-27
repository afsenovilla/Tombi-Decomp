// FUNC 80118188 656 X010
// MATCHING 80118188 656
#include "spad.h"
#include "TOBJ.H"

extern char D_80132C94[];
extern char D_80132C9C[];
extern char D_80132CA4[];
extern char D_80132CAC[];
extern char D_80132CB4[];
extern char D_80132CBC[];
static void *D_80132C3C[6] = {
    D_80132C94, D_80132C9C, D_80132CA4, D_80132CAC,
    D_80132CB4, D_80132CBC,
};
extern char D_80132CC4[];
extern char D_80132CCC[];
extern char D_80132CD4[];
extern char D_80132CDC[];
extern char D_80132CE4[];
extern char D_80132CEC[];
extern char D_80132CF4[];
extern char D_80132D18[];
extern char D_80132D3C[];
extern char D_80132D60[];
extern char D_80132D84[];
extern char D_80132DA8[];
static void *D_80132C54[12] = {
    D_80132CC4, D_80132CCC, D_80132CD4, D_80132CDC,
    D_80132CE4, D_80132CEC, D_80132CF4, D_80132D18,
    D_80132D3C, D_80132D60, D_80132D84, D_80132DA8,
};
extern unsigned short SCREEN_ORIGIN_Y;
extern unsigned short GetClut(int, int);
extern int Rand(void);
extern short MulNegSin(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

void updateScreenDecorSprite(TObj *o)
{
    short v;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->w08 = GetClut(0x80, 0x1ff);
        o->b0a = 7;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        switch (o->subtype) {
        case 0:
            o->b0d = 1;
            o->w08 = GetClut(0x80, 0x1ff);
            o->w1e = 4;
            o->anim = D_80132C3C[o->b0c];
            break;
        case 1:
            o->b0d = 0x81;
            o->w08 = GetClut(0x90, 0x1e0);
            o->w1e = 6;
            o->anim = D_80132C54[o->b0c];
            break;
        case 2:
            o->b0d = 1;
            o->w08 = GetClut(0x90, 0x1e0);
            o->w1e = 6;
            o->anim = D_80132C54[o->b0c];
            o->timer = Rand() & 7;
            o->w22 = 0;
            break;
        }
        o->d3c = SPAD.i2dc;
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->subtype == 2) {
            v = MulNegSin(o->w22, 2);
            o->a.p.whole = v + (o->d30 - (*(unsigned short*)((char*)&SPAD.i174 + 2)));
            o->w22 = (o->w22 + o->timer) & 0xff;
            o->y.p.whole = o->d34 - SCREEN_ORIGIN_Y;
            if (v < 0) {
                o->animFrame = 0;
            } else {
                o->animFrame = 1;
            }
        } else {
            *(short *)((char *)o + 0x12) = o->d30 - (*(unsigned short*)((char*)&SPAD.i174 + 2));
            o->y.p.whole = o->d34 - SCREEN_ORIGIN_Y;
        }
        o->visible = 1;
        ObjListPush_1F800220(o);
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
