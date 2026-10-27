// FUNC 80055618 1696 MAIN0
// MATCHING 80055618 1696
#include "spad.h"
/* Matching debt: the `__asm__ volatile("")` barrier after `p->u0 = u` in setuvwh (twice).
   Without it sched2 ties sb u0 against addu u+w (both priority 6) and schedule_select always
   prefers the store (potential_hazard of the memory unit beats an ALU insn), giving score 4.
   No C-level dependency can order a store before an addu that does not touch its registers,
   so the barrier (a full scheduling fence that emits nothing) pins sb u0 before the addu. */
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned char u0, v0; unsigned short clut;
    unsigned short x1, y1;
    unsigned char u1, v1; unsigned short tpage;
    unsigned short x2, y2;
    unsigned char u2, v2; unsigned short pad2;
    unsigned short x3, y3;
    unsigned char u3, v3; unsigned short pad3;
} PolyFT4;
typedef struct { short n, p2, x0, y0, x1, y1; char pad[8]; } ENT;
typedef struct { ENT e[7]; } ROW;
typedef struct { char pad[0x4c]; unsigned short w4c; } C4C;
extern PolyFT4 *NEXT_PRIM;
extern char *CURRENT_OT;
extern C4C *TASK_C;
extern unsigned long LZ_FILE_CTRL;
extern char VIEW_CAMERA[];
extern ROW LOAD_BUFFER[];
extern short **D_800a4468;
extern short *D_800b0bb0;
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetSemiTrans(void *, int);

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

#define WC6(o) (*(unsigned short *)((char *)(o) + 0xc6))

static __inline__ int proj(void)
{
    gte_ldv0(((short*)&SPAD.s60));
    gte_rtps();
    gte_stflg(&SPAD.i8c);
    if (SPAD.i8c < 0) return 1;
    gte_stsxy(&LZ_FILE_CTRL);
    gte_stszotz(&SPAD.i74);
    return 0;
}

static __inline__ int otadd(unsigned long *a, char *b, int c, int d, unsigned long e)
{
    d = ((signed char)d + c) << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - (int)CURRENT_OT) >= 0xca0) return 1;
    {
        unsigned long v = *(unsigned long *)d;
        *(unsigned long *)d = (unsigned long)a;
        *a = v | e;
    }
    return 0;
}

void drawObjectMultiPartSprite3D(TObj *o)
{
    PolyFT4 *p;
    unsigned char *q;
    int i;
    short *e;
    int d;
    ROW *tbl;
    short x;
    int y;
    unsigned long xy;
    int u;
    int v;
    unsigned char w;
    int h;

    ((short*)&SPAD.s60)[0] = o->a.p.whole;
    SPAD.s62 = o->y.p.whole;
    if ((*(int*)&GAME.selectedArea) == 0x10005 && TASK_C->w4c != 3)
        SPAD.s64 = o->b.p.whole >> 2;
    else
        SPAD.s64 = o->b.p.whole;
    SetRotMatrix(((char*)&SPAD.bc0));
    SetTransMatrix(VIEW_CAMERA);
    if (proj()) return;
    xy = LZ_FILE_CTRL;
    x = xy;
    y = xy >> 16;
    tbl = LOAD_BUFFER;
    for (i = 0; i < 6; i++) {
        if ((&tbl[WC6(o)].e[i])->n > 0) {
            d = o->d3c;
            d += *D_800a4468[((unsigned short *)&o->wb4)[i]] * 4;
            p = NEXT_PRIM;
            q = (unsigned char *)(o->d3c + ((short *)d)[1]);
            p->code = 0x2d;
            SetSemiTrans(p, 0);
            u = q[0];
            v = q[1];
            w = q[10];
            h = q[11];
            p->u0 = u;
            __asm__ volatile("");
            p->u2 = u;
            p->v0 = v;
            p->u1 = u + w;
            p->v1 = v;
            p->v2 = v + h;
            p->u3 = u + w;
            p->v3 = v + h;
            p->x0 = x + (&tbl[WC6(o)].e[i])->x0 / 100;
            p->y0 = y + (&tbl[WC6(o)].e[i])->y0 / 100;
            p->x1 = x + (&tbl[WC6(o)].e[i])->x1 / 100;
            p->y1 = p->y0;
            p->x2 = p->x0;
            p->y2 = y + (&tbl[WC6(o)].e[i])->y1 / 100;
            p->x3 = p->x1;
            p->y3 = p->y2;
            p->tpage = o->w1e;
            p->clut = *(unsigned short *)(q + 2);
            if (!otadd((unsigned long *)p, CURRENT_OT, 0, o->b0f, 0x9000000))
                NEXT_PRIM = (PolyFT4 *)((char *)NEXT_PRIM + 0x28);
        }
    }
    d = o->d3c;
    d += *D_800b0bb0 * 4;
    p = NEXT_PRIM;
    q = (unsigned char *)(o->d3c + ((short *)d)[1]);
    p->code = 0x2d;
    SetSemiTrans(p, 0);
    u = q[0];
    v = q[1];
    w = q[10];
    h = q[11];
    p->u0 = u;
    __asm__ volatile("");
    p->u2 = u;
    p->v0 = v;
    p->u1 = u + w;
    p->v1 = v;
    p->v2 = v + h;
    p->u3 = u + w;
    p->v3 = v + h;
    p->x0 = x + LOAD_BUFFER[WC6(o)].e[6].x0 / 100;
    p->y0 = y + LOAD_BUFFER[WC6(o)].e[6].y0 / 100;
    p->x1 = x + LOAD_BUFFER[WC6(o)].e[6].x1 / 100;
    p->y1 = p->y0;
    p->x2 = p->x0;
    p->y2 = y + LOAD_BUFFER[WC6(o)].e[6].y1 / 100;
    p->x3 = p->x1;
    p->y3 = p->y2;
    p->tpage = o->w1e;
    p->clut = *(unsigned short *)(q + 2);
    if (!otadd((unsigned long *)p, CURRENT_OT, 0, o->b0f, 0x9000000))
        NEXT_PRIM = (PolyFT4 *)((char *)NEXT_PRIM + 0x28);
}
