// VERSION SLPM_80165 slpm80165 80023B08 740
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of func_8002457C_slps01144 (src/func_8002457C_slps01144.c): quad form of func_8002388C, the screen
// test after the colour and an OT window of 0x814 bytes (JP 0xCA0).
typedef struct {
    unsigned long tag;
    unsigned char r0, g0, b0, code;
    unsigned short x0, y0;
    unsigned short x1, y1;
    unsigned short x2, y2;
    unsigned short x3, y3;
} PolyF4;

extern PolyF4 *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern unsigned long *CURRENT_OT;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_f3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 12( %0 );swc2 $14, 16( %0 )" : : "r"(r0) : "memory")
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_ldrgb(r0) __asm__ volatile ("lwc2 $6, 0( %0 )" : : "r"(r0))
#define gte_nccs() __asm__ volatile ("nop;nop;.word 0x4b08041b")
#define gte_strgb(r0) __asm__ volatile ("swc2 $22, 0( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

typedef struct { char p[0xd]; unsigned char b13, b14; } Fade;

char *func_80023B08(Fade *c, long *f, unsigned long *ot)
{
    long otz;
    unsigned long col;
    int n;
    PolyF4 *p;
    unsigned long len;
    int d; unsigned char u; unsigned long cc;
    unsigned char *w, *k;

    n = *f++;
    if (n != 0) {
        len = 0x5000000;
        w = (unsigned char *)f;
        cc = 0x28000000;
        k = (unsigned char *)f + 0x24;
        do {
            gte_ldv3c((char *)f + 4);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    p = NEXT_PRIM;
                    gte_stsxy3_f3(p);
                    gte_ldv0((char *)f + 0x1c);
                    gte_rtps();
                    gte_stflg(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL >= 0) {
                    gte_avsz4();
                    gte_stotz(&otz);
                    gte_stsxy(&p->x3);
                    {
                        col = *(unsigned long *)(k + 8);
                        if (c->b14 != 0) {
                            col = cc;
                            d = k[10] - c->b13;
                            u = d;
                            if (d < 0) u = 0;
                            col = ((u & 0xff) << 16) | cc;
                            d = k[9] - c->b13;
                            u = d;
                            if (d < 0) u = 0;
                            col |= (u & 0xff) << 8;
                            d = k[8] - c->b13;
                            u = d;
                            if (d < 0) u = 0;
                            col |= u & 0xff;
                        }
                        gte_ldrgb(&col);
                        gte_ldv0(k);
                        gte_nccs();
                        gte_strgb(&p->r0);
                        *(long *)w = *(long *)&p->r0;
                        w[3] = 41;
                        if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1 || p->y3 < 0xf1) &&
                            (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141 || p->x3 < 0x141)) {
                        otz = (otz << 2) + (long)ot;
                        if ((unsigned long)(otz - (long)CURRENT_OT) < 0x814) {
                            unsigned long t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            NEXT_PRIM = NEXT_PRIM + 1;
                        }
                        }
                    }
                    }
                }
            }
            k += 0x30;
            f += 12;
            w += 0x30;
        } while (--n != 0);
    }
    return (char *)f;
}
#endif
