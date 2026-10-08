// FUNC 8002235c 724 MAIN0
// MATCHING 8002235c 724
#include "TOBJ.H"

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

extern PolyFT4 *DAT_1f800164;
extern long DAT_1f800070;
extern char *DAT_1f80008c;
extern unsigned long *DAT_1f8001e0;
extern unsigned long *D_1E0[]; /* same global as DAT_1f8001e0 (second name keeps the load after the stores) */

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

char *func_8002235C(TObj *o, long *f, char *vtx, unsigned long *ot)
{
    long otz;
    int n;
    PolyFT4 *p;
    unsigned long len;
    unsigned long t;

    n = *f++;
    if (n != 0) {
        len = 0x9000000;
        do {
            gte_ldv3c(vtx + 0x8);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    p = DAT_1f800164;
                    gte_stsxy3_ft4(p);
                    gte_ldv0(vtx + 0x20);
                    gte_rtps();
                    gte_stflg(&DAT_1f800070);
                    if (DAT_1f800070 >= 0) {
                        gte_stsxy2(&p->x3);
                        if ((p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100 || p->y3 < 0x100) &&
                            (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140 || p->x3 < 0x140)) {
                            gte_avsz4();
                            gte_stotz(&otz);
                            p->c0 = ((Face *)f)->c0;
                            p->clut = ((Face *)f)->clut;
                            p->tpage = ((Face *)f)->tpage;
                            if (o != 0 && (o->category & 0xf) == 8 && o->type == 1) {
                                p->uv0 = o->box0;
                                p->uv1 = o->box1;
                                p->uv2 = o->box2;
                                p->uv3 = o->box3;
                                otz = (long)D_1E0[0] + 0xc;
                            } else {
                                otz = (otz << 2) + (long)ot;
                                if ((unsigned long)(otz - (long)DAT_1f8001e0) >= 0xca0) goto next;
                                p->uv0 = ((Face *)f)->uv0;
                                p->uv1 = ((Face *)f)->uv1;
                                p->uv2 = ((Face *)f)->uv2;
                                p->uv3 = ((Face *)f)->uv3;
                            }
                            t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            DAT_1f800164 = DAT_1f800164 + 1;
                        }
                    }
                }
            }
        next:
            f += 10;
            vtx += 0x28;
        } while (--n != 0);
    }
    DAT_1f80008c = vtx;
    return (char *)f;
}
