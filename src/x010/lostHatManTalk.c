// FUNC 801253ac 1844 X010
// MATCHING 801253ac 1844
#include "gcd.h"
#include "gvars.h"
#include "player.h"
/* GCD.bcdf2 is read through two names (k via the array, the talk test via the scalar) to keep both loads. */
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012F3B8[];
extern unsigned char GVARS_USE_ITEM_PENDING;
extern unsigned char D_8009CDF2a[];
extern unsigned char D_8009D143;
extern int showMessageBoxDirect(int, int, V6 *);
extern void FUN_80026e0c(int, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void AnimJump(TObj *, int);

#define NEXT(o) (*(short *)((char *)(o) + 0xc2))

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimJump(o, 0);
}

void lostHatManTalk(TObj *o)
{
    V6 v;
    TObj *p;
    int m, n;
    unsigned char k;

    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->animFrame = o->b68 & 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        o->state++;
        break;
    case 1:
        GVARS.b3e = 1;
        o->state = 10;
        break;
    case 10:
        k = D_8009CDF2a[0];
        if (k == 0xff) break;
        {
        int r = 0; int t;
        if (GVARS_USE_ITEM_PENDING == 1) r = GVARS.useItemId == 0x9f;
        t = 0;
        if (r) t = GCD.bcdf2 != 0xff;
        if (t) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 3, &v);
            FUN_80026e0c(0x9f, 1);
            GVARS_USE_ITEM_PENDING = 0;
            o->state = 0x5a;
            NEXT(o) = 0x46;
            break;
        }
        }
        if (k == 0) {
            v = *(V6 *)&o->a;
            o->d90 = showMessageBoxDirect(4, 0, &v);
            o->state = 0x5a;
            NEXT(o) = 0x32;
            break;
        }
        if (o->b68 && D_8009D143) {
            v = *(V6 *)&o->a;
            m = 4;
            n = 2;
        } else {
            v = *(V6 *)&o->a;
            m = 4;
            n = 1;
        }
        o->d90 = showMessageBoxDirect(m, n, &v);
        o->state = 0x5a;
        NEXT(o) = 0x7a;
        break;
    case 0x32:
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(4, 1, &v);
        o->state = 0x5a;
        NEXT(o) = 0x33;
        break;
    case 0x33:
        if (GCD.bcdf2 == 0) setEventStarted(0x4e, 0, 0);
        o->state = 0x78;
        NEXT(o) = 0xfb;
        break;
    case 0x46:
        o->animFrame = 1;
        o->velX = 0x600;
        o->state = 0x47;
        o->b69 = 0;
        o->timer = 200;
        setAnim(o, D_8012F3B8[o->subtype].anims[1]);
        setEventComplete(0x4e, 0);
        break;
    case 0x47:
        o->d->raw += o->velX << 8;
        if (--o->timer != 0) break;
        o->state = 0x78;
        NEXT(o) = 0xfa;
        break;
    case 0x49:
        break;
    case 0x5a:
        o->anim = D_8012F3B8[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state = 0x5b;
        break;
    case 0x5b:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_8012F3B8[o->subtype].anims[0];
        AnimJump(o, 0);
        o->state = NEXT(o);
        break;
    case 0x6e:
        o->timer = 200;
        o->state = 0x6f;
        break;
    case 0x6f:
        if (--o->timer == 0) goto next;
        break;
    case 0x64:
        GAME.fadeScreenControl = 4;
        o->state = NEXT(o);
        break;
    case 0x65:
        GAME.fadeScreenControl = 3;
        o->state = NEXT(o);
        break;
    case 0x66:
        GAME.fadeScreenControl = 4;
        o->state = 0x68;
        break;
    case 0x67:
        GAME.fadeScreenControl = 3;
        o->state = 0x68;
        break;
    case 0x68:
        if ((unsigned char)(GAME.fadeScreenControl - 3) < 2) break;
    next:
        o->state = NEXT(o);
        break;
    case 0x78:
        o->timer = 200;
        o->state = 0x79;
        break;
    case 0x79:
        if (--o->timer != 0) break;
        o->state = 0x7a;
        break;
    case 0x7a:
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        GVARS.b3e = 0;
        PLAYER.obj.wb2 = 0;
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->b68 = 0;
        switch (*(unsigned short *)((char *)o + 0xc2)) {
        case 0xf0:
            o->state = 0xf0;
            break;
        case 0xfa:
            o->state = 0xfa;
            break;
        case 0xfc:
            o->state = 0;
            o->step++;
            break;
        case 0xfd:
            o->b04 = 0;
            o->step = 0;
            o->state = 0;
            break;
        case 0xfe:
            o->step = 0;
            o->state = 0;
            o->b04++;
            break;
        default:
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    case 0xf0:
        o->anim = 0;
        break;
    case 0xfa:
        o->b04 = 2;
        break;
    case 0x48:
    default:
        break;
    }
}
