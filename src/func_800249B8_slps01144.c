// VERSION SLPS_01144 slps01144 800249B8 448
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "spad.h"

typedef struct {
    unsigned long tag;
    unsigned long c0;
    unsigned short x0, y0;
    unsigned short uv0, clut;
    unsigned short x1, y1;
    unsigned short uv1, tpage;
    unsigned short x2, y2;
    unsigned short uv2, pad2;
    unsigned short x3, y3;
    unsigned short uv3, pad3;
} PolyFT4;

typedef struct {
    unsigned short clut, tpage;
    unsigned long c0;
    unsigned short p08[3];
    unsigned short uv0;
    unsigned short p10[3];
    unsigned short uv1;
    unsigned short p18[3];
    unsigned short uv2;
    unsigned short p20[3];
    unsigned short uv3;
} Face;

extern PolyFT4 *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern char *MODEL_VERTEX_PTR;
extern unsigned long *CURRENT_OT;
extern unsigned long *D_1E0[]; /* same global as CURRENT_OT (second name keeps the load after the stores) */

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_ft4(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stsxy2(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")

extern unsigned long *D_1F8001E0;
extern long *func_80023914(long *f, unsigned long *ot);

long *func_800249B8(long *f0, unsigned long *ot0, TObj *o0)
{
    long otz;
    long *pz;
    int n;
    register PolyFT4 *p asm("$7");
    unsigned long len;
    unsigned long t;
    unsigned long *cp;
    unsigned short *up;

    register long *f asm("$4") = f0;
    register unsigned long *ot asm("$5") = ot0;
    register TObj *o asm("$6") = o0;
    n = *++f;
    f++;
    if (n != 0) {
        len = 0x9000000;
        cp = (unsigned long *)f;
        pz = &otz;
        up = (unsigned short *)f + 5;
        do {
            gte_ldv3c((char *)f + 0x14);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    p = NEXT_PRIM;
                    gte_stsxy3_ft4(p);
                    gte_ldv0((char *)f + 0x2c);
                    gte_rtps();
                    gte_stflg(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL >= 0) {
                        gte_avsz4();
                        gte_stotz(pz);
                        gte_stsxy2(&p->x3);
                        p->c0 = *cp;
                        p->clut = up[-2];
                        p->tpage = up[0];
                        p->uv0 = o->box0;
                        p->uv1 = o->box1;
                        p->uv2 = o->box2;
                        p->uv3 = o->box3;
                        __asm__ volatile ("" : : : "memory");
                        otz = (long)D_1F8001E0 + 0xc;
                        t = *(unsigned long *)otz;
                        *(unsigned long *)otz = (unsigned long)p;
                        p->tag = t | len;
                        NEXT_PRIM = NEXT_PRIM + 1;
                    }
                }
            }
            f += 13;
            up += 26;
            cp += 13;
        } while (--n != 0);
    }
    return func_80023914(f + 5, ot);
}
#endif
