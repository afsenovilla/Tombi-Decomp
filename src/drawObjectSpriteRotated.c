// FUNC 800532fc 2144 MAIN0
// MATCHING 800532fc 2144
// FLAGS -O2 -G0 -fno-strength-reduce
#include "spad.h"
/* Rotated FT4 sprite strip without scale (MAIN0 twin of drawObjectSpriteRotScaled / X000 drawObjectSpriteRotatedFT4): same macro loop body
   with A/B/C/D read straight from the frame (short C/D: D spills as sh/lhu) and f = NEXT_PRIM read before C/D so
   the lw s0 schedules above their lbu. */
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned short uv0, clut;
    unsigned short x1, y1;
    unsigned short uv1, tpage;
    unsigned short x2, y2;
    unsigned short uv2, pad2;
    unsigned short x3, y3;
    unsigned short uv3, pad3;
} PolyFT4;
typedef struct { char pad[0x4c]; unsigned short w4c; } C4C;
extern PolyFT4 *NEXT_PRIM;
extern char *CURRENT_OT;
extern C4C *TASK_C;
extern long LZ_FILE_CTRL;
extern int SPRITE_PART_COUNTER;
extern char VIEW_CAMERA[];
extern void SetRotMatrix(void *);
extern void SetTransMatrix(void *);
extern void SetSemiTrans(void *, int);
extern short COS_TABLE[];
extern short NEG_SIN_TABLE[];

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stszotz(r0) __asm__ volatile ("mfc2 $12, $19;nop;sra $12, $12, 2;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")

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

static __inline__ int onscreen(PolyFT4 *p)
{
    if (p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100 || p->y3 < 0x100) {
        if (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140 || p->x3 < 0x140)
            return 1;
    }
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

#define SN(i) COS_TABLE[i]
#define a1 ((ang + 0x80) & 0xff)
#define a0 (ang & 0xff)
#define CS(i) NEG_SIN_TABLE[i]

void drawObjectSpriteRotated(TObj *o)
{
    PolyFT4 *f;
    short *e;
    unsigned char *q;
    unsigned char *fp;
    int xy;
    short x;
    short y;
    int d;
    int ang;
    short A, B;
    short C;
    short D;
    short u, w;

    if (o->anim == 0) return;
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
    y = (unsigned)xy >> 16;
    d = o->d3c;
    d += *(unsigned short *)o->anim * 4;
    e = (short *)d;
    SPRITE_PART_COUNTER = e[0];
    q = (unsigned char *)(o->d3c + e[1]);
    fp = q + 0xc;
    ang = o->d8c;
    do {
        A = (signed char)fp[2];
        B = (signed char)fp[3];
        f = NEXT_PRIM;
        C = fp[-2];
        D = fp[-1];
        if (o->animFrame & 1) {
            u = A;
            w = B;
            f->x0 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y0 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x1 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y1 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x2 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y2 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x3 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y3 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        } else {
            u = -(A + C);
            w = B;
            f->x1 = x + ((-u * SN(a1)) >> 12) + ((-w * SN((a1 + 0xc0) & 0xff)) >> 12);
            f->y1 = y + ((-u * CS(a1)) >> 12) + ((-w * CS((a1 + 0xc0) & 0xff)) >> 12);
            f->x0 = x + (((u + C) * SN(a0)) >> 12) + ((-w * SN((a0 + 0x40) & 0xff)) >> 12);
            f->y0 = y + (((u + C) * CS(a0)) >> 12) + ((-w * CS((a0 + 0x40) & 0xff)) >> 12);
            f->x3 = x + ((-u * SN(a1)) >> 12) + (((w + D) * SN((a1 + 0x40) & 0xff)) >> 12);
            f->y3 = y + ((-u * CS(a1)) >> 12) + (((w + D) * CS((a1 + 0x40) & 0xff)) >> 12);
            f->x2 = x + (((u + C) * SN(a0)) >> 12) + (((w + D) * SN((a0 + 0xc0) & 0xff)) >> 12);
            f->y2 = y + (((u + C) * CS(a0)) >> 12) + (((w + D) * CS((a0 + 0xc0) & 0xff)) >> 12);
        }
        if (onscreen(f)) {
            f->code = 0x2d;
            SetSemiTrans(f, o->b0d >> 7);
            *(unsigned long *)&f->uv0 = *(unsigned long *)q;
            *(unsigned long *)&f->uv1 = *(unsigned long *)(fp - 8);
            f->uv2 = *(unsigned short *)(fp - 4);
            f->uv3 = *(unsigned short *)fp;
            f->tpage += o->w1e;
            if (o->b0d & 1)
                f->clut = o->w08;
            if (o->b0b) {
                if (otadd((unsigned long *)f, CURRENT_OT + 0x10, 0, o->b0f, 0x9000000)) goto next;
            } else {
                if (otadd((unsigned long *)f, CURRENT_OT + 0x10, SPAD.i74, o->b0f, 0x9000000)) goto next;
            }
            NEXT_PRIM = (PolyFT4 *)((char *)NEXT_PRIM + 0x28);
        }
    next:
        fp += 16;
        SPRITE_PART_COUNTER--;
        q += 16;
    } while (SPRITE_PART_COUNTER != 0);
}
