// VERSION SLPM_80165 slpm80165 8002243C 488
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of the JP func_80022F34 (src/func_80022F34_slps01144.c): ordering table window 0x814 bytes (JP 0xCA0).
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
    unsigned long c0;
    unsigned long uv0clut;
    unsigned long c1;
    unsigned long uv1tpage;
    unsigned long c2;
    unsigned short uv2, pad;
    short v[12];
} Face;

extern PolyGT3 *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern unsigned long *CURRENT_OT;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 20( %0 );swc2 $14, 32( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

long *func_8002243C(long *f, unsigned long *ot)
{
    long otz;
    int n;
    PolyGT3 *p;
    unsigned long len;
    unsigned long t;
    unsigned long *cp;
    unsigned short *up;

    n = *f++;
    if (n != 0) {
        len = 0x9000000;
        cp = (unsigned long *)f;
        up = (unsigned short *)f + 10;
        do {
            gte_ldv3c((char *)f + 24);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    gte_avsz3();
                    gte_stotz(&otz);
                    p = NEXT_PRIM;
                    gte_stsxy3_gt3(p);
                    if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1) &&
                        (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141)) {
                        *(unsigned long *)((char *)p + 4) = *cp;
                        *(unsigned long *)((char *)p + 16) = *(unsigned long *)((char *)up - 12);
                        *(unsigned long *)((char *)p + 28) = *(unsigned long *)((char *)up - 4);
                        *(unsigned long *)((char *)p + 12) = *(unsigned long *)((char *)up - 16);
                        *(unsigned long *)((char *)p + 24) = *(unsigned long *)((char *)up - 8);
                        *(unsigned short *)((char *)p + 36) = *up;
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
            f += 12;
            up += 24;
            cp += 12;

        } while (--n != 0);
    }
    return f;
}
#endif
