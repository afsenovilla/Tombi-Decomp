// FUNC 800efd6c 3012 D0SPSUB
// wip: score 786 (the old draft scored 529 with the WRONG frame: it saved s0-s5 in a 0x98 frame; now the prologue,
// the 0x90 frame, s0-s4 and the first 0x138 bytes match, so the number is higher only because every later
// register name shifted). What fixed the frame: (1) the Text struct D_8011113C must only be used for the fields the
// game addresses through a base register (x at +0 for the three `x = 0xE8/0x10` stores after loop 1, attr at +6
// and font at +0x14 inside loop 1: `sh v0,(s2); sw v0,0xe(s2)` is cse's related-value rewrite of font from the attr
// address); y, w, img, D_8011114C are separate externs (D_8011113E, D_80111140, D_80111144, D_8011114C), otherwise
// gcc keeps the struct base in an extra s-reg (s5) for the loops; (2) every loop's `Slot *s` is block-scoped (the
// game uses s1/s2/s0 per loop), the function-scope `s` is only for the non-loop `&D_80109208[GAME_BITTEN_JUMP_KIND]` use.
// Left: inside the loops the game's RTL order differs (loop 1: the x store precedes the y load, the attr store sits
// between the w loads and the w store: try x/y/w/attr orders and the plain-vs-volatile attr form, 786-792 so far),
// then the usual per-loop register names. The hoisted -1 (`end`) goes to s2 in the game's loops 2+ (not in loop 1).
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
typedef struct { char p0[0x48]; short w48; char p4a[0x2a]; unsigned char b74, b75, b76; } Hud;
typedef struct { char p0[0x12]; short rx; short p14; short ry; short p18; short rz; } Model;
extern Menu MENU_STATE;
extern Text D_8011113C;
extern short D_8011113Cx;
extern short D_8011113E;
extern short D_80111140;
extern unsigned short D_80111142;
extern int D_80111144;
extern int D_8011114C;
extern void * D_80111150;
extern Hud HUD_STATE;
extern unsigned char GAME_PLAYER_HEALTH, GAME_BITTEN_JUMP_KIND;
extern unsigned short GAME, GAME_SELECTED_SECTION, D_8009D2A4;
extern short D_8010A058;
extern Slot D_8010922C[], D_80109208[], D_801091D0[], D_80109C70[], D_80109BF8[], D_80109C58[];
extern int D_801091C4[];
extern unsigned char D_80109228[];
extern int D_80108BC8[], D_80108B78[];
extern int *D_80108E5C[];
extern int D_800FE89C[];
extern char D_800FFD98[];
extern unsigned char D_801086F0[], GAME_INVENTORY_SCREEN[];
extern unsigned short D_8010A38A, D_8010A38C, D_8010A38E;
extern MATRIX VIEW_CAMERA;
extern SVECTOR D_1F800060, D_1F800068;
extern short D_80111100, D_801110F8, D_80111104, D_80111108;
extern Model D_8010A3A8;
extern char D_8010A47C[], D_8010A2D4[];
extern Sprite D_8010A550[];
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
extern void func_800EA5E8(Menu *m);
extern void subScreenMapDrawAreaName(Menu *m, int a, int b);
extern void subScreenMapDrawRoomName(Menu *m, int a, int b);
extern void getBaseMatrix(MATRIX *m);
extern void RotMatrixZ(int r, MATRIX *m);
extern void RotMatrixX(int r, MATRIX *m);
extern void ApplyMatrixSV(MATRIX *m, SVECTOR *a, SVECTOR *b);
extern void drawObjectModelRotated(Model *o);
extern void drawObjectSprite(void *o);
extern void drawObjectSpriteScaled(Sprite *o);
extern int rsin(int a);
extern void subScreenMapDrawRoutes(void);

void func_800EFD6C(void)
{
    Menu *m = &MENU_STATE;
    Slot *s;
    short end;
    int pages;
    int i, last;
    switch (m->mode) {
    case 0:
        HUD_STATE.w48 = 1;
        subScreenDrawTabs(m);
        drawHudVitality(&HUD_STATE, 0xC0, 0x70, GAME_PLAYER_HEALTH);
        HUD_STATE.b74 = 0;
        HUD_STATE.b75 = 0;
        HUD_STATE.b76 = 0;
        drawHudItemGauge(&HUD_STATE, 0, 0xE, 0x5E, 1);
        drawHudItemGauge(&HUD_STATE, 1, 0xE, 0x6E, 1);
        drawHudItemGauge(&HUD_STATE, 2, 0xE, 0x7E, 1);
        subScreenDrawGaugePercent(m, 0);
        subScreenDrawGaugePercent(m, 1);
        subScreenDrawGaugePercent(m, 2);
        subScreenDrawStatusNotes(m);
        {
        Slot *s;
        for (s = D_8010922C; s->x != -1; s++) {
            D_8011113Cx = s->x;
            D_8011113E = s->y;
            *(volatile unsigned short *)&D_8011113C.attr = 0x7C15;
            D_80111140 = m->w10 + s->dw;
            D_8011113C.font = D_800FFD98;
            D_8011114C = D_801091C4[s->idx];
            subScreenDrawString(5, 1);
        }
        }
        D_8011113C.x = 0xE8;
        D_8011113E = 0x66;
        s = &D_80109208[GAME_BITTEN_JUMP_KIND];
        D_80111142 = 0x7892;
        D_80111140 = m->w10 + s->dw;
        D_80111144 = m->p54[D_80109228[GAME_BITTEN_JUMP_KIND]];
        subScreenDrawSprite(1);
        D_8011113Cx = s->x;
        D_8011113E = s->y;
        D_80111142 = 0x7C15;
        D_80111140 = m->w10 + s->dw;
        D_80111144 = m->p58[s->idx];
        subScreenDrawSprite(1);
        subScreenDrawDigitsSprite(&HUD_STATE, 0x132, 0x50);
        drawHudLives(&HUD_STATE, 0x120, 0x6A);
        D_8011113C.x = 0x10;
        D_8011113E = 0xC8;
        D_80111140 = m->w10 + 1;
        D_80111142 = 0x7C15;
        if (GAME == 9) D_8011114C = D_80108BC8[GAME_SELECTED_SECTION];
        else D_8011114C = D_80108B78[GAME];
        D_8011113C.font = D_800FFD98;
        subScreenDrawString(5, 0);
        D_8011113C.x = 0x10;
        D_8011113E = 0xD8;
        D_8011113C.attr = 0x7C15;
        D_80111140 = m->w10 + 1;
        D_80111150 = D_800FFD98;
        D_8011114C = D_80108E5C[GAME][GAME_SELECTED_SECTION];
        subScreenDrawString(5, 0);
        {
        Slot *s;
        for (s = D_801091D0; s->x != -1; s++) {
            D_8011113Cx = s->x;
            D_8011113E = s->y;
            D_8011113C.attr = 0x79D2;
            D_80111140 = m->w10 + s->dw;
            D_80111144 = m->p48[s->idx];
            subScreenDrawSprite(1);
        }
        }
        D_80111142 = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80109C70; s->x != end; s++) {
            D_8011113C.x = s->x;
            D_8011113E = s->y;
            D_80111140 = m->w10 + s->dw;
            D_80111144 = D_800FE89C[s->idx];
            subScreenDrawSprite(3);
        }
        }
        return;
    case 1:
        subScreenDrawTabs(m);
        if (D_8010A058 >= 0) subScreenDrawPopupMenu(m);
        pages = (D_8009D2A4 + 9) / 10;
        subScreenDrawItemGrid(m);
        subScreenDrawItemNames(m);
        if (pages >= 2) {
            subScreenDrawPageNumber(m, pages);
            {
            Slot *s;
            s = D_80109BF8;
            while (s->x != -1) {
                if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                    s++;
                } else {
                    D_8011113C.x = s->x;
                    D_8011113E = s->y;
                    D_8011113C.attr = 0x7C15;
                    D_80111140 = m->w10 + s->dw;
                    D_80111144 = m->p58[s->idx];
                    subScreenDrawSprite(1);
                    s++;
                }
            }
            }
        }
        D_80111142 = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80109C58; s->x != end; s++) {
            D_8011113C.x = s->x;
            D_8011113E = s->y;
            D_80111140 = m->w10 + s->dw;
            D_80111144 = D_800FE89C[s->idx];
            subScreenDrawSprite(3);
        }
        }
        break;
    case 2:
        subScreenDrawTabs(m);
        {
        int j;
        last = 0;
        for (j = 0; j < 0x100; j++) {
            if (D_801086F0[j] != 0xFF && GAME_INVENTORY_SCREEN[D_801086F0[j]] != 0) last = j;
        }
        }
        pages = (last + 10) / 10;
        subScreenDrawEventIcons(m, last, 0xFF);
        subScreenDrawEventList(m);
        func_800EA5E8(m);
        if (pages >= 2) {
            subScreenDrawPageNumber(m, pages);
            {
            Slot *s;
            s = D_80109BF8;
            while (s->x != -1) {
                if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                    s++;
                } else {
                    D_8011113C.x = s->x;
                    D_8011113E = s->y;
                    D_8011113C.attr = 0x7C15;
                    D_80111140 = m->w10 + s->dw;
                    D_80111144 = m->p58[s->idx];
                    subScreenDrawSprite(1);
                    s++;
                }
            }
            }
        }
        D_80111142 = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80109C58; s->x != end; s++) {
            D_8011113C.x = s->x;
            D_8011113E = s->y;
            D_80111140 = m->w10 + s->dw;
            D_80111144 = D_800FE89C[s->idx];
            subScreenDrawSprite(3);
        }
        }
        break;
    case 3:
        subScreenDrawTabs(m);
        D_80111142 = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80109C70; s->x != end; s++) {
            D_8011113C.x = s->x;
            D_8011113E = s->y;
            D_80111140 = m->w10 + s->dw;
            D_80111144 = D_800FE89C[s->idx];
            subScreenDrawSprite(3);
        }
        }
        if (D_8010A38E != 0) {
            subScreenMapDrawAreaName(m, D_8010A38A, D_8010A38C);
            subScreenMapDrawRoomName(m, D_8010A38A, D_8010A38C);
        }
        if (m->b1 != 0) {
            getBaseMatrix(&VIEW_CAMERA);
            RotMatrixZ(D_80111100, &VIEW_CAMERA);
            RotMatrixX(D_801110F8, &VIEW_CAMERA);
            VIEW_CAMERA.t[0] = 0;
            D_1F800060.vx = 0;
            D_1F800060.vy = 0;
            D_1F800060.vz = 0;
            VIEW_CAMERA.t[1] = D_80111104;
            VIEW_CAMERA.t[2] = D_80111108 + 1000;
            ApplyMatrixSV(&VIEW_CAMERA, &D_1F800060, &D_1F800068);
            D_8010A3A8.rx = 0;
            D_8010A3A8.ry = -1000;
            D_8010A3A8.rz = 0;
            VIEW_CAMERA.t[0] += D_1F800068.vx;
            VIEW_CAMERA.t[1] += D_1F800068.vy;
            VIEW_CAMERA.t[2] += D_1F800068.vz;
            drawObjectModelRotated(&D_8010A3A8);
            drawObjectSprite(D_8010A47C);
            drawObjectSprite(D_8010A2D4);
            for (i = 0; i < 0x80; i++) {
                D_1F8002B4 = (rsin(D_801110F8 - 0x1AA) - D_8010A550[i].zi) * 3 + 0x1000;
                D_8010A550[i].b0a = 0;
                drawObjectSpriteScaled(&D_8010A550[i]);
            }
            subScreenMapDrawRoutes();
        }
        break;
    }
}
