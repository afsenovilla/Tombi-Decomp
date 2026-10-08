// FUNC 8005aa74 2328 MAIN0
/* score 136 (ncheck): structure, switch, loop hoisting and caller-saves all match. Left:
 * (1) p4 and flag f2 swap registers (game: p4=$fp, f2=$a2 caller-saved); type brute force and
 *     declaration order did not fix it (int f1 gives 190 without the andi but same swap);
 * (2) id==1 block: game stores a,y then lh G before sw d30 (perms of the 6 stores tried; the
 *     natural a,y,b,d30,d34,d38 order makes loop.c hoist dy<<16);
 * (3) loop 2: wc0/wc8/wca load order (reordering flips the global register allocation).
 * Tried: explicit pointer locals (worse), volatile dx/dy (worse), register asm (worse). */
typedef struct { unsigned short frac; short whole; } FixParts;
typedef union { int raw; FixParts p; } Fix16;

typedef struct {
    unsigned char active;   /* 0x00 */
    unsigned char b01;
    unsigned char type;     /* 0x02 */
    unsigned char subtype;  /* 0x03 */
    char p04[4];
    short w08;              /* 0x08 */
    char p0a[2];
    unsigned char b0c;      /* 0x0c */
    unsigned char b0d;      /* 0x0d */
    char p0e;
    signed char b0f;        /* 0x0f */
    int a;                  /* 0x10 */
    int y;                  /* 0x14 */
    int b;                  /* 0x18 */
    unsigned char category; /* 0x1c */
    char p1d;
    short w1e;              /* 0x1e */
    short timer;            /* 0x20 */
    char p22[0xc];
    short w2e;              /* 0x2e */
    int d30, d34, d38;      /* 0x30 */
    char p3c[0xa0 - 0x3c];
    int da0;                /* 0xa0 */
    char pa4[0xbc - 0xa4];
    unsigned short wbc;     /* 0xbc */
    char pbe[2];
    short wc0;              /* 0xc0 */
    char pc2[2];
    short wc4, wc6, wc8, wca; /* 0xc4 */
    unsigned short wcc, wce, wd0, wd2; /* 0xcc */
} Q;

typedef struct {
    unsigned char b3c, b3d, b3e, b3f; /* 0x800a603c */
    char p40[8];
    Fix16 f48;                        /* 0x800a6048 */
    Fix16 f4c;                        /* 0x800a604c */
    Fix16 f50;                        /* 0x800a6050 */
    char p54[0xd6 - 0x54];
    unsigned char bd6;                /* 0x800a60d6 */
    char pd7[0xe4 - 0xd7];
    unsigned char be4;                /* 0x800a60e4 */
} G;

typedef struct { short a, b; } SS;

extern G D_800A603C;
extern Fix16 D_800A4558;
extern Fix16 D_1F8000F0;
extern unsigned char *D_1F80039C;
extern char *D_1F80033C;
extern unsigned char *D_800806B4[];
extern unsigned short D_8009C960;
extern unsigned short D_8009C962;
extern unsigned char D_8009C933;
extern signed char D_8009D2B0[];
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009C940;
extern unsigned char D_8009C93E;
extern short D_800804DA[2];
extern short D_800804E0[];
extern SS D_800804EC[];
extern signed char **D_8013A044, **D_8013D8A4, **D_800F0E3C, **D_8011F13C, **D_800F13CC;
extern signed char **D_801385E4, **D_801339A0, **D_80102F9C, **D_8011A000, **D_801228B8;
extern signed char **D_80118C64, **D_801184C4, **D_8012D9E0, **D_80131A78, **D_8011C208;
extern signed char **D_80119884, **D_8011A7EC, **D_80128C70, **D_8011B350, **D_8011C088;
extern signed char **D_8011CA44;
extern Q *FUN_800185f8(void);

#define FLAGS() \
        if (D_8009D2B0[0] != 3 && !f1) { \
            if (p4 != 0) { \
                q->wd0 = D_8009C93F[0]; \
                D_8009C93F[0] = 1; \
                D_8009C942[0] = 1; \
                if (p4 != 4 && (D_800A603C.bd6 == 0 || D_800A603C.be4 < 2)) { \
                    D_800A603C.b3c = 5; \
                    D_800A603C.b3d = 0; \
                    D_800A603C.b3e = 0; \
                    D_800A603C.b3f = 0; \
                } else { \
                    if (D_8009C940 == 0) D_8009C93E = 1; \
                } \
                q->wce = p4; \
            } \
            f1 = 1; \
            q->wce |= 0x8000; \
        } \
        if (id == 15 && kind == 1 && !f2) { \
            f2 = 1; \
            q->wd2 = 0x27; \
            q->wce = 0x8002; \
        } else { \
            q->wd2 = 0; \
        }

#define POS() \
        q->category |= 0x80; \
        q->da0 = (int)(*(char *volatile *)&D_1F80033C + *(int *)(*(char *volatile *)&D_1F80033C + 4)); \
        if (id == 1) { \
            int v = (dx << 16) - 0xa00000; \
            q->d30 = v; \
            q->a = v; \
            q->b = (D_800A603C.f50.p.whole + 10) << 16; \
            q->y = dy << 16; \
            q->d34 = dy << 16; \
            q->d38 = (D_800A603C.f50.p.whole + 10) << 16; \
        } else { \
            q->a = D_800804DA[0] << 16; \
            q->y = D_800804DA[1] << 16; \
            q->b = 0; \
            q->d30 = D_800804DA[0] << 16; \
            q->d34 = D_800804DA[1] << 16; \
            q->d38 = 0; \
        }

void FUN_8005aa74(int id, int kind, int w, int p4)
{
    unsigned short dx, dy;
    unsigned char *h;
    unsigned char k;
    unsigned short *t;
    signed char **tab;
    signed char *p;
    unsigned char cnt;
    short n;
    unsigned short f1;
    int f2;
    Q *q;
    short e;
    short *pos;
    short *cy;
    unsigned char *fl;

    if (id == 1) {
        w = 1;
        dx = D_800A603C.f48.p.whole - D_800A4558.p.whole;
        dy = D_800A603C.f4c.p.whole - D_1F8000F0.p.whole;
    }
    h = D_1F80039C;
    h += *(unsigned short *)(h + 8);
    k = D_800806B4[D_8009C960][id];
    if (k == 0xff) {
        t = (unsigned short *)(h + 2);
    } else {
        t = (unsigned short *)(h + k * 2);
    }
    h += *t;
    cnt = 0;
    switch (D_8009C960) {
    case 0: tab = D_8013A044; break;
    case 1: tab = D_8013D8A4; break;
    case 2:
        switch (D_8009C962) {
        case 3: tab = D_800F13CC; break;
        case 0: tab = D_800F0E3C; break;
        case 1: case 2: case 4: case 5: tab = D_8011F13C; break;
        }
        break;
    case 3: tab = D_801385E4; break;
    case 4: tab = D_801339A0; break;
    case 5:
        switch (D_8009C962) {
        case 0: tab = D_80102F9C; break;
        case 1: tab = D_8011A000; break;
        case 2: tab = D_80102F9C; break;
        case 3: tab = D_8011A000; break;
        }
        break;
    case 6: tab = D_801228B8; break;
    case 7: tab = D_80118C64; break;
    case 8:
        switch (D_8009C962) {
        case 0: tab = D_80102F9C; break;
        case 1: tab = D_801184C4; break;
        case 2: tab = D_80102F9C; break;
        case 3: tab = D_801184C4; break;
        }
        break;
    case 9: tab = D_8012D9E0; break;
    case 10: tab = D_80131A78; break;
    case 11: tab = D_8011C208; break;
    case 12: tab = D_80119884; break;
    case 13: tab = D_8011A7EC; break;
    case 14: tab = D_80128C70; break;
    case 16: tab = D_8011B350; break;
    case 17: tab = D_8011C088; break;
    case 18: tab = D_8011CA44; break;
    case 19:
        switch (D_8009C962) {
        case 0: tab = D_800F0E3C; break;
        case 1: tab = D_8011F13C; break;
        case 2: tab = D_80102F9C; break;
        }
        break;
    }
    f1 = 0;
    f2 = 0;
    p = tab[id];
    D_8009C933 = 1;
    n = 0;
    if (*p != -1) {
    do {
        q = FUN_800185f8();
        if (q != 0) {
            q->active = 1;
            q->type = 1;
            q->b0d = 1;
            q->w08 = 0x7d16;
            q->subtype = kind;
            q->w1e = 0;
            q->b0f = -90;
            q->w2e = 1;
            q->timer = w;
            n++;
            POS();
            q->b0c = cnt;
            e = p[3] & 0xf;
            q->wbc = *(unsigned short *)h & 0xfff;
            q->wc0 = D_800804E0[e];
            q->wc4 = p[1] * 10;
            q->wc6 = p[2] * 14;
            q->wc8 = D_800804EC[e].a;
            q->wca = D_800804EC[e].b;
            cnt++;
            q->wcc = (unsigned char)p[3];
            q->wce = 0;
            h += 2;
            FLAGS();
        }
        p += 4;
    } while (*p != -1);
    }
    if (n < 6 && kind == 1) {
        for (; n < 6; n++) {
            q = FUN_800185f8();
            if (q != 0) {
                q->active = 2;
                q->type = 1;
                q->b0d = 1;
                q->w08 = 0x7d16;
                q->subtype = kind;
                q->w1e = 0;
                q->b0f = -90;
                q->w2e = 1;
                q->timer = w;
                POS();
                q->b0c = cnt;
                e = n & 0xf;
                q->wc4 = 0;
                q->wc6 = 0;
                q->wc0 = D_800804E0[e];
                q->wc8 = D_800804EC[e].a;
                q->wcc = n | 0x80;
                q->wce = 0;
                q->wca = D_800804EC[e].b;
                cnt++;
                FLAGS();
            }
        }
    }
}
