// VERSION SLPM_80165 slpm80165 8002388C 636
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of func_80024300_slps01144 (src/func_80024300_slps01144.c): the screen test comes after the colour
// is computed, and the OT window is 0x814 bytes (JP 0xCA0).
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned short x1, y1;
    unsigned short x2, y2;
} PolyF3;

extern PolyF3 *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern unsigned long *CURRENT_OT;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_f3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 12( %0 );swc2 $14, 16( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_ldrgb(r0) __asm__ volatile ("lwc2 $6, 0( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_nccs() __asm__ volatile ("nop;nop;.word 0x4b08041b")
#define gte_strgb(r0) __asm__ volatile ("swc2 $22, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

char *func_8002388C(unsigned char *ctx, char *f, unsigned long *ot)
{
    long otz;
    int n;
    PolyF3 *p;
    unsigned long len;
    unsigned long t;
    unsigned long rgb;
    unsigned char *c;
    int d; unsigned char r;
    unsigned long base;
    char *g;

    n = *(long *)f;
    f += 4;
    if (n != 0) {
        len = 0x4000000;
        g = f;
        base = 0x20000000;
        c = (unsigned char *)f + 28;
        do {
            gte_ldv3c(f + 4);
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
                    {
                        rgb = *(unsigned long *)(c + 8);
                        if (ctx[14]) {
                            rgb = base;
                            d = c[10] - ctx[13];
                            r = d;
                            if (d < 0) r = 0;
                            rgb = ((r & 0xff) << 16) | base;
                            d = c[9] - ctx[13];
                            r = d;
                            if (d < 0) r = 0;
                            rgb |= (r & 0xff) << 8;
                            d = c[8] - ctx[13];
                            r = d;
                            if (d < 0) r = 0;
                            rgb |= r & 0xff;
                        }
                        gte_ldrgb(&rgb);
                        gte_ldv0(c);
                        gte_nccs();
                        gte_strgb(&p->r0);
                        *(unsigned long *)g = *(unsigned long *)&p->r0;
                        g[3] = 0x21;
                        if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1) &&
                            (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141)) {
                        otz = (otz << 2) + (long)ot;
                        if ((unsigned long)(otz - (long)CURRENT_OT) < 0x814) {
                            t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            NEXT_PRIM = NEXT_PRIM + 1;
                        }
                        }
                    }
                }
            }
            c += 40;
            f += 40;
            g += 40;
        } while (--n != 0);
    }
    return f;
}
#endif
