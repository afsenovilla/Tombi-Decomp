// VERSION SLPS_01144 x09 8011EAD4 564
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned long a1;
    unsigned short x1, y1;
    unsigned long a2;
    unsigned short x2, y2;
    unsigned short h, pad;
} PolyF4;

extern PolyF4 *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern unsigned long *CURRENT_OT;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_f3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_ldrgb(r0) __asm__ volatile ("lwc2 $6, 0( %0 )" : : "r"(r0))
#define gte_nccs() __asm__ volatile ("nop;nop;.word 0x4b08041b")
#define gte_strgb(r0) __asm__ volatile ("swc2 $22, 0( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")


char *func_8011EAD4_slps01144(long *f, unsigned long *ot)
{
    long otz;
    int n;
    PolyF4 *p;
    unsigned char *w;
    long *k;
    unsigned long cc;
    int t;
    unsigned short h;

    n = *f++;
    if (n != 0) {
        cc = 0x7000000;
        w = (unsigned char *)f;
        k = f + 3;
        do {
            gte_ldv3c((char *)f + 16);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    gte_avsz3();
                    gte_stotz(&otz);
                    p = NEXT_PRIM;
                    gte_stsxy3_f3(p);
                    if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1) &&
                        (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141)) {
                        *(long *)&p->r0 = *(long *)w;
                        p->a1 = k[-2];
                        p->a2 = k[-1];
                        h = *(unsigned short *)k;
                        p->code = 0x24;
                        p->h = h;
                        p->r0 = (otz >> 2) < p->r0 ? p->r0 - (otz >> 2) : 8;
                        p->g0 = (otz >> 2) < p->g0 ? p->g0 - (otz >> 2) : 8;
                        p->b0 = (otz >> 2) < p->b0 ? p->b0 - (otz >> 2) : 8;
                        otz = (otz << 2) + (long)ot;
                        if ((unsigned long)(otz - (long)CURRENT_OT) < 0xca0) {
                            unsigned long tt = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = tt | cc;
                            NEXT_PRIM = NEXT_PRIM + 1;
                        }
                    }
                }
            }
            f += 10;
            k += 10;
            w += 0x28;
        } while (--n != 0);
    }
    return (char *)f;
}
#endif
