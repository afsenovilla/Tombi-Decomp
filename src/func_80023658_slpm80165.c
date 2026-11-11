// VERSION SLPM_80165 slpm80165 80023658 564
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPM_80165)
// Demo version of the JP func_800240CC (src/func_800240CC_slps01144.c): ordering table window 0x814 bytes (JP 0xCA0).
typedef struct {
    unsigned long tag;
    unsigned long c0;
    unsigned short x0, y0;
    unsigned long c1;
    unsigned short x1, y1;
    unsigned long c2;
    unsigned short x2, y2;
    unsigned long c3;
    unsigned short x3, y3;
} PolyG4;

typedef struct {
    unsigned long c0, c1, c2, c3;
    short v[12];
    short v3[4];
    long pad[9];
} Face;

extern PolyG4 *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern unsigned long *CURRENT_OT;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_ft4(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_ldrgb(r0) __asm__ volatile ("lwc2 $6, 0( %0 )" : : "r"(r0))
#define gte_ncs() __asm__ volatile ("nop;nop;.word 0x4b08041b")
#define gte_ncdt() __asm__ volatile ("nop;nop;.word 0x4b18043f")
#define gte_strgb2(r0) __asm__ volatile ("swc2 $22, 0( %0 )" : : "r"(r0) : "memory")
#define gte_strgb3(r0) __asm__ volatile ("swc2 $20, 4( %0 );swc2 $21, 12( %0 );swc2 $22, 20( %0 )" : : "r"(r0) : "memory")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

long *func_80023658(long *f, unsigned long *ot)
{
    long otz;
    int n;
    PolyG4 *p;
    unsigned long len;
    unsigned long t;
    unsigned long *cp;
    unsigned long *up;

    n = *f++;
    if (n != 0) {
        len = 0x8000000;
        cp = (unsigned long *)f;
        up = (unsigned long *)f + 3;
        do {
            p = NEXT_PRIM;
            gte_ldv3c((char *)f + 16);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    gte_stsxy3_ft4(p);
                    gte_ldv0((char *)f + 40);
                    gte_rtps();
                    gte_stflg(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL >= 0) {
                        gte_avsz4();
                        gte_stotz(&otz);
                        gte_stsxy((char *)p + 32);
                        if ((p->y0 < 0xf1 || p->y1 < 0xf1 || p->y2 < 0xf1 || p->y3 < 0xf1) &&
                            (p->x0 < 0x141 || p->x1 < 0x141 || p->x2 < 0x141 || p->x3 < 0x141)) {
                            *(unsigned long *)((char *)p + 4) = *cp;
                            *(unsigned long *)((char *)p + 12) = *(unsigned long *)((char *)up - 8);
                            *(unsigned long *)((char *)p + 20) = *(unsigned long *)((char *)up - 4);
                            *(unsigned long *)((char *)p + 28) = *(unsigned long *)up;
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
            f += 21;
            up += 21;
            cp += 21;

        } while (--n != 0);
    }
    return f;
}
#endif
