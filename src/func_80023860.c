// FUNC 80023860 676 MAIN0
// MATCHING 80023860 676
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

extern PolyG4 *DAT_1f800164;

extern long DAT_1f800070;
extern char *DAT_1f80008c;
extern unsigned long *DAT_1f8001e0;

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
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

#define gte_stsxy3_g3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")

typedef struct { char pad[0xe]; unsigned char b0e; } O;
typedef struct { long c[4]; } C4;
extern C4 D_8007965C[];

char *func_80023860(O *o, long *f, char *vtx, unsigned long *ot)
{
    long otz;
    int n;
    PolyG4 *p;
    unsigned long len;
    unsigned long t;
    int i;

    n = *f++;
    if (n != 0) {
        len = 0x8000000;
        do {
            p = DAT_1f800164;
            gte_ldv3c(vtx + 0x10);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    gte_stsxy3_g3(p);
                    gte_ldv0(vtx + 0x28);
                    gte_rtps();
                    gte_stflg(&DAT_1f800070);
                    if (DAT_1f800070 >= 0) {
                        gte_stsxy(&p->x3);
                        if ((p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100 || p->y3 < 0x100) &&
                            (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140 || p->x3 < 0x140)) {
                            gte_avsz4();
                            gte_stotz(&otz);
                            otz = (otz << 2) + (long)ot;
                            if ((unsigned long)(otz - (long)DAT_1f8001e0) < 0xca0) {
                                t = *(unsigned long *)otz;
                                *(unsigned long *)otz = (unsigned long)p;
                                p->tag = t | len;
                                if (o == 0 || o->b0e == 0) {
                                    p->c0 = f[0];
                                    p->c1 = f[1];
                                    p->c2 = f[2];
                                    p->c3 = f[3];
                                } else {
                                    i = 0x12 - n;
                                    p->c0 = D_8007965C[i].c[0];
                                    p->c1 = D_8007965C[i].c[1];
                                    p->c2 = D_8007965C[i].c[2];
                                    p->c3 = D_8007965C[i].c[3];
                                }
                                DAT_1f800164 = DAT_1f800164 + 1;
                            }
                        }
                    }
                }
            }
            f += 21;
            vtx += 0x54;
        } while (--n != 0);
    }
    DAT_1f80008c = vtx;
    return (char *)f;
}
