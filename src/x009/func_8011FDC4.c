// FUNC 8011fdc4 540 X009
// MATCHING 8011fdc4 540

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
} PolyGT3;

typedef struct {
    unsigned short clut, tpage;
    unsigned long c0, c1, c2;
    unsigned short p10[3];
    unsigned short uv0;
    unsigned short p18[3];
    unsigned short uv1;
    unsigned short p20[3];
    unsigned short uv2;
} Face;

extern PolyGT3 *DAT_1f800164;
extern long DAT_1f800070;
extern unsigned long *DAT_1f8001e0;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 20( %0 );swc2 $14, 32( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

long *func_8011FDC4(long *f, unsigned long *ot)
{
    long otz;
    int n;
    PolyGT3 *p;
    unsigned long len;
    unsigned long t;

    n = *f++;
    if (n != 0) {
        len = 0x9000000;
        do {
            gte_ldv3c((char *)f + 0x10);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    gte_avsz3();
                    gte_stotz(&otz);
                    p = DAT_1f800164;
                    gte_stsxy3_gt3(p);
                    if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1) &&
                        (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141)) {
                        p->c0 = ((Face *)f)->c0;
                        p->c1 = ((Face *)f)->c1;
                        p->c2 = ((Face *)f)->c2;
                        p->clut = ((Face *)f)->clut;
                        p->tpage = ((Face *)f)->tpage;
                        p->uv0 = ((Face *)f)->uv0;
                        p->uv1 = ((Face *)f)->uv1;
                        *(unsigned short *)((char *)p + 0x24) = ((Face *)f)->uv2;
                        *((unsigned char *)p + 7) = 0x34;
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
        next:
            f += 10;
        } while (--n != 0);
    }
    return f;
}
