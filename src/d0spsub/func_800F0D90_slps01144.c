// VERSION SLPS_01144 dspsub 800F0D90 4720
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800efd6c (sub-screen page draw), reworked: modes 0 (status), 1 (items), 2 (events),
// 3 (map) and 6 (JP list); the pages draw tab headers first and a mode title (DRAW_TITLE) before the frame.
// Case 1 is the code of DSPSUB3 func_800EF1AC, written with the same forms. The form of each Text-global store
// decides the schedule: the array forms (Xa[0] ...) are MEM_IN_STRUCT_P, so the Slot/Menu loads stay after them,
// and each one also adds an insn at loop time (which constants loop.c hoists: 0x7C15 / 0x7A92 in case 0); the
// plain externs (D_8011140E ...) let the loads move up and share no address register; D_80111408.x/.y in case 0
// share one base register (s1) from the bitten-jump sprite to the title.
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xc];
    short w10;
    short mode;
    short page;
    char p16[0x32];
    void **p48, **p4c;
    char p50[4];
    void **p54, **p58;
    char p5c[4];
    void **p60;
} Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; unsigned short attr; void *img; int pC; unsigned short *str; void *font; } Text;
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { char p0[0x12]; short rx; short p14; short ry; short p18; short rz; } Model;
typedef struct { char p0[0xa]; unsigned char b0a; char p0b[0xf]; short zi; char p1c[0xb8]; } Sprite;
typedef struct { char p0[0xd4]; } Obj;
extern Menu D_800A6120;
extern short D_800A6132;
extern char D_800B2EE0[];
extern short D_800B2F28;
extern unsigned char D_800B2F44, D_800B2F45, D_800B2F46;
/* Text globals: x D_80111408, y D_8011140A, w D_8011140C, attr D_8011140E, img D_80111410, str D_80111418,
   font D_8011141C */
extern Text D_80111408;
extern short D_8011140A, D_8011140C;
extern unsigned short D_8011140E;
extern void *D_80111410;
extern unsigned short *D_80111418;
extern short Xa[] asm("D_80111408"), Ya[] asm("D_8011140A"), Wa[] asm("D_8011140C"), Aa[] asm("D_8011140E");
extern void *Ia[] asm("D_80111410");
extern unsigned short *Sa[] asm("D_80111418");
extern void *Fa[] asm("D_8011141C");
extern unsigned char Ka[] asm("D_8009EE59"); /* bitten-jump kind; array form: read between the x and y stores */
extern Slot D_8010941C[], D_80109434[], D_801094D0[], D_801094AC[], D_801093EC[], D_801093FC[];
extern Slot D_80109EA4[], D_80109F04[], D_80109F1C[];
extern unsigned char D_801094CC[];
extern unsigned short D_8009E508, D_8009E50A;
extern unsigned short *D_80108F6C[], *D_80108F1C[];
extern unsigned short **D_801091A4[];
extern char D_8010580C[];
extern short D_8010A31C;
extern unsigned short D_8009EE4C;
extern unsigned char D_80108D0C[], D_8009E94B[];
extern unsigned short D_8010A64E, D_8010A650, D_8010A652;
extern MATRIX VIEW_CAMERA;
extern SVECTOR WORK_VEC_A, WORK_VEC_B;
extern short D_801113BC, D_801113C4, D_801113C8, D_801113CC;
extern Model D_8010A66C asm("D_8010A67E-18"); /* map model at 0x8010A66C, named after its rx field like the game */
extern short D_8010A682[], D_8010A686[];
extern Obj D_80111214[];
extern char D_8010A740[], SUBSCREEN_MAP_OBJECTS[];
extern Sprite D_8010A814[];
extern int D_1F8002B4;
extern void func_800EB648(Menu *m);
extern void func_8004BEFC(void *h, int x, int y);
extern void func_8004C0F0(void *h, int n, int x, int y, int f);
extern void func_800EC7B8(Menu *m, int n);
extern void func_800EB450(Menu *m);
extern void func_800EC9D8(int pri);
extern void func_8004BD10(void *h, int x, int y);
extern void func_8004BB8C(void *h, int x, int y);
extern void func_800ED0DC(int pri);
extern void func_800ED344(Menu *m);
extern void func_800EBFC0(Menu *m);
extern void func_800EC314(Menu *m);
extern void subScreenDrawPageNumber(Menu *m, int n);
extern void func_800EB868(Menu *m);
extern void func_800EBA58(Menu *m);
extern void subScreenDrawEventCounters(Menu *m);
extern void func_800F24DC(Menu *m, int a, int b);
extern void func_800F25EC(Menu *m, int a, int b);
extern void getBaseMatrix(MATRIX *m);
extern void RotMatrixZ(int r, MATRIX *m);
extern void RotMatrixX(int r, MATRIX *m);
extern void ApplyMatrixSV(MATRIX *m, SVECTOR *a, SVECTOR *b);
extern void func_8004A8AC(Model *o);
extern void func_80048FB8(Obj *o);
extern void func_80045F10(void *o);
extern int rsin(int a);
extern void func_8004630C(Sprite *o);
extern void subScreenMapDrawRoutes(void);

/* tab headers of the page (as in DSPSUB3 func_800EF1AC) */
#define DRAW_HEADERS(m) { \
    Slot *s; \
    switch (m->mode) { \
    case 1: \
    case 2: \
    case 3: \
        s = D_80109434; \
        break; \
    case 0: \
    case 7: \
        s = D_8010941C; \
        break; \
    } \
    Wa[0] = m->w10 + s->dw; \
    for (; s->x != -1; s++) { \
        int k; \
        short y; \
        Xa[0] = s->x; \
        y = s->y; \
        asm volatile("li %0,0x7A92" : "=r"(k)); /* the game reloads the constant in the loop */ \
        Aa[0] = k; \
        Ya[0] = y; \
        if (s->idx == 4) D_8011140E = k; \
        Ia[0] = m->p48[s->idx]; \
        func_800EC9D8(1); \
    } \
}

/* page arrows: 0x16 hidden on the first page, 0x17 on the last */
#define DRAW_ARROWS(m, pages) \
    if (pages >= 2) { \
        Slot *s; \
        subScreenDrawPageNumber(m, pages); \
        s = D_80109EA4; \
        while (s->x != -1) { \
            if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) { \
                s++; \
            } else { \
                Xa[0] = s->x; \
                Ya[0] = s->y; \
                Wa[0] = m->w10 + s->dw; \
                Aa[0] = 0x7C15; \
                Ia[0] = m->p58[s->idx]; \
                func_800EC9D8(1); \
                s++; \
            } \
        } \
    }

/* mode title */
#define DRAW_TITLE(m) \
    Aa[0] = 0x7912; \
    Wa[0] = m->w10; \
    if (m->mode >= 6) { \
        Xa[0] = 0x20; \
        Ya[0] = 0x10; \
        Ia[0] = m->p4c[1]; \
    } else { \
        Xa[0] = 0x20; \
        if (m->mode == 3) Ya[0] = 0x18; \
        else Ya[0] = 0x10; \
        Ia[0] = m->p4c[m->mode]; \
    } \
    func_800EC9D8(1);

/* frame parts of the page */
#define DRAW_FRAME(T, A) { \
    Slot *s; \
    end = -1; \
    Aa[0] = A; \
    for (s = T; s->x != end; s++) { \
        Xa[0] = s->x; \
        Ya[0] = s->y; \
        Wa[0] = m->w10 + s->dw; \
        Ia[0] = m->p60[s->idx]; \
        func_800EC9D8(3); \
    } \
}

#define DRAW_STATUS_PARTS(T) { \
    Slot *s; \
    for (s = T; s->x != -1; s++) { \
        Xa[0] = s->x; \
        Ya[0] = s->y; \
        Wa[0] = m->w10 + s->dw; \
        D_8011140E = 0x7A92; \
        D_80111410 = m->p48[s->idx]; \
        func_800EC9D8(1); \
    } \
}

void func_800F0D90(void)
{
    Menu *m = &D_800A6120;
    short end;
    int pages;
    int i, last;
    switch (D_800A6132) {
    case 0:
        D_800B2F28 = 1;
        func_800EB648(m);
        DRAW_HEADERS(m)
        func_8004BEFC(D_800B2EE0, 0x20, 0xBA);
        D_800B2F44 = 0;
        D_800B2F45 = 0;
        D_800B2F46 = 0;
        func_8004C0F0(D_800B2EE0, 0, 0xE, 0x5E, 1);
        func_8004C0F0(D_800B2EE0, 1, 0xE, 0x6E, 1);
        func_8004C0F0(D_800B2EE0, 2, 0xE, 0x7E, 1);
        func_800EC7B8(m, 0);
        func_800EC7B8(m, 1);
        func_800EC7B8(m, 2);
        func_800EB450(m);
        {
            Slot *s;
            for (s = D_801094D0; s->x != -1; s++) {
                Xa[0] = s->x;
                Ya[0] = s->y;
                Wa[0] = m->w10 + s->dw;
                D_8011140E = 0x7C15;
                D_80111410 = m->p58[s->idx];
                func_800EC9D8(1);
            }
        }
        {
            Slot *s;
            D_80111408.x = 0x50;
            D_80111408.y = 0xB8;
            s = &D_801094AC[Ka[0]];
            D_8011140E = 0x7892;
            D_8011140C = m->w10 + s->dw;
            D_80111410 = m->p54[D_801094CC[Ka[0]]];
            func_800EC9D8(1);
            Xa[0] = s->x;
            Ya[0] = s->y;
            Wa[0] = m->w10 + s->dw;
            Aa[0] = 0x7C15;
            D_80111410 = m->p58[s->idx];
            func_800EC9D8(1);
        }
        func_8004BD10(D_800B2EE0, 0x98, 0x10);
        func_8004BB8C(D_800B2EE0, 0x18, 0x28);
        D_8011140E = 0x7912;
        D_8011140C = m->w10;
        if (m->mode >= 6) {
            D_80111408.x = 0x20;
            D_80111408.y = 0x10;
            Ia[0] = m->p4c[1];
        } else {
            D_80111408.x = 0x20;
            if (m->mode == 3) D_80111408.y = 0x18;
            else D_80111408.y = 0x10;
            Ia[0] = m->p4c[m->mode];
        }
        func_800EC9D8(1);
        D_80111408.x = 0x30;
        D_8011140A = 0x1C;
        D_8011140E = 0x7C15;
        D_8011140C = m->w10 + 1;
        if (D_8009E508 == 9) D_80111418 = D_80108F6C[D_8009E50A];
        else D_80111418 = D_80108F1C[D_8009E508];
        Fa[0] = D_8010580C;
        func_800ED0DC(5);
        Wa[0] = m->w10 + 1;
        D_80111408.x = 0x30;
        Ya[0] = 0x2C;
        Aa[0] = 0x7C15;
        Sa[0] = D_801091A4[D_8009E508][D_8009E50A];
        Fa[0] = D_8010580C;
        func_800ED0DC(5);
        DRAW_STATUS_PARTS(D_801093EC)
        DRAW_STATUS_PARTS(D_801093FC)
        break;
    case 1:
        func_800EB648(m);
        DRAW_HEADERS(m)
        if (D_8010A31C >= 0) func_800ED344(m);
        pages = (D_8009EE4C + 15) / 16;
        func_800EBFC0(m);
        func_800EC314(m);
        DRAW_ARROWS(m, pages)
        DRAW_TITLE(m)
        DRAW_FRAME(D_80109F04, 0x7BD3)
        break;
    case 2:
        func_800EB648(m);
        DRAW_HEADERS(m)
        {
            int j;
            last = 0;
            for (j = 0; j < 0x100; j++) {
                if (D_80108D0C[j] != 0xFF && D_8009E94B[D_80108D0C[j]] != 0) last = j;
            }
        }
        pages = (last + 16) / 16;
        func_800EB868(m);
        func_800EBA58(m);
        subScreenDrawEventCounters(m);
        DRAW_ARROWS(m, pages)
        DRAW_TITLE(m)
        DRAW_FRAME(D_80109F1C, 0x7B93)
        break;
    case 3:
        func_800EB648(m);
        DRAW_TITLE(m)
        if (D_8010A652 != 0) {
            func_800F24DC(m, D_8010A64E, D_8010A650);
            func_800F25EC(m, D_8010A64E, D_8010A650);
        }
        if (m->b1 != 0) {
            getBaseMatrix(&VIEW_CAMERA);
            RotMatrixZ(D_801113C4, &VIEW_CAMERA);
            RotMatrixX(D_801113BC, &VIEW_CAMERA);
            VIEW_CAMERA.t[0] = 0;
            WORK_VEC_A.vx = 0;
            WORK_VEC_A.vy = 0;
            WORK_VEC_A.vz = 0;
            VIEW_CAMERA.t[1] = D_801113C8;
            VIEW_CAMERA.t[2] = D_801113CC + 1000;
            ApplyMatrixSV(&VIEW_CAMERA, &WORK_VEC_A, &WORK_VEC_B);
            D_8010A66C.rx = 0;
            D_8010A682[0] = -1000;
            D_8010A686[0] = 0;
            VIEW_CAMERA.t[0] += WORK_VEC_B.vx;
            VIEW_CAMERA.t[1] += WORK_VEC_B.vy;
            VIEW_CAMERA.t[2] += WORK_VEC_B.vz;
            func_8004A8AC(&D_8010A66C);
            for (i = 0; i < 2; i++) func_80048FB8(&D_80111214[i]);
            func_80045F10(D_8010A740);
            func_80045F10(SUBSCREEN_MAP_OBJECTS);
            for (i = 0; i < 0x80; i++) {
                D_1F8002B4 = (rsin(D_801113BC - 0x1AA) - D_8010A814[i].zi) * 3 + 0x1000;
                D_8010A814[i].b0a = 0;
                func_8004630C(&D_8010A814[i]);
            }
            subScreenMapDrawRoutes();
        }
        break;
    case 6:
        pages = (D_8009EE4C + 15) / 16;
        func_800EBFC0(m);
        func_800EC314(m);
        DRAW_ARROWS(m, pages)
        DRAW_TITLE(m)
        DRAW_FRAME(D_80109F04, 0x7BD3)
        break;
    }
}
#endif
