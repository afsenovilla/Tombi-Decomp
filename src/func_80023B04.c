// FUNC 80023b04 1972 MAIN0
// MATCHING 80023b04 1972
typedef struct {
    unsigned long tag;
    unsigned long c0;
    unsigned short x0, y0;
    unsigned short x1, y1;
    unsigned short x2, y2;
} PolyF3;
typedef struct {
    unsigned long tag;
    unsigned long c0;
    unsigned short x0, y0;
    unsigned long w0c;
    unsigned short x1, y1;
    unsigned long w14;
    unsigned short x2, y2;
} PolyFT3;

extern char *D_1F800164[];
extern long DAT_1f800070;
extern char *DAT_1f80008c;
extern unsigned long *DAT_1f8001e0;
extern char *func_8002216C(char *f, char *vtx, unsigned long *ot);
extern char *func_8002235C(void *o, char *f, char *vtx, unsigned long *ot);
extern char *func_80022630(char *f, char *vtx, unsigned long *ot);
extern char *func_80022838(char *f, char *vtx, unsigned long *ot);
extern char *func_8002348C(char *f, char *vtx, unsigned long *ot);
extern char *func_80023860(void *o, char *f, char *vtx, unsigned long *ot);
extern char *func_80022C80(char *f, char *vtx, unsigned long *ot);
extern char *func_80023084(char *f, char *vtx, unsigned long *ot);

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_f3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 12( %0 );swc2 $14, 16( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_ft3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_ldrgb(r0) __asm__ volatile ("lwc2 $6, 0( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_nccs() __asm__ volatile ("nop;nop;.word 0x4b08041b")
#define gte_strgb(r0) __asm__ volatile ("swc2 $22, 0( %0 )" : : "r"(r0) : "memory")
#define gte_ncct() __asm__ volatile ("nop;nop;.word 0x4b18043f")
#define gte_strgb3_g3(r0) __asm__ volatile ("swc2 $20, 4( %0 );swc2 $21, 12( %0 );swc2 $22, 20( %0 )" : : "r"(r0) : "memory")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")

static __inline__ char *drawF3(long *f, char *vtx, unsigned long *ot)
{
    long otz;
    int n;
    PolyF3 *p;
    unsigned long len;
    unsigned long t;

    n = *f++;
    if (n != 0) {
        len = 0x4000000;
        do {
            p = (void *)D_1F800164[0];
            gte_ldv3c(vtx + 4);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    gte_stsxy3_f3(p);
                    if ((p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100) &&
                        (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140)) {
                        gte_avsz3();
                        gte_stotz(&otz);
                        otz = (otz << 2) + (long)ot;
                        if ((unsigned long)(otz - (long)DAT_1f8001e0) < 0xca0) {
                            t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            p->c0 = f[0];
                            D_1F800164[0] = D_1F800164[0] + sizeof(PolyF3);
                        }
                    }
                }
            }
            f += 10;
            vtx += 0x28;
        } while (--n != 0);
    }
    DAT_1f80008c = vtx;
    return (char *)f;
}

static __inline__ char *drawFT3(long *f, char *vtx, unsigned long *ot)
{
    long otz;
    int n;
    PolyFT3 *p;
    unsigned long len;
    unsigned long t;

    n = *f++;
    if (n != 0) {
        len = 0x6000000;
        do {
            p = (void *)D_1F800164[0];
            gte_ldv3c(vtx + 0xc);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    gte_stsxy3_ft3(p);
                    if ((p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100) &&
                        (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140)) {
                        gte_avsz3();
                        gte_stotz(&otz);
                        otz = (otz << 2) + (long)ot;
                        if ((unsigned long)(otz - (long)DAT_1f8001e0) < 0xca0) {
                            t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            p->c0 = f[0];
                            p->w0c = f[1];
                            p->w14 = f[2];
                            D_1F800164[0] = D_1F800164[0] + 0x1c;
                        }
                    }
                }
            }
            f += 16;
            vtx += 0x40;
        } while (--n != 0);
    }
    DAT_1f80008c = vtx;
    return (char *)f;
}

static __inline__ char *drawF3L(long *f, char *vtx, unsigned long *ot)
{
    long otz;
    int n;
    PolyF3 *p;
    unsigned long len;
    unsigned long t;

    n = *f++;
    if (n != 0) {
        len = 0x4000000;
        do {
            p = (void *)D_1F800164[0];
            gte_ldv3c(vtx + 4);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    gte_stsxy3_f3(p);
                    if ((p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100) &&
                        (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140)) {
                        gte_avsz3();
                        gte_stotz(&otz);
                        otz = (otz << 2) + (long)ot;
                        if ((unsigned long)(otz - (long)DAT_1f8001e0) < 0xca0) {
                            t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            gte_ldrgb((char *)f + 0x24);
                            gte_ldv0((char *)f + 0x1c);
                            gte_nccs();
                            gte_strgb(&p->c0);
                            D_1F800164[0] = D_1F800164[0] + sizeof(PolyF3);
                        }
                    }
                }
            }
            f += 10;
            vtx += 0x28;
        } while (--n != 0);
    }
    DAT_1f80008c = vtx;
    return (char *)f;
}

static __inline__ char *drawG3L(long *f, char *vtx, unsigned long *ot)
{
    long otz;
    int n;
    PolyFT3 *p;
    unsigned long len;
    unsigned long t;

    n = *f++;
    if (n != 0) {
        len = 0x6000000;
        do {
            gte_ldv3c(vtx + 0xc);
            gte_rtpt();
            gte_stflg(&DAT_1f800070);
            if (DAT_1f800070 >= 0) {
                gte_nclip();
                gte_stopz(&DAT_1f800070);
                if (DAT_1f800070 > 0) {
                    p = (void *)D_1F800164[0];
                    gte_stsxy3_ft3(p);
                    if ((p->y0 < 0x100 || p->y1 < 0x100 || p->y2 < 0x100) &&
                        (p->x0 < 0x140 || p->x1 < 0x140 || p->x2 < 0x140)) {
                        gte_avsz3();
                        gte_stotz(&otz);
                        otz = (otz << 2) + (long)ot;
                        if ((unsigned long)(otz - (long)DAT_1f8001e0) < 0xca0) {
                            t = *(unsigned long *)otz;
                            *(unsigned long *)otz = (unsigned long)p;
                            p->tag = t | len;
                            gte_ldrgb((char *)f + 0x3c);
                            gte_ldv3c((char *)f + 0x24);
                            gte_ncct();
                            gte_strgb3_g3(p);
                            D_1F800164[0] = D_1F800164[0] + 0x1c;
                        }
                    }
                }
            }
            f += 16;
            vtx += 0x40;
        } while (--n != 0);
    }
    DAT_1f80008c = vtx;
    return (char *)f;
}

void func_80023B04(char *f, unsigned long *ot, void *o, unsigned char lit)
{
    f = func_8002216C(f, f + 4, ot);
    f = func_8002235C(o, f, f + 4, ot);
    f = func_80022630(f, f + 4, ot);
    f = func_80022838(f, f + 4, ot);
    if (lit) {
        f = drawF3L((long *)f, f + 4, ot);
        f = func_80022C80(f, f + 4, ot);
        f = drawG3L((long *)f, f + 4, ot);
        func_80023084(f, f + 4, ot);
    } else {
        f = drawF3((long *)f, f + 4, ot);
        f = func_8002348C(f, f + 4, ot);
        f = drawFT3((long *)f, f + 4, ot);
        func_80023860(o, f, f + 4, ot);
    }
}
