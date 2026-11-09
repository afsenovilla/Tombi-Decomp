// VERSION SLPS_01144 dspsub2 800EBD14 1604
#if defined(VERSION_SLPS_01144)
// JP memory card save (PAL d0spsub2 800e999c, src/d0spsub2/func_800E999C.c): builds the 0x2000 header and
// copies the game block. Matched 2026-11-08 with the PAL tricks (function-scope i/p shared by both digit loops,
// slot through a barriered $17, the tail copies through a barriered $16) plus: `p` also holds the place-name
// source pointer, the digit block as h100, h10, m10, d4, d1, d2, then the two `% 10`, and the `% 10` of m10 through
// an unsigned char temp. JP: header 'S','C',0x13,1; play time at 60 Hz; a 22-byte place name from a per-area table.
typedef struct { int d[3]; } B12;
typedef struct { char c[2]; } SJ;
typedef struct { char d[22]; } B22;
typedef struct { char d[64]; } B64;
typedef struct { char d[32]; } B32;
typedef struct { char d[0x180]; } B384;
typedef struct { char d[0x988]; } BGame;
extern char D_800E38F8[];         /* save data buffer */
extern char D_800E38F9, D_800E38FA, D_800E38FB;
extern char D_800E3AF8[];         /* its game block */
extern char D_8009E508[];         /* GAME */
extern unsigned short D_8009E508h, D_8009E50A; /* GAME area / section */
asm(".set D_8009E508h,D_8009E508");
extern int D_8009EE5C[], D_800A7B88[];
extern short D_8009E52A[];
extern char D_8009E51A[], D_8009EE8E[], D_8009EE8C[], D_8009EE8D[];
extern unsigned short D_800A0F6C;
extern unsigned char D_8009F20C, D_800A0C78;
extern unsigned int D_8009E510;   /* total play time */
extern B64 D_800EA290;
extern B32 D_800F2A58;
extern B384 D_800F28D8;
extern B22 **D_800F2860[];        /* place names per area / section */
extern SJ *D_800F28B0[];          /* digit glyphs */
extern void *func_8005879C(void *p, int c, int n); /* memset */
extern void memCardStoreChecksum(void *p, int n);

void func_800EBD14(int slot_)
{
    int slot;
    char *h = D_800E38F8;
    unsigned char dg[5];
    int i;
    unsigned char *p;
    {
        register int t asm("$17");
        t = slot_;
        asm("" : "=r"(t) : "0"(t));
        slot = t;
    }
    func_8005879C(h, 0, 0x2000);
    *(B12 *)D_8009EE5C = *(B12 *)D_800A7B88;
    D_8009E52A[0] = 0;
    D_8009E51A[0] = 1;
    h[0] = 'S';
    D_800E38F9 = 'C';
    D_800E38FA = 0x13;
    D_800E38FB = 1;
    D_8009EE8E[0] = D_800A0F6C;
    D_8009EE8C[0] = D_8009F20C;
    D_8009EE8D[0] = D_800A0C78;
    *(B64 *)(h + 4) = D_800EA290;
    {
        unsigned int hh = D_8009E510 / 216000;
        int h1 = hh % 1000;
        int m = D_8009E510 / 3600 - hh * 60;
        {
            int m10, h100, h10; unsigned char d4, d1, d2, d3;
            p = (unsigned char *)D_800F2860[D_8009E508h][D_8009E50A];
            *(B22 *)(h + 0x2E) = *(B22 *)p;
            h100 = h1 / 100;
            h10 = h1 / 10;
            m10 = m / 10;
            d4 = m - m10 * 10;
            dg[4] = d4;
            d1 = h10 - h100 * 10;
            dg[1] = d1;
            d2 = h1 - h10 * 10;
            dg[2] = d2;
            dg[3] = (d3 = m10 % 10);
            dg[0] = h100 % 10;
        }
    }
    {
        p = dg;
        for (i = 0; i < 6; i++) {
            if (i != 3) *(SJ *)&h[0x20 + i * 2] = *D_800F28B0[*p++];
        }
    }
    {
        int s10 = slot / 10;
        dg[1] = slot % 10;
        dg[0] = s10 % 10;
        p = dg;
        for (i = 0; i < 2; i++) {
            *(SJ *)&h[0x16 + i * 2] = *D_800F28B0[*p++];
        }
    }
    func_8005879C(h + 0x44, 0, 0x1C);
    {
        register char *g asm("$16");
        g = h;
        asm("" : "=r"(g) : "0"(g));
        *(B32 *)(g + 0x60) = D_800F2A58;
        *(B384 *)(g + 0x80) = D_800F28D8;
    }
    *(BGame *)D_800E3AF8 = *(BGame *)D_8009E508;
    memCardStoreChecksum(D_800E3AF8, 0xA00);
}
#endif
