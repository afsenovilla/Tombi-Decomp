// VERSION SLPS_01144 x09 8011F1E0 656
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)

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
    unsigned long c0;
    unsigned long w1;
    unsigned long c1;
    unsigned long w3;
    unsigned long c2;
    unsigned short uv2, q5;
    unsigned long c3;
    unsigned short uv3, q7;
} Face;


extern PolyGT4 *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern unsigned long *CURRENT_OT;

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

long *func_8011F1E0_slps01144(long *f, unsigned long *ot)
{
    long otz;
    int n;
    PolyGT4 *p;
    unsigned long len;
    unsigned long t;
    long *h, *k;

    n = *f++;
    if (n != 0) {
        len = 0xc000000;
        h = f;
        k = f + 7;
        do {
            gte_ldv3c((char *)f + 0x20);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    p = NEXT_PRIM;
                    gte_stsxy3_gt4(p);
                    gte_ldv0((char *)f + 0x38);
                    gte_rtps();
                    gte_stflg(&LZ_FILE_CTRL);
                    if ((LZ_FILE_CTRL & 0x7f810000) == 0) {
                        gte_avsz4();
                        gte_stotz(&otz);
                        gte_stsxy(&p->x3);
                        if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1 || p->y3 < 0xf1) &&
                            (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141 || p->x3 < 0x141)) {
                            p->c0 = *h;
                            p->c1 = k[-5];
                            p->c2 = k[-3];
                            p->c3 = k[-1];
                            *(unsigned long *)((char *)p + 12) = k[-6];
                            *(unsigned long *)((char *)p + 24) = k[-4];
                            *(unsigned short *)((char *)p + 36) = *(unsigned short *)&k[-2];
                            *(unsigned short *)((char *)p + 48) = *(unsigned short *)&k[0];
                            *((unsigned char *)p + 7) = 0x3c;
                            otz = (otz << 2) + (long)ot;
                            if ((unsigned long)(otz - (long)CURRENT_OT) < 4)
                                otz = (long)CURRENT_OT + 16;
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
            f += 16;
            k += 16;
            h += 16;
        } while (--n != 0);
    }
    return f;
}
#endif
