// FUNC 8012d9fc 768 X000
// MATCHING 8012d9fc 768
#include "TOBJ.H"
extern char D_80077CDC[];
extern void *D_8013B26C;
extern unsigned char D_80139124[];
extern unsigned short D_1F8001F8;
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001fab4(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern void SfxPlay2(int, int);

void func_8012D9FC(TObj *o)
{
    short t;
    switch (o->state) {
    case 0:
        o->movetab = D_80077CDC;
        o->animFrame = Rand() & 1;
        o->anim = D_8013B26C;
        AnimLoadDuration(o);
        switch (D_80139124[Rand() & 0xf]) {
        case 0:
            o->timer = 60;
            o->wac = 0;
            o->state = 1;
            break;
        case 1:
            o->wac = 1;
            o->timer = 0x5a;
            o->state = 2;
            break;
        case 2:
            o->wac = 2;
            o->velY = -0x200;
            o->timer = 0;
            o->state = 4;
            break;
        }
        break;
    case 1:
        if (--o->timer == 0)
            o->state++;
        AnimAdvance(o);
        break;
    case 2:
        o->anim = D_8013B26C;
        AnimLoadDuration(o);
        o->timer = 0x60;
        o->state++;
    case 3:
        AnimAdvance(o);
        FUN_8001fab4(o);
        if (o->velH + 8 < o->h->p.whole)
            o->h->p.whole = o->velH + 8;
        if (o->h->p.whole < o->velH - 8)
            o->h->p.whole = o->velH - 8;
        (t = o->y.p.whole + 0x10, TileCollideAt(o, o->h->p.whole, t - o->b0c * 16));
        if ((D_1F8001F8 & 0xf) == 0 && (Rand() & 3) == 0)
            SfxPlay2(0x3a, 0);
        if (--o->timer == 0)
            o->state = 0;
        break;
    case 4:
        AnimAdvance(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0)
            o->state = 5;
        break;
    case 5:
        AnimAdvance(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if ((t = o->y.p.whole + 0x10, TileCollideAt(o, o->h->p.whole, t - o->b0c * 16)))
            o->state = 0;
        break;
    case 6:
        *(signed char *)&o->b0f = -7;
        AnimAdvance(o);
        break;
    }
}
