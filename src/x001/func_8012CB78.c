// FUNC 8012cb78 2356 X001
// MATCHING 8012cb78 2356
#include "TOBJ.H"

typedef struct { short w0; unsigned short w2; short w4; unsigned short w6; } AH;
typedef struct {
    short a;     /* 0xb4 */
    short b;     /* 0xb6 */
    short wb8;   /* 0xb8 */
    char p[0xc4 - 0xba];
    short c4;    /* 0xc4 */
} Sub;

extern AH *D_8013DB00[], *D_8013DB04[], *D_8013DB08[], *D_8013DB0C[];
extern unsigned char D_8013C800[];
extern unsigned short D_8013C8E8[];
extern char D_80077CF4[];
extern short D_1F80027E, D_1F800284;
extern unsigned char D_800A603E, D_800A60E4;
extern void FUN_80026bfc(int, int);
extern short func_8004065C(TObj *, short, short, short);
extern TObj *FUN_80018448(void);
extern short FUN_8001fddc(short, int);
extern short FUN_8001fdac(short, int);
extern short TileCollideAt(TObj *o, short x, short y);
extern void FUN_800eae0c(short, int, int, int);
extern void FUN_8001fb20(TObj *);

static __inline__ int land2(TObj *o)
{
    short t;
    short w;

    if (o->b69 == 1) {
        o->d8c = 0;
        o->wb2 = 0;
        o->b69 = 0;
        o->wae = -1;
        o->b9c = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        t = D_1F80027E;
        if (t < 0) t = -t;
        if (t >= 9) t = 8;
        if (D_1F80027E < 0) t = -t;
        o->d8c = -t & 0xff;
        o->wb2 = t;
        w = D_1F800284;
        o->wb6 = (-t << 2) & 0xff;
        o->b9c = 0;
        o->wae = w;
        return 1;
    }
    return 0;
}

void func_8012CB78(TObj *o)
{
    Sub *s = (Sub *)&o->wb4;
    unsigned char *p0, *p1, *p2, *p3;
    TObj *n;
    short c, d;
    unsigned short k;
    unsigned char st;

    switch (o->state) {
    case 0:
        o->b9d = 0;
        FUN_80026bfc(0, 6);
        o->timer = 0x1e;
        *(signed char *)&o->b0f = -7;
        o->b68 = 0;
        o->wac = 7;
        o->state++;
        o->anim = D_8013DB04[0];
        p0 = &D_8013C800[((AH *)o->anim)->w2 * 4];
        o->box0 = *p0++;
        o->box1 = *p0++;
        o->box2 = *p0++;
        o->box3 = *p0;
        o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
        func_8004065C(o, o->h->p.whole + 0x10, o->y.p.whole + 8, 0);
        func_8004065C(o, o->h->p.whole - 0x10, o->y.p.whole + 8, 1);
        n = FUN_80018448();
        if (n) {
            n->active = 1;
            n->type = 0x10;
            {
                int e = (o->animFrame & 1) ? 0x10 : -0x10;
                n->a.p.whole = o->a.p.whole + e;
            }
            n->y.p.whole = o->y.p.whole;
            n->b.p.whole = o->b.p.whole;
            n->animFrame = o->animFrame & 1;
        }
        break;
    case 1:
        D_800A603E = 2;
        st = o->state;
        o->state = st + 1;
        if (o->velV == 0) {
            o->state = st + 2;
            break;
        }
    case 2:
        c = FUN_8001fddc(s->b, 0x250);
        d = FUN_8001fdac(s->b, 0x250);
        if (s->c4) o->h->raw -= c << 8;
        else o->h->raw += c << 8;
        o->y.raw += d << 8;
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        func_8004065C(o, o->h->p.whole + 0x10, o->y.p.whole + 8, 0);
        func_8004065C(o, o->h->p.whole - 0x10, o->y.p.whole + 8, 1);
        if (land2(o)) {
            o->velV = 0;
            o->d8c = (unsigned short)s->b;
            if (o->visible) {
                if (o->animFrame) s->wb8 = -0x10;
                else s->wb8 = 0x10;
                FUN_800eae0c(o->a.p.whole + s->wb8, o->y.p.whole, o->b.p.whole, 1);
            }
            o->state = 3;
            o->velH = 0x200;
            o->timer = 0x1e;
        }
        break;
    case 3:
        if (--o->timer == -1) {
            if (o->visible) {
                if (o->animFrame) s->wb8 = -0x10;
                else s->wb8 = 0x10;
                FUN_800eae0c(o->a.p.whole + s->wb8, o->y.p.whole, o->b.p.whole, 1);
            }
            o->timer = 0x1e;
        }
        FUN_8001fb20(o);
        if (land2(o)) {
            o->d8c = (unsigned short)s->b;
            if (s->c4) c = D_8013C8E8[-o->wb2];
            else c = D_8013C8E8[o->wb2];
            o->velH += c;
            if (o->velH > 0x300) o->velH = 0x300;
        }
        if (s->c4) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        if (o->b9d & 2) {
            if (s->c4 == (o->b9d & 1)) o->velH = 0;
        } else {
            if (s->c4) s->wb8 = -0x10;
            else s->wb8 = 0x10;
            if (func_8004065C(o, o->h->p.whole + s->wb8, o->y.p.whole + 8, s->c4)) o->velH = 0;
        }
        if (o->velH < 0x150) {
            o->wac = 8;
            o->anim = D_8013DB08[0];
            p1 = &D_8013C800[((AH *)o->anim)->w2 * 4];
            o->box0 = *p1++;
            o->box1 = *p1++;
            o->box2 = *p1++;
            o->box3 = *p1;
            o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
            if (o->velH <= 0) {
                D_800A60E4 = 3;
                o->state = 6;
            }
        }
        break;
    case 6:
        o->movetab = D_80077CF4;
        o->wac = 6;
        o->anim = D_8013DB00[0];
        p2 = &D_8013C800[((AH *)o->anim)->w2 * 4];
        o->box0 = *p2++;
        o->box1 = *p2++;
        o->box2 = *p2++;
        o->box3 = *p2;
        o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
        FUN_8001fb20(o);
        land2(o);
        break;
    case 8:
        break;
    case 7:
        o->wac = 9;
        o->anim = D_8013DB0C[0];
        p3 = &D_8013C800[((AH *)o->anim)->w2 * 4];
        o->box0 = *p3++;
        o->box1 = *p3++;
        o->box2 = *p3++;
        o->box3 = *p3;
        o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
        o->state++;
        break;
    }
}
