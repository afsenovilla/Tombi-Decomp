// FUNC 800242b8 1976 MAIN0
/* score 116 (ncheck and matchcheck): only register allocation of the FIRST loop in each branch
   (litF3/drawF3) differs: game n=$9 vtx=$5 len=$6 otz=$7 p=$8, ours n=$7 vtx=$8 len=$9 p=$6 otz=$5.
   Rest matches. Tried: param order, local decl order/types, register kw, vtx copies, len as param,
   function-scope vars instead of inline (worse), scalar vs [0] for D_1F800164, lit types. */
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
    unsigned long c1;
    unsigned short x1, y1;
    unsigned long c2;
    unsigned short x2, y2;
} PolyG3;

extern char *D_1F800164[];
extern long DAT_1f800070;
extern char *DAT_1f80008c;
extern char *D_1F80008C[];
extern char *volatile D_1F80008CV;
extern unsigned long *DAT_1f8001e0;
extern char *func_8002216C(char *f, char *vtx, unsigned long *ot);
extern char *func_8002235C(void *o, char *f, char *vtx, unsigned long *ot);
extern char *func_80022630(char *f, char *vtx, unsigned long *ot);
extern char *func_80022838(char *f, char *vtx, unsigned long *ot);
extern char *func_80022C80(char *f, char *vtx, unsigned long *ot);
extern char *func_80023084(char *f, char *vtx, unsigned long *ot);
extern char *func_8002348C(char *f, char *vtx, unsigned long *ot);
extern char *func_80023860(void *o, char *f, char *vtx, unsigned long *ot);

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_f3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 12( %0 );swc2 $14, 16( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_g3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")
#define gte_ldrgb(r0) __asm__ volatile ("lwc2 $6, 0( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_nccs() __asm__ volatile ("nop;nop;.word 0x4b08041b")
#define gte_ncct() __asm__ volatile ("nop;nop;.word 0x4b18043f")
#define gte_strgb(r0) __asm__ volatile ("swc2 $22, 0( %0 )" : : "r"(r0) : "memory")
#define gte_strgb3_g3(r0) __asm__ volatile ("swc2 $20, 4( %0 );swc2 $21, 12( %0 );swc2 $22, 20( %0 )" : : "r"(r0) : "memory")

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
    PolyG3 *p;
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
                    gte_stsxy3_g3(p);
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
                            p->c1 = f[1];
                            p->c2 = f[2];
                            D_1F800164[0] = D_1F800164[0] + 0x1c;
                        }
                    }
                }
            }
            f += 16;
            vtx += 0x40;
        } while (--n != 0);
    }
    D_1F80008CV = vtx;
    return (char *)f;
}

static __inline__ char *litF3(long *f, char *vtx, unsigned long *ot)
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

static __inline__ char *litG3(long *f, char *vtx, unsigned long *ot)
{
    long otz;
    int n;
    PolyG3 *p;
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
                    gte_stsxy3_g3(p);
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
    D_1F80008CV = vtx;
    return (char *)f;
}

void func_800242B8(char *f, unsigned long *ot, void *o, char *x, unsigned char lit)
{
    char **v = D_1F80008C;

    f = func_8002216C(f, x, ot);
    f = func_8002235C(o, f, *v, ot);
    f = func_80022630(f, *v, ot);
    f = func_80022838(f, *v, ot);
    if (lit) {
        char **w;
        f = litF3((long *)f, *v, ot);
        w = D_1F80008C;
        f = func_80022C80(f, *w, ot);
        f = litG3((long *)f, *w, ot);
        func_80023084(f, D_1F80008CV, ot);
    } else {
        char **w;
        f = drawF3((long *)f, *v, ot);
        w = D_1F80008C;
        f = func_8002348C(f, *w, ot);
        f = drawFT3((long *)f, *w, ot);
        func_80023860(o, f, D_1F80008CV, ot);
    }
}
