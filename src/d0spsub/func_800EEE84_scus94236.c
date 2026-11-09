// VERSION SCUS_94236 dspsub 800EEE84 2956
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub 800efd6c (func_800EFD6C.c): the same C with the US addresses, NTSC coordinates, 8 entries
// a page, both status lines from a [GAME][section] string table (no GAME == 9 case) and w = m->w10. With no if/else
// in case 0 the whole run from loop 1 to loop 2 is one block: the two status lines write w as D_8011030C.w (cse
// relates it to the x base and cse2 folds it back to an absolute store); the one-field alias Ws there would be one
// address pseudo for both stores and cost an extra s-register (s6). Frame 0x98: s3 = &x, s0 = 16, s1 = 0x7C15,
// s2 = the font and s4 = the loop-2 table live across the calls.
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xc];
    short w10;
    short mode;
    short page;
    char p16[0x32];
    int *p48;
    char p4c[8];
    int *p54;
    int *p58;
} Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *font; } Text;
typedef struct { short m[3][3]; long t[3]; } MATRIX;
typedef struct { short vx, vy, vz, pad; } SVECTOR;
typedef struct { char p0[0xa]; unsigned char b0a; char p0b[0xf]; short zi; char p1c[0xb8]; } Sprite;
typedef struct { char p0[0x48]; short w48; char p4a[0x1a]; unsigned char b64, b65, b66; } Hud;
typedef struct { char p0[0x12]; short rx; short p14; short ry; short p18; short rz; } Model;
extern Menu D_800A3940;
extern Text D_8011030C;
extern short D_80110310;
extern unsigned short D_80110312;
extern int D_80110314;
extern int D_8011031C;
typedef struct { short v; } S16;
extern S16 Xs asm("D_8011030C"), Ys asm("D_8011030E"), Ws asm("D_80110310");
extern Hud D_800B0770;
extern unsigned char D_8009BCD8, D_8009C619;
extern unsigned short D_8009BCC8, D_8009BCCA, D_8009C60C;
extern short D_80109228;
extern Slot D_801083FC[], D_801083D8[], D_801083A0[], D_80108E40[], D_80108DC8[], D_80108E28[];
extern int D_80108394[];
extern unsigned char D_801083F8[];
extern int *D_80107EB8[], *D_801081A8[];
extern int D_800FD948[];
extern char D_800FEE44[];
extern unsigned char D_801074A8[], D_8009C10B[];
extern unsigned short D_8010955A, D_8010955C, D_8010955E;
extern MATRIX VIEW_CAMERA;
extern SVECTOR D_1F800060, D_1F800068;
extern short D_801102D0, D_801102C8, D_801102D4, D_801102D8;
extern Model D_80109578;
extern char D_8010964C[], D_801094A4[];
extern Sprite D_80109720[];
extern int D_1F8002B4;
extern void subScreenDrawTabs(Menu *m);
extern void drawHudVitality(Hud *h, int x, int y, int v);
extern void drawHudItemGauge(Hud *h, int n, int x, int y, int f);
extern void subScreenDrawGaugePercent(Menu *m, int n);
extern void subScreenDrawStatusNotes(Menu *m);
extern void subScreenDrawString(int a, int b);
extern void subScreenDrawSprite(int n);
extern void subScreenDrawDigitsSprite(Hud *h, int x, int y);
extern void drawHudLives(Hud *h, int x, int y);
extern void subScreenDrawPopupMenu(Menu *m);
extern void subScreenDrawItemGrid(Menu *m);
extern void subScreenDrawItemNames(Menu *m);
extern void subScreenDrawPageNumber(Menu *m, int n);
extern void subScreenDrawEventIcons(Menu *m, int n, int c);
extern void subScreenDrawEventList(Menu *m);
extern void subScreenDrawEventCounters(Menu *m);
extern void subScreenMapDrawAreaName(Menu *m, int a, int b);
extern void subScreenMapDrawRoomName(Menu *m, int a, int b);
extern void getBaseMatrix(MATRIX *m);
extern void RotMatrixZ(int r, MATRIX *m);
extern void RotMatrixX(int r, MATRIX *m);
extern void ApplyMatrixSV(MATRIX *m, SVECTOR *a, SVECTOR *b);
extern void drawObjectModelRotated(Model *o);
extern void func_800487A4_scus94236(void *o);
extern void func_80048BF0_scus94236(Sprite *o);
extern int rsin(int a);
extern void subScreenMapDrawRoutes(void);

void func_800EEE84_scus94236(void)
{
    Menu *m = &D_800A3940;
    Slot *s;
    short end;
    int pages;
    int i, last;
    switch (m->mode) {
    case 0:
        D_800B0770.w48 = 1;
        subScreenDrawTabs(m);
        drawHudVitality(&D_800B0770, 0xC0, 0x60, D_8009BCD8);
        D_800B0770.b64 = 0;
        D_800B0770.b65 = 0;
        D_800B0770.b66 = 0;
        drawHudItemGauge(&D_800B0770, 0, 0xE, 0x4E, 1);
        drawHudItemGauge(&D_800B0770, 1, 0xE, 0x5E, 1);
        drawHudItemGauge(&D_800B0770, 2, 0xE, 0x6E, 1);
        subScreenDrawGaugePercent(m, 0);
        subScreenDrawGaugePercent(m, 1);
        subScreenDrawGaugePercent(m, 2);
        subScreenDrawStatusNotes(m);
        {
        Slot *s;
        for (s = D_801083FC; s->x != -1; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            D_8011030C.attr = 0x7C15;
            D_8011031C = D_80108394[s->idx];
            D_8011030C.font = D_800FEE44;
            subScreenDrawString(5, 1);
        }
        }
        D_8011030C.x = 0xE8;
        Ys.v = 0x56;
        s = &D_801083D8[D_8009C619];
        D_80110310 = m->w10 + s->dw;
        D_80110312 = 0x7892;
        D_80110314 = m->p54[D_801083F8[D_8009C619]];
        subScreenDrawSprite(1);
        Xs.v = s->x;
        D_8011030C.y = s->y;
        D_8011030C.w = m->w10 + s->dw;
        D_8011030C.attr = 0x7C15;
        D_80110314 = m->p58[s->idx];
        subScreenDrawSprite(1);
        subScreenDrawDigitsSprite(&D_800B0770, 0x132, 0x40);
        drawHudLives(&D_800B0770, 0x120, 0x5A);
        D_8011030C.x = 0x10;
        D_8011030C.y = 0xB8;
        D_8011030C.w = m->w10;
        D_8011030C.attr = 0x7C15;
        D_8011031C = D_80107EB8[D_8009BCC8][D_8009BCCA];
        D_8011030C.font = D_800FEE44;
        subScreenDrawString(5, 0);
        D_8011030C.x = 0x10;
        D_8011030C.y = 0xC8;
        D_8011030C.w = m->w10;
        D_8011030C.attr = 0x7C15;
        D_8011031C = D_801081A8[D_8009BCC8][D_8009BCCA];
        D_8011030C.font = D_800FEE44;
        subScreenDrawString(5, 0);
        {
        Slot *s;
        for (s = D_801083A0; s->x != -1; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            D_80110312 = 0x79D2;
            D_80110314 = m->p48[s->idx];
            subScreenDrawSprite(1);
        }
        }
        D_80110312 = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80108E40; s->x != end; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            D_80110314 = D_800FD948[s->idx];
            subScreenDrawSprite(3);
        }
        }
        return;
    case 1:
        subScreenDrawTabs(m);
        if (D_80109228 >= 0) subScreenDrawPopupMenu(m);
        pages = (D_8009C60C + 7) / 8;
        subScreenDrawItemGrid(m);
        subScreenDrawItemNames(m);
        if (pages >= 2) {
            subScreenDrawPageNumber(m, pages);
            {
            Slot *s;
            s = D_80108DC8;
            while (s->x != -1) {
                if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                    s++;
                } else {
                    Xs.v = s->x;
                    Ys.v = s->y;
                    Ws.v = m->w10 + s->dw;
                    D_80110312 = 0x7C15;
                    D_80110314 = m->p58[s->idx];
                    subScreenDrawSprite(1);
                    s++;
                }
            }
            }
        }
        D_80110312 = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80108E28; s->x != end; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            D_80110314 = D_800FD948[s->idx];
            subScreenDrawSprite(3);
        }
        }
        break;
    case 2:
        subScreenDrawTabs(m);
        {
        int ff;
        int j;
        last = 0;
        for (j = 0, ff = 0xFF; j < 0x100; j++) {
            if (D_801074A8[j] != ff && D_8009C10B[D_801074A8[j]] != 0) last = j;
        }
        pages = (last + 8) / 8;
        subScreenDrawEventIcons(m, last, ff);
        }
        subScreenDrawEventList(m);
        subScreenDrawEventCounters(m);
        if (pages >= 2) {
            subScreenDrawPageNumber(m, pages);
            {
            Slot *s;
            s = D_80108DC8;
            while (s->x != -1) {
                if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                    s++;
                } else {
                    Xs.v = s->x;
                    Ys.v = s->y;
                    Ws.v = m->w10 + s->dw;
                    D_80110312 = 0x7C15;
                    D_80110314 = m->p58[s->idx];
                    subScreenDrawSprite(1);
                    s++;
                }
            }
            }
        }
        D_80110312 = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80108E28; s->x != end; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            D_80110314 = D_800FD948[s->idx];
            subScreenDrawSprite(3);
        }
        }
        break;
    case 3:
        subScreenDrawTabs(m);
        D_80110312 = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80108E40; s->x != end; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            D_80110314 = D_800FD948[s->idx];
            subScreenDrawSprite(3);
        }
        }
        if (D_8010955E != 0) {
            subScreenMapDrawAreaName(m, D_8010955A, D_8010955C);
            subScreenMapDrawRoomName(m, D_8010955A, D_8010955C);
        }
        if (m->b1 != 0) {
            getBaseMatrix(&VIEW_CAMERA);
            RotMatrixZ(D_801102D0, &VIEW_CAMERA);
            RotMatrixX(D_801102C8, &VIEW_CAMERA);
            VIEW_CAMERA.t[0] = 0;
            D_1F800060.vx = 0;
            D_1F800060.vy = 0;
            D_1F800060.vz = 0;
            VIEW_CAMERA.t[1] = D_801102D4;
            VIEW_CAMERA.t[2] = D_801102D8 + 1000;
            ApplyMatrixSV(&VIEW_CAMERA, &D_1F800060, &D_1F800068);
            D_80109578.rx = 0;
            D_80109578.ry = -1000;
            D_80109578.rz = 0;
            VIEW_CAMERA.t[0] += D_1F800068.vx;
            VIEW_CAMERA.t[1] += D_1F800068.vy;
            VIEW_CAMERA.t[2] += D_1F800068.vz;
            drawObjectModelRotated(&D_80109578);
            func_800487A4_scus94236(D_8010964C);
            func_800487A4_scus94236(D_801094A4);
            for (i = 0; i < 0x80; i++) {
                D_1F8002B4 = (rsin(D_801102C8 - 0x1AA) - D_80109720[i].zi) * 3 + 0x1000;
                D_80109720[i].b0a = 0;
                func_80048BF0_scus94236(&D_80109720[i]);
            }
            subScreenMapDrawRoutes();
        }
        break;
    }
}
#endif
