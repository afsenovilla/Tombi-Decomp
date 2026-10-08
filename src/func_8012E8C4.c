// FUNC 8012e8c4 1180 X000
// MATCHING 8012e8c4 1180
#include "TOBJ.H"
extern void *D_80139154[][3];
extern unsigned short *D_801391E4[];
extern short D_80139134[];
extern short D_8013913C[][3];
extern int DAT_1f8002d4[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_8003e3cc(int, int, short *, int, int);
extern void AnimJump(TObj *, int);
extern int Rand(void);
extern void FUN_80119584(TObj *, short, short, short);
extern void playSFX(int);
extern void FUN_80020aec(int);
extern void AnimAdvance(TObj *);
extern void FUN_80018790(TObj *);

#define U(x) (*(unsigned short *)&(x))
#define BOX(o) { b = D_801391E4[(o)->subtype] + U((o)->wb4) * 4; \
    (o)->box0 = *b++; (o)->box2 = *b++; (o)->box1 = *b; (o)->box3 = b[1]; }

void func_8012E8C4(TObj *o)
{
    short sv[6];
    unsigned short *b;
    TObj *q;
    int s;

    switch (o->b04) {
    case 0:
        o->w1e = 10;
        o->b0d = 0;
        U(o->wb4) = 0;
        U(o->wb6) = 0;
        U(o->wb8) = 0;
        o->anim = D_80139154[o->subtype][U(o->wb4)];
        o->d3c = DAT_1f8002d4[0];
        o->animFrame = 1;
        o->b0a = 0;
        *(signed char *)&o->b0f = -12;
        switch (o->subtype) {
        case 0:
        case 2:
        case 3:
            BOX(o);
            break;
        case 1:
            o->active = 2;
            o->box0 = 0;
            o->box1 = 0;
            o->box2 = 0;
            o->box3 = 0;
            break;
        }
        o->timer = 0;
        o->b04++;
        AnimLoadDuration(o);
        break;
    case 1:
        if (ObjCullRegister(o) == 0)
            break;
        if (o->b68) {
            if (o->b68 == 1) {
                U(o->wb4)++;
                if (U(o->wb4) >= D_80139134[o->subtype])
                    o->b0a = 0xff;
                q = (TObj *)o->d94;
                if (q != 0 && U(o->wb4) - U(q->wb4) >= 2)
                    q->b68 = 1;
                if (o->subtype == 0 && U(o->wb4) == 2) {
                    sv[1] = o->h->p.whole - 0x28;
                    sv[3] = o->y.p.whole - 0x18;
                    sv[5] = 0;
                    FUN_8003e3cc(0, 0, sv, -0x100, -0x100);
                }
                o->anim = D_80139154[o->subtype][U(o->wb4)];
                AnimJump(o, 0);
                BOX(o);
                U(o->wb6) = (Rand() & 1) + 3;
                U(o->wb8) = (Rand() & 7) + 5;
                o->timer = 0x1e;
            }
            o->b68 = 0;
        }
        if (U(o->wb8) != 0) {
            if (--U(o->wb6) == 0) {
                s = (Rand() & 0x1f) + D_8013913C[o->subtype][U(o->wb4)];
                FUN_80119584(o, o->a.p.whole - s, o->y.p.whole - (Rand() & 0x3f), o->b.p.whole);
                if ((U(o->wb8) & 3) == 0)
                    playSFX(0x33);
                U(o->wb6) = (Rand() & 1) + 1;
                U(o->wb8)--;
            }
        }
        if (o->timer != 0) {
            if (--o->timer == 0) {
                if (U(o->wb4) >= D_80139134[o->subtype]) {
                    FUN_80020aec(o->b6b);
                    o->b04++;
                } else {
                    o->active = 1;
                }
            }
        }
        AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
