// VERSION SLPS_01144 x09 8011ED08 716
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
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
    unsigned long c0, w1, w2;
    unsigned short uv2, p0e, uv3, p12;
} Face;

extern PolyFT4 *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern unsigned long *CURRENT_OT;

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

#define DIM(c) ((c) = (otz >> 2) < (c) ? (c) - (otz >> 2) : 8)

char *func_8011ED08_slps01144(long *f, unsigned long *ot)
{
    long otz;
    int n;
    PolyFT4 *p;
    unsigned long len;
    unsigned long t;

    long *q;
    long *r;

    n = *f++;
    if (n != 0) {
        len = 0x9000000;
        q = f;
        r = f + 4;
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
                    if (!(LZ_FILE_CTRL & 0x7f810000)) {
                        gte_avsz4();
                        gte_stotz(&otz);
                        gte_stsxy2(&p->x3);
                        if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1 || p->y3 < 0xf1) &&
                            (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141 || p->x3 < 0x141)) {
                            p->c0 = ((Face *)q)->c0;
                            *(unsigned long *)&p->uv0 = r[-3];
                            *(unsigned long *)&p->uv1 = r[-2];
                            p->uv2 = *(unsigned short *)&r[-1];
                            p->uv3 = *(unsigned short *)&r[0];
                            ((unsigned char *)p)[7] = 0x2c;
                            DIM(((unsigned char *)p)[4]);
                            DIM(((unsigned char *)p)[5]);
                            DIM(((unsigned char *)p)[6]);
                            otz = (otz << 2) + (long)ot;
                            if ((unsigned long)(otz - (long)CURRENT_OT) < 4)
                                otz = (long)CURRENT_OT + 0x10;
                            else if ((unsigned long)(otz - (long)CURRENT_OT) >= 0xca0)
                                goto next;
                            t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            NEXT_PRIM = NEXT_PRIM + 1;
                        }
                    }
                }
            }
        next:
            f += 13;
            r += 13;
            q += 13;
        } while (--n != 0);
    }
    return (char *)f;
}
#endif
