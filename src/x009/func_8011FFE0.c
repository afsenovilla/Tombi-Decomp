// FUNC 8011ffe0 672 X009
// MATCHING 8011ffe0 672

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
    unsigned short uv2, pad1;
    unsigned long c3;
    unsigned short x3, y3;
    unsigned short uv3, pad2;
} PolyGT4;

typedef struct {
    unsigned short clut, tpage;
    unsigned long c0, c1, c2, c3;
    unsigned short p14[3];
    unsigned short uv0;
    unsigned short p1c[3];
    unsigned short uv1;
    unsigned short p24[3];
    unsigned short uv2;
    unsigned short p2c[3];
    unsigned short uv3;
} Face;

extern PolyGT4 *DAT_1f800164;
extern long DAT_1f800070;
extern unsigned long *DAT_1f8001e0;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_gt4(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 20( %0 );swc2 $14, 32( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

long *func_8011FFE0(long *f, unsigned long *ot)
{
    long otz;
    int n;
    PolyGT4 *p;
    unsigned long len;
    unsigned long t;

    n = *f++;
    if (n != 0) {
        len = 0xc000000;
        do {
            gte_ldv3c((char *)f + 0x14);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    p = DAT_1f800164;
                    gte_stsxy3_gt4(p);
                    gte_ldv0((char *)f + 0x2c);
                    gte_rtps();
                    gte_stflg(&DAT_1f800070);
                    if ((DAT_1f800070 & 0x7f810000) == 0) {
                        gte_avsz4();
                        gte_stotz(&otz);
                        gte_stsxy(&p->x3);
                        if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1 || p->y3 < 0xf1) &&
                            (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141 || p->x3 < 0x141)) {
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
                            *((unsigned char *)p + 7) = 0x3c;
                            otz = (otz << 2) + (long)ot;
                            if ((unsigned long)(otz - (long)DAT_1f8001e0) < 4)
                                otz = (long)DAT_1f8001e0 + 16;
                            else if ((unsigned long)(otz - (long)DAT_1f8001e0) >= 0xca0)
                                goto next;
                            t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            DAT_1f800164 = DAT_1f800164 + 1;
                        }
                    }
                }
            }
        next:
            f += 13;
        } while (--n != 0);
    }
    return f;
}
