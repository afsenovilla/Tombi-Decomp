// FUNC 80116d18 1208 X017
// MATCHING 80116d18 1208
#include "TOBJ.H"

extern void *D_1F8002D4;
extern unsigned short D_1F800176[], D_1F800186[];
extern unsigned char D_8009CFD6[], D_8009CDF1;
extern unsigned short D_8009C962;
extern void *D_8011C12C[], *D_8011C134[], *D_8011C138[], *D_8011C13C[], *D_8011C140[];
extern void *D_8011C148, *D_8011C14C, *D_8011C150[], *D_8011C154[], *D_8011C158[];
extern void *D_8011C160, *D_8011C164[], *D_8011C168[], *D_8011C170[], *D_8011C174, *D_8011C178, *D_8011C17C;
extern short GetClut(int, int);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

void func_80116D18(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->step = 0;
        o->animFrame = 1;
        o->b04++;
        o->w08 = GetClut(0x80, 0x1ff);
        o->b0a = 7;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        switch (o->subtype) {
        case 0:
            o->b0d = 1;
            o->w1e = 6;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            if (D_8009CFD6[0]) o->anim = D_8011C134[0];
            else o->anim = D_8011C12C[0];
            break;
        case 1:
            o->b0d = 1;
            o->w1e = 6;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            o->anim = D_8011C138[0];
            break;
        case 2:
            o->b0d = 1;
            o->w1e = 8;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            o->anim = D_8011C13C[0];
            break;
        case 3:
            o->b0d = 1;
            o->w1e = 2;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            o->anim = D_8011C140[o->b0c];
            break;
        case 4:
            o->b0d = 1;
            o->w1e = 2;
            o->b0f = 2;
            o->b.p.whole = 0xc8;
            o->anim = D_8011C148;
            break;
        case 5:
            o->b0d = 1;
            o->w1e = 4;
            o->b0f = 2;
            o->b.p.whole = 0xc8;
            o->anim = D_8011C14C;
            break;
        case 6:
            o->b0d = 1;
            o->w1e = 4;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            o->anim = D_8011C150[o->b0c];
            break;
        case 7:
            o->b0d = 1;
            o->w1e = 4;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            o->anim = D_8011C158[o->b0c];
            break;
        case 8:
            o->b0d = 1;
            o->w1e = 6;
            *(signed char *)&o->b0f = -10;
            o->b.p.whole = 0x64;
            o->anim = D_8011C160;
            break;
        case 9:
            o->b0d = 1;
            o->w1e = 6;
            o->b.p.whole = 0x64;
            *(signed char *)&o->b0f = -10;
            o->anim = D_8011C164[0];
            break;
        case 10:
            o->b0d = 1;
            o->w1e = 6;
            o->b.p.whole = 0xc8;
            o->b0f = 2;
            o->anim = D_8011C168[o->b0c];
            break;
        case 11:
            o->b0d = 1;
            o->w1e = 6;
            o->b0f = 2;
            o->b.p.whole = 0xc8;
            o->anim = D_8011C174;
            break;
        case 12:
            o->b0d = 1;
            o->w1e = 6;
            o->b0f = 2;
            o->b.p.whole = 0x12c;
            o->anim = D_8011C178;
            break;
        case 13:
            o->b0d = 1;
            o->w1e = 6;
            o->b0f = 2;
            o->b.p.whole = 0xc8;
            o->anim = D_8011C17C;
            break;
        }
        o->d3c = (int)D_1F8002D4;
        AnimLoadDuration(o);
        break;
    case 1:
        o->visible = 1;
        o->a.p.whole = o->d30 - D_1F800176[0];
        o->y.p.whole = o->d34 - D_1F800186[0];
        ObjListPush_1F800220(o);
        if (D_8009C962 == 5) {
            switch (o->step) {
            case 0:
                if (D_8009CDF1 != 0xff) break;
                switch (o->subtype) {
                case 6:
                    o->anim = D_8011C154[0];
                    AnimLoadDuration(o);
                    break;
                case 10:
                    o->anim = D_8011C170[0];
                    AnimLoadDuration(o);
                    break;
                }
                o->step++;
                break;
            case 1:
                AnimAdvance(o);
                break;
            }
        } else {
            AnimAdvance(o);
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
