// JP-only mesh renderer (no PAL/USA twin): FT3/FT4/GT3/GT4 lists, then lit (NCCS/NCCT, ctx[14]==1 darkens the
// flat colour by 0x20) or unlit screen-clipped F3/F4/G3/G4 lists. Each list has its own block-scope prim pointer
// (retail gives it a different register per list); PTag makes the tag store a struct store so the NEXT_PRIM load
// schedules above it.
// VERSION SLPS_01144 slps01144 80024B78 5112
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)

typedef struct { unsigned long tag; } PTag;
extern char *NEXT_PRIM;
extern long LZ_FILE_CTRL;
extern unsigned long *CURRENT_OT;

#define gte_ldv3c(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 );lwc2 $2, 8( %0 );lwc2 $3, 12( %0 );lwc2 $4, 16( %0 );lwc2 $5, 20( %0 )" : : "r"(r0))
#define gte_rtpt() __asm__ volatile ("nop;nop;.word 0x4a280030")
#define gte_rtps() __asm__ volatile ("nop;nop;.word 0x4a180001")
#define gte_stflg(r0) __asm__ volatile ("cfc2 $12, $31;nop;sw $12, 0( %0 )" : : "r"(r0) : "$12", "memory")
#define gte_nclip() __asm__ volatile ("nop;nop;.word 0x4b400006")
#define gte_stopz(r0) __asm__ volatile ("swc2 $24, 0( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_f3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 12( %0 );swc2 $14, 16( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_g3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 16( %0 );swc2 $14, 24( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy3_gt3(r0) __asm__ volatile ("swc2 $12, 8( %0 );swc2 $13, 20( %0 );swc2 $14, 32( %0 )" : : "r"(r0) : "memory")
#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0( %0 )" : : "r"(r0) : "memory")
#define gte_avsz3() __asm__ volatile ("nop;nop;.word 0x4b58002d")
#define gte_avsz4() __asm__ volatile ("nop;nop;.word 0x4b68002e")
#define gte_stotz(r0) __asm__ volatile ("swc2 $7, 0( %0 )" : : "r"(r0) : "memory")
#define gte_ldrgb(r0) __asm__ volatile ("lwc2 $6, 0( %0 )" : : "r"(r0))
#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0( %0 );lwc2 $1, 4( %0 )" : : "r"(r0))
#define gte_nccs() __asm__ volatile ("nop;nop;.word 0x4b08041b")
#define gte_ncct() __asm__ volatile ("nop;nop;.word 0x4b18043f")
#define gte_strgb(r0) __asm__ volatile ("swc2 $22, 0( %0 )" : : "r"(r0) : "memory")
#define gte_strgb3_g3(r0) __asm__ volatile ("swc2 $20, 4( %0 );swc2 $21, 12( %0 );swc2 $22, 20( %0 )" : : "r"(r0) : "memory")

#define W(p, o) (*(unsigned long *)((char *)(p) + (o)))
#define H(p, o) (*(unsigned short *)((char *)(p) + (o)))

/* link p into the OT at the average z and advance the prim pointer by sz */
#define LINK(sz)                                                          \
    otz = (otz << 2) + (long)ot;                                          \
    k = otz - (long)CURRENT_OT;                                           \
    if (k < 0xca0) {                                                      \
        t = *(unsigned long *)otz | len;                                  \
        *(unsigned long *)otz = (unsigned long)p;                         \
        ((PTag *)p)->tag = t;                                             \
        NEXT_PRIM = NEXT_PRIM + (sz);                                     \
    }

#define CLIP3(a, b, c) \
    ((H(p, a + 2) < 0xf1 || H(p, b + 2) < 0xf1 || H(p, c + 2) < 0xf1) && \
     (H(p, a) < 0x141 || H(p, b) < 0x141 || H(p, c) < 0x141))
#define CLIP4(a, b, c, d) \
    ((H(p, a + 2) < 0xf1 || H(p, b + 2) < 0xf1 || H(p, c + 2) < 0xf1 || H(p, d + 2) < 0xf1) && \
     (H(p, a) < 0x141 || H(p, b) < 0x141 || H(p, c) < 0x141 || H(p, d) < 0x141))

#define DARK(c)                                                           \
    if (ctx[14] == 1) {                                                   \
        rgb = base;                                                       \
        d = (c)[2] - 0x20; r = d; if (d < 0) r = 0;                       \
        rgb = ((r & 0xff) << 16) | base;                                  \
        d = (c)[1] - 0x20; r = d; if (d < 0) r = 0;                       \
        rgb |= (r & 0xff) << 8;                                           \
        d = (c)[0] - 0x20; r = d; if (d < 0) r = 0;                       \
        rgb |= r & 0xff;                                                  \
    } else {                                                              \
        rgb = *(unsigned long *)(c);                                      \
    }

void func_80024B78(long *f, unsigned long *ot, unsigned char *ctx, char *vtx, unsigned char lit)
{
    long otz;
    unsigned long rgb;
    int n;
    long *g;
    unsigned long len;
    unsigned long t;
    unsigned long k;
    unsigned long base;
    int d; unsigned char r;

    /* FT3 */
    n = *f++;
    if (n != 0) {
        char *p;
        len = 0x7000000;
        g = f;
        do {
            p = NEXT_PRIM;
            gte_ldv3c(vtx);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    gte_avsz3();
                    gte_stotz(&otz);
                    gte_stsxy3_g3(p);
                    W(p, 4) = g[0];
                    W(p, 0xc) = g[1];
                    W(p, 0x14) = g[2];
                    H(p, 0x1c) = H(g, 0xc);
                    LINK(0x20)
                }
            }
            f += 10;
            g += 10;
            vtx += 0x20;
        } while (--n != 0);
    }
    /* FT4 */
    n = *f++;
    if (n != 0) {
        char *p;
        len = 0x9000000;
        g = f;
        do {
            p = NEXT_PRIM;
            gte_ldv3c(vtx);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    gte_stsxy3_g3(p);
                    gte_ldv0(vtx + 0x18);
                    gte_rtps();
                    gte_stflg(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL >= 0) {
                        gte_avsz4();
                        gte_stotz(&otz);
                        gte_stsxy(p + 0x20);
                        W(p, 4) = g[0];
                        W(p, 0xc) = g[1];
                        W(p, 0x14) = g[2];
                        H(p, 0x1c) = H(g, 0xc);
                        H(p, 0x24) = H(g, 0x10);
                        LINK(0x28)
                    }
                }
            }
            f += 13;
            g += 13;
            vtx += 0x28;
        } while (--n != 0);
    }
    /* GT3 */
    n = *f++;
    if (n != 0) {
        char *p;
        len = 0x9000000;
        g = f;
        do {
            p = NEXT_PRIM;
            gte_ldv3c(vtx);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    gte_avsz3();
                    gte_stotz(&otz);
                    gte_stsxy3_gt3(p);
                    W(p, 4) = g[0];
                    W(p, 0x10) = g[2];
                    W(p, 0x1c) = g[4];
                    W(p, 0xc) = g[1];
                    W(p, 0x18) = g[3];
                    H(p, 0x24) = H(g, 0x14);
                    LINK(0x28)
                }
            }
            f += 12;
            g += 12;
            vtx += 0x30;
        } while (--n != 0);
    }
    /* GT4 */
    n = *f++;
    if (n != 0) {
        char *p;
        len = 0xc000000;
        g = f;
        do {
            p = NEXT_PRIM;
            gte_ldv3c(vtx);
            gte_rtpt();
            gte_stflg(&LZ_FILE_CTRL);
            if (LZ_FILE_CTRL >= 0) {
                gte_nclip();
                gte_stopz(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL > 0) {
                    gte_stsxy3_gt3(p);
                    gte_ldv0(vtx + 0x18);
                    gte_rtps();
                    gte_stflg(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL >= 0) {
                        gte_avsz4();
                        gte_stotz(&otz);
                        gte_stsxy(p + 0x2c);
                        W(p, 4) = g[0];
                        W(p, 0x10) = g[2];
                        W(p, 0x1c) = g[4];
                        W(p, 0x28) = g[6];
                        W(p, 0xc) = g[1];
                        W(p, 0x18) = g[3];
                        H(p, 0x24) = H(g, 0x14);
                        H(p, 0x30) = H(g, 0x1c);
                        LINK(0x34)
                    }
                }
            }
            f += 16;
            g += 16;
            vtx += 0x40;
        } while (--n != 0);
    }
    if (lit) {
        /* F3 lit */
        n = *f++;
        if (n != 0) {
        char *p;
            len = 0x4000000;
            base = 0x20000000;
            do {
                p = NEXT_PRIM;
                gte_ldv3c(vtx);
                gte_rtpt();
                gte_stflg(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL >= 0) {
                    gte_nclip();
                    gte_stopz(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL > 0) {
                        gte_avsz3();
                        gte_stotz(&otz);
                        gte_stsxy3_f3(p);
                        DARK((unsigned char *)f + 0x24)
                        gte_ldrgb(&rgb);
                        gte_ldv0(vtx + 0x18);
                        gte_nccs();
                        gte_strgb(p + 4);
                        LINK(0x14)
                    }
                }
                f += 10;
                vtx += 0x20;
            } while (--n != 0);
        }
        /* F4 lit */
        n = *f++;
        if (n != 0) {
        char *p;
            len = 0x5000000;
            base = 0x28000000;
            do {
                p = NEXT_PRIM;
                gte_ldv3c(vtx);
                gte_rtpt();
                gte_stflg(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL >= 0) {
                    gte_nclip();
                    gte_stopz(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL > 0) {
                        gte_stsxy3_f3(p);
                        gte_ldv0(vtx + 0x18);
                        gte_rtps();
                        gte_stflg(&LZ_FILE_CTRL);
                        if (LZ_FILE_CTRL >= 0) {
                            gte_avsz4();
                            gte_stotz(&otz);
                            gte_stsxy(p + 0x14);
                            DARK((unsigned char *)f + 0x2c)
                            gte_ldrgb(&rgb);
                            gte_ldv0(vtx + 0x20);
                            gte_nccs();
                            gte_strgb(p + 4);
                            LINK(0x18)
                        }
                    }
                }
                f += 12;
                vtx += 0x28;
            } while (--n != 0);
        }
        /* G3 lit */
        n = *f++;
        if (n != 0) {
        char *p;
            len = 0x6000000;
            do {
                p = NEXT_PRIM;
                gte_ldv3c(vtx);
                gte_rtpt();
                gte_stflg(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL >= 0) {
                    gte_nclip();
                    gte_stopz(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL > 0) {
                        gte_avsz3();
                        gte_stotz(&otz);
                        gte_stsxy3_g3(p);
                        rgb = f[15];
                        gte_ldrgb(&rgb);
                        gte_ldv3c(vtx + 0x18);
                        gte_ncct();
                        gte_strgb3_g3(p);
                        LINK(0x1c)
                    }
                }
                f += 16;
                vtx += 0x30;
            } while (--n != 0);
        }
        /* G4 lit */
        n = *f++;
        if (n != 0) {
        char *p;
            len = 0x8000000;
            do {
                p = NEXT_PRIM;
                gte_ldv3c(vtx);
                gte_rtpt();
                gte_stflg(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL >= 0) {
                    gte_nclip();
                    gte_stopz(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL > 0) {
                        gte_stsxy3_g3(p);
                        gte_ldv0(vtx + 0x18);
                        gte_rtps();
                        gte_stflg(&LZ_FILE_CTRL);
                        if (LZ_FILE_CTRL >= 0) {
                            gte_avsz4();
                            gte_stotz(&otz);
                            gte_stsxy(p + 0x20);
                            rgb = f[20];
                            gte_ldrgb(&rgb);
                            gte_ldv0(vtx + 0x38);
                            gte_nccs();
                            gte_strgb(p + 0x1c);
                            gte_ldv3c(vtx + 0x20);
                            gte_ncct();
                            gte_strgb3_g3(p);
                            LINK(0x24)
                        }
                    }
                }
                f += 21;
                vtx += 0x40;
            } while (--n != 0);
        }
    } else {
        /* F3 */
        n = *f++;
        if (n != 0) {
        char *p;
        long *g;
            len = 0x4000000;
            g = f;
        do {
                p = NEXT_PRIM;
                gte_ldv3c(vtx);
                gte_rtpt();
                gte_stflg(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL >= 0) {
                    gte_nclip();
                    gte_stopz(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL > 0) {
                        gte_avsz3();
                        gte_stotz(&otz);
                        gte_stsxy3_f3(p);
                        if (CLIP3(8, 0xc, 0x10)) {
                            W(p, 4) = g[0];
                            LINK(0x14)
                        }
                    }
                }
                f += 10;
                vtx += 0x20;
                g += 10;
            } while (--n != 0);
        }
        /* F4 */
        n = *f++;
        if (n != 0) {
        char *p;
        long *g;
            len = 0x5000000;
            g = f;
        do {
                p = NEXT_PRIM;
                gte_ldv3c(vtx);
                gte_rtpt();
                gte_stflg(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL >= 0) {
                    gte_nclip();
                    gte_stopz(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL > 0) {
                        gte_stsxy3_f3(p);
                        gte_ldv0(vtx + 0x18);
                        gte_rtps();
                        gte_stflg(&LZ_FILE_CTRL);
                        if (LZ_FILE_CTRL >= 0) {
                            gte_avsz4();
                            gte_stotz(&otz);
                            gte_stsxy(p + 0x14);
                            if (CLIP4(8, 0xc, 0x10, 0x14)) {
                                W(p, 4) = g[0];
                                LINK(0x18)
                            }
                        }
                    }
                }
                f += 12;
                vtx += 0x28;
                g += 12;
            } while (--n != 0);
        }
        /* G3 */
        n = *f++;
        if (n != 0) {
        char *p;
        long *g;
            len = 0x6000000;
            g = f;
        do {
                p = NEXT_PRIM;
                gte_ldv3c(vtx);
                gte_rtpt();
                gte_stflg(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL >= 0) {
                    gte_nclip();
                    gte_stopz(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL > 0) {
                        gte_avsz3();
                        gte_stotz(&otz);
                        gte_stsxy3_g3(p);
                        if (CLIP3(8, 0x10, 0x18)) {
                            W(p, 4) = g[0];
                            W(p, 0xc) = g[1];
                            W(p, 0x14) = g[2];
                            LINK(0x1c)
                        }
                    }
                }
                f += 16;
                vtx += 0x30;
                g += 16;
            } while (--n != 0);
        }
        /* G4 */
        n = *f++;
        if (n != 0) {
        char *p;
            len = 0x8000000;
            do {
                p = NEXT_PRIM;
                gte_ldv3c(vtx);
                gte_rtpt();
                gte_stflg(&LZ_FILE_CTRL);
                if (LZ_FILE_CTRL >= 0) {
                    gte_nclip();
                    gte_stopz(&LZ_FILE_CTRL);
                    if (LZ_FILE_CTRL > 0) {
                        gte_stsxy3_g3(p);
                        gte_ldv0(vtx + 0x28);
                        gte_rtps();
                        gte_stflg(&LZ_FILE_CTRL);
                        if (LZ_FILE_CTRL >= 0) {
                            gte_avsz4();
                            gte_stotz(&otz);
                            gte_stsxy(p + 0x20);
                            if (CLIP4(8, 0x10, 0x18, 0x20)) {
                                W(p, 4) = f[0];
                                W(p, 0xc) = f[1];
                                W(p, 0x14) = f[2];
                                W(p, 0x1c) = f[3];
                                LINK(0x24)
                            }
                        }
                    }
                }
                vtx += 0x40;
                f += 21;
            } while (--n != 0);
        }
    }
}
#endif
