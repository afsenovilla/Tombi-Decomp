// FUNC 80022838 644 MAIN0
// MATCHING 80022838 644

typedef struct {
    unsigned long tag;
    unsigned long c0;
    unsigned short x0, y0;
    unsigned short uv0, clut;
    unsigned long c1;
    unsigned short x1, y1;
    unsigned short uv1, tpage;
    unsigned long c2;
    unsigned short x2, y2;
    unsigned short uv2, pad;
    unsigned long c3;
    unsigned short x3, y3;
    unsigned short uv3, pad3;
} PolyGT3;

typedef struct {
    unsigned short clut, tpage;
    unsigned long c0, c1, c2;
    unsigned long c3;
    unsigned short p14[3];
    unsigned short uv0;
    unsigned short p1c[3];
    unsigned short uv1;
    unsigned short p24[3];
    unsigned short uv2;
    unsigned short p2c[3];
    unsigned short uv3;
} Face;

extern PolyGT3 *DAT_1f800164;
extern long DAT_1f800070;
extern char *DAT_1f80008c;
extern unsigned long *DAT_1f8001e0;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 20( %0 );swc2 $14, 32( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stsxy2(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")

char *func_80022838(long *f, char *vtx, unsigned long *ot)
{
    long otz;
    int n;
    PolyGT3 *p;
    unsigned long len;
    unsigned long t;

    n = *f++;
    if (n != 0) {
        len = 0xc000000;
        do {
            gte_ldv3c(vtx + 0x14);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    p = DAT_1f800164;
                    gte_stsxy3_gt3(p);
                    gte_ldv0(vtx + 0x2c);
                    gte_rtps();
                    gte_stflg(&DAT_1f800070);
                    if (DAT_1f800070 >= 0) {
                        gte_stsxy2(&p->x3);
                        if ((p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100 || p->y3 < 0x100) &&
                            (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140 || p->x3 < 0x140)) {
                            gte_avsz4();
                            gte_stotz(&otz);
                            otz = (otz << 2) + (long)ot;
                            if ((unsigned long)(otz - (long)DAT_1f8001e0) < 0xca0) {
                                t = *(unsigned long *)otz;
                                *(unsigned long *)otz = (unsigned long)p;
                                p->tag = t | len;
                                p->c0 = ((Face *)f)->c0;
                                p->c1 = ((Face *)f)->c1;
                                p->c2 = ((Face *)f)->c2;
                                p->c3 = ((Face *)f)->c3;
                                p->clut = ((Face *)f)->clut;
                                p->tpage = ((Face *)f)->tpage;
                                p->uv0 = ((Face *)f)->uv0;
                                p->uv1 = ((Face *)f)->uv1;
                                p->uv2 = ((Face *)f)->uv2;
                                *(unsigned short *)((char *)p + 0x30) = ((Face *)f)->uv3;
                                DAT_1f800164 = DAT_1f800164 + 1;
                            }
                        }
                    }
                }
            }
            f += 13;
            vtx += 0x34;
        } while (--n != 0);
    }
    DAT_1f80008c = vtx;
    return (char *)f;
}
