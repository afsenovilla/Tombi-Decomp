// FUNC 800efd6c 3012 D0SPSUB
// MATCHING 800efd6c 3012
// Matched 2026-11-09 (was score 185). Every block writes the Text globals at 0x8011113C in the same order: x, y, w,
// attr, then str/img, then font. Three store forms, which sched and cse treat differently:
// - `SUBSCREEN_DRAW_PARAMS.field`: in-struct (keeps its place against the `s->` loads); cse rewrites a later field of the same
//   symbol in the block as base+offset, so the first field written becomes the base register: loop 1 writes only
//   attr and font this way (s2 = &attr, font at 14(s2)); `x = 0xE8` makes s2 = &x for `x = 0x10`. Where cse2 knows
//   the base it folds the address back to an absolute store.
// - one-field-struct aliases Xs/Ys/Ws (asm names of the real symbols): in-struct, but another symbol (no base).
// - plain externs SUBSCREEN_DRAW_TPAGE/42/44/4C: not in struct, never ordered against the `s->` loads, so sched moves them.
// Loop 2 (D_801091D0) stores attr through the plain extern: a struct form adds an address insn, loop.c then counts
// 30 insns (over its threshold of 29 for a loop with a call) and stops hoisting 0x79D2 into s1.
// Case 2: the 0xFF of the SUBSCREEN_EVENT_ORDER scan is a variable, so subScreenDrawEventIcons(m, last, ff) reuses its a2.
// HUD_STATE: the three bytes cleared in case 0 are +0x64..0x66 (the wip draft had +0x74, past the 0x68-byte block;
// the byte compare masks relocated offsets, tools/ncheck.py cannot see it).
#ifndef PORT_NTSC
#define PORT_NTSC(pal, us) (pal)   /* port2 (port_prelude.h): the USA value with the NTSC picture */
#endif
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
extern Menu MENU_STATE;
extern Text SUBSCREEN_DRAW_PARAMS;
extern short SUBSCREEN_DRAW_TPAGE;
extern unsigned short SUBSCREEN_DRAW_CLUT;
extern int SUBSCREEN_DRAW_FRAME;
extern int SUBSCREEN_DRAW_STR;
typedef struct { short v; } S16;
extern S16 Xs asm("SUBSCREEN_DRAW_PARAMS"), Ys asm("SUBSCREEN_DRAW_Y"), Ws asm("SUBSCREEN_DRAW_TPAGE");
extern Hud HUD_STATE;
extern unsigned char GAME_PLAYER_HEALTH, GAME_BITTEN_JUMP_KIND;
extern unsigned short GAME, GAME_SELECTED_SECTION, D_8009D2A4;
extern short SUBSCREEN_POPUP_CURSOR;
extern Slot D_8010922C[], D_80109208[], D_801091D0[], D_80109C70[], D_80109BF8[], D_80109C58[];
extern int D_801091C4[];
extern unsigned char D_80109228[];
extern int D_80108BC8[], MAP_AREA14_AREA_NAMES[];
extern int *D_80108E5C[];
extern int D_800FE89C[];
extern char SUBSCREEN_FONT[];
extern unsigned char SUBSCREEN_EVENT_ORDER[], GAME_INVENTORY_SCREEN[];
extern unsigned short D_8010A38A, D_8010A38C, D_8010A38E;
extern MATRIX VIEW_CAMERA;
extern SVECTOR WORK_VEC_A, WORK_VEC_B;
extern short SUBSCREEN_MAP_VIEW_Y, SUBSCREEN_MAP_VIEW_X, SUBSCREEN_MAP_CAMERA_Y, SUBSCREEN_MAP_CAMERA_Z;
extern Model D_8010A3A8;
extern char D_8010A47C[], SUBSCREEN_MAP_OBJECTS[];
extern Sprite SUBSCREEN_MAP_AREA_SPRITES[];
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
        drawHudVitality(&HUD_STATE, 0xC0, PORT_NTSC(0x70, 0x60), GAME_PLAYER_HEALTH);
        HUD_STATE.b64 = 0;
        HUD_STATE.b65 = 0;
        HUD_STATE.b66 = 0;
        drawHudItemGauge(&HUD_STATE, 0, 0xE, PORT_NTSC(0x5E, 0x4E), 1);
        drawHudItemGauge(&HUD_STATE, 1, 0xE, PORT_NTSC(0x6E, 0x5E), 1);
        drawHudItemGauge(&HUD_STATE, 2, 0xE, PORT_NTSC(0x7E, 0x6E), 1);
        subScreenDrawGaugePercent(m, 0);
        subScreenDrawGaugePercent(m, 1);
        subScreenDrawGaugePercent(m, 2);
        subScreenDrawStatusNotes(m);
        {
        Slot *s;
        for (s = D_8010922C; s->x != -1; s++) {
            Xs.v = s->x;
            Ys.v = s->y - PORT_NTSC(0, 16);
            Ws.v = m->w10 + s->dw;
            SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
            SUBSCREEN_DRAW_STR = D_801091C4[s->idx];
            SUBSCREEN_DRAW_PARAMS.font = SUBSCREEN_FONT;
            subScreenDrawString(5, 1);
        }
        }
        SUBSCREEN_DRAW_PARAMS.x = 0xE8;
        Ys.v = PORT_NTSC(0x66, 0x56);
        s = &D_80109208[GAME_BITTEN_JUMP_KIND];
        SUBSCREEN_DRAW_TPAGE = m->w10 + s->dw;
        SUBSCREEN_DRAW_CLUT = 0x7892;
        SUBSCREEN_DRAW_FRAME = m->p54[D_80109228[GAME_BITTEN_JUMP_KIND]];
        subScreenDrawSprite(1);
        Xs.v = s->x;
        SUBSCREEN_DRAW_PARAMS.y = s->y - PORT_NTSC(0, 16);
        SUBSCREEN_DRAW_PARAMS.w = m->w10 + s->dw;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_FRAME = m->p58[s->idx];
        subScreenDrawSprite(1);
        subScreenDrawDigitsSprite(&HUD_STATE, 0x132, PORT_NTSC(0x50, 0x40));
        drawHudLives(&HUD_STATE, 0x120, PORT_NTSC(0x6A, 0x5A));
        SUBSCREEN_DRAW_PARAMS.x = 0x10;
        SUBSCREEN_DRAW_PARAMS.y = PORT_NTSC(0xC8, 0xB8);
        Ws.v = m->w10 + 1;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        if (GAME == 9) SUBSCREEN_DRAW_STR = D_80108BC8[GAME_SELECTED_SECTION];
        else SUBSCREEN_DRAW_STR = MAP_AREA14_AREA_NAMES[GAME];
        SUBSCREEN_DRAW_PARAMS.font = SUBSCREEN_FONT;
        subScreenDrawString(5, 0);
        SUBSCREEN_DRAW_PARAMS.x = 0x10;
        SUBSCREEN_DRAW_PARAMS.y = PORT_NTSC(0xD8, 0xC8);
        Ws.v = m->w10 + 1;
        SUBSCREEN_DRAW_PARAMS.attr = 0x7C15;
        SUBSCREEN_DRAW_STR = D_80108E5C[GAME][GAME_SELECTED_SECTION];
        SUBSCREEN_DRAW_PARAMS.font = SUBSCREEN_FONT;
        subScreenDrawString(5, 0);
        {
        Slot *s;
        for (s = D_801091D0; s->x != -1; s++) {
            Xs.v = s->x;
            Ys.v = s->y - PORT_NTSC(0, 16);
            Ws.v = m->w10 + s->dw;
            SUBSCREEN_DRAW_CLUT = 0x79D2;
            SUBSCREEN_DRAW_FRAME = m->p48[s->idx];
            subScreenDrawSprite(1);
        }
        }
        SUBSCREEN_DRAW_CLUT = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80109C70; s->x != end; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            SUBSCREEN_DRAW_FRAME = D_800FE89C[s->idx];
            subScreenDrawSprite(3);
        }
        }
        return;
    case 1:
        subScreenDrawTabs(m);
        if (SUBSCREEN_POPUP_CURSOR >= 0) subScreenDrawPopupMenu(m);
        pages = (D_8009D2A4 + PORT_NTSC(9, 7)) / PORT_NTSC(10, 8);
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
                    Xs.v = s->x;
                    Ys.v = s->y;
                    Ws.v = m->w10 + s->dw;
                    SUBSCREEN_DRAW_CLUT = 0x7C15;
                    SUBSCREEN_DRAW_FRAME = m->p58[s->idx];
                    subScreenDrawSprite(1);
                    s++;
                }
            }
            }
        }
        SUBSCREEN_DRAW_CLUT = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80109C58; s->x != end; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            SUBSCREEN_DRAW_FRAME = D_800FE89C[s->idx];
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
            if (SUBSCREEN_EVENT_ORDER[j] != ff && GAME_INVENTORY_SCREEN[SUBSCREEN_EVENT_ORDER[j]] != 0) last = j;
        }
        pages = (last + PORT_NTSC(10, 8)) / PORT_NTSC(10, 8);
        subScreenDrawEventIcons(m, last, ff);
        }
        subScreenDrawEventList(m);
        subScreenDrawEventCounters(m);
        if (pages >= 2) {
            subScreenDrawPageNumber(m, pages);
            {
            Slot *s;
            s = D_80109BF8;
            while (s->x != -1) {
                if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                    s++;
                } else {
                    Xs.v = s->x;
                    Ys.v = s->y;
                    Ws.v = m->w10 + s->dw;
                    SUBSCREEN_DRAW_CLUT = 0x7C15;
                    SUBSCREEN_DRAW_FRAME = m->p58[s->idx];
                    subScreenDrawSprite(1);
                    s++;
                }
            }
            }
        }
        SUBSCREEN_DRAW_CLUT = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80109C58; s->x != end; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            SUBSCREEN_DRAW_FRAME = D_800FE89C[s->idx];
            subScreenDrawSprite(3);
        }
        }
        break;
    case 3:
        subScreenDrawTabs(m);
        SUBSCREEN_DRAW_CLUT = 0x7812;
        end = -1;
        {
        Slot *s;
        for (s = D_80109C70; s->x != end; s++) {
            Xs.v = s->x;
            Ys.v = s->y;
            Ws.v = m->w10 + s->dw;
            SUBSCREEN_DRAW_FRAME = D_800FE89C[s->idx];
            subScreenDrawSprite(3);
        }
        }
        if (D_8010A38E != 0) {
            subScreenMapDrawAreaName(m, D_8010A38A, D_8010A38C);
            subScreenMapDrawRoomName(m, D_8010A38A, D_8010A38C);
        }
        if (m->b1 != 0) {
            getBaseMatrix(&VIEW_CAMERA);
            RotMatrixZ(SUBSCREEN_MAP_VIEW_Y, &VIEW_CAMERA);
            RotMatrixX(SUBSCREEN_MAP_VIEW_X, &VIEW_CAMERA);
            VIEW_CAMERA.t[0] = 0;
            WORK_VEC_A.vx = 0;
            WORK_VEC_A.vy = 0;
            WORK_VEC_A.vz = 0;
            VIEW_CAMERA.t[1] = SUBSCREEN_MAP_CAMERA_Y;
            VIEW_CAMERA.t[2] = SUBSCREEN_MAP_CAMERA_Z + 1000;
            ApplyMatrixSV(&VIEW_CAMERA, &WORK_VEC_A, &WORK_VEC_B);
            D_8010A3A8.rx = 0;
            D_8010A3A8.ry = -1000;
            D_8010A3A8.rz = 0;
            VIEW_CAMERA.t[0] += WORK_VEC_B.vx;
            VIEW_CAMERA.t[1] += WORK_VEC_B.vy;
            VIEW_CAMERA.t[2] += WORK_VEC_B.vz;
            drawObjectModelRotated(&D_8010A3A8);
            drawObjectSprite(D_8010A47C);
            drawObjectSprite(SUBSCREEN_MAP_OBJECTS);
            for (i = 0; i < 0x80; i++) {
                D_1F8002B4 = (rsin(SUBSCREEN_MAP_VIEW_X - 0x1AA) - SUBSCREEN_MAP_AREA_SPRITES[i].zi) * 3 + 0x1000;
                SUBSCREEN_MAP_AREA_SPRITES[i].b0a = 0;
                drawObjectSpriteScaled(&SUBSCREEN_MAP_AREA_SPRITES[i]);
            }
            subScreenMapDrawRoutes();
        }
        break;
    }
}
