// FUNC 800efd6c 3012 D0SPSUB
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
extern Menu D_800A45D8;
extern Text D_8011113C;
extern short D_8011113Cx;
extern short D_8011113E;
extern short D_80111140;
extern unsigned short D_80111142;
extern int D_80111144;
extern int D_8011114C;
extern void * D_80111150;
extern Hud D_800B1410;
extern unsigned char D_8009C970, D_8009D2B1;
extern unsigned short D_8009C960, D_8009C962, D_8009D2A4;
extern short D_8010A058;
extern Slot D_8010922C[], D_80109208[], D_801091D0[], D_80109C70[], D_80109BF8[], D_80109C58[];
extern int D_801091C4[];
extern unsigned char D_80109228[];
extern int D_80108BC8[], D_80108B78[];
extern int *D_80108E5C[];
extern int D_800FE89C[];
extern char D_800FFD98[];
extern unsigned char D_801086F0[], D_8009CDA3[];
extern unsigned short D_8010A38A, D_8010A38C, D_8010A38E;
extern MATRIX D_1F8000C0;
extern SVECTOR D_1F800060, D_1F800068;
extern short D_80111100, D_801110F8, D_80111104, D_80111108;
extern Model D_8010A3A8;
extern char D_8010A47C[], D_8010A2D4[];
extern Sprite D_8010A550[];
extern int D_1F8002B4;
extern void func_800E9EA0(Menu *m);
extern void drawHudVitality(Hud *h, int x, int y, int v);
extern void drawHudItemGauge(Hud *h, int n, int x, int y, int f);
extern void func_800EB148(Menu *m, int n);
extern void func_800E9AB0(Menu *m);
extern void func_800EBCB8(int a, int b);
extern void func_800EB2F4(int n);
extern void func_800E9CB4(Hud *h, int x, int y);
extern void drawHudLives(Hud *h, int x, int y);
extern void func_800EBF34(Menu *m);
extern void func_800EA8F4(Menu *m);
extern void func_800EAC9C(Menu *m);
extern void func_800EAF28(Menu *m, int n);
extern void func_800EA168(Menu *m, int n, int c);
extern void func_800EA370(Menu *m);
extern void func_800EA5E8(Menu *m);
extern void func_800F0E0C(Menu *m, int a, int b);
extern void func_800F0F20(Menu *m, int a, int b);
extern void getBaseMatrix(MATRIX *m);
extern void RotMatrixZ(int r, MATRIX *m);
extern void RotMatrixX(int r, MATRIX *m);
extern void ApplyMatrixSV(MATRIX *m, SVECTOR *a, SVECTOR *b);
extern void drawObjectModelRotated(Model *o);
extern void drawObjectSprite(void *o);
extern void drawObjectSpriteScaled(Sprite *o);
extern int rsin(int a);
extern void func_800F0A5C(void);

void func_800EFD6C(void)
{
    Menu *m = &D_800A45D8;
    Slot *s;
    short end;
    int pages;
    int i, last;
    switch (m->mode) {
    case 0:
        D_800B1410.w48 = 1;
        func_800E9EA0(m);
        drawHudVitality(&D_800B1410, 0xC0, 0x70, D_8009C970);
        D_800B1410.b74 = 0;
        D_800B1410.b75 = 0;
        D_800B1410.b76 = 0;
        drawHudItemGauge(&D_800B1410, 0, 0xE, 0x5E, 1);
        drawHudItemGauge(&D_800B1410, 1, 0xE, 0x6E, 1);
        drawHudItemGauge(&D_800B1410, 2, 0xE, 0x7E, 1);
        func_800EB148(m, 0);
        func_800EB148(m, 1);
        func_800EB148(m, 2);
        func_800E9AB0(m);
        for (s = D_8010922C; s->x != -1; s++) {
            D_8011113Cx = s->x;
            D_8011113C.y = s->y;
            D_80111142 = 0x7C15;
            D_8011113C.w = m->w10 + s->dw;
            D_8011114C = D_801091C4[s->idx];
            D_8011113C.font = D_800FFD98;
            func_800EBCB8(5, 1);
        }
        D_8011113C.x = 0xE8;
        D_8011113C.y = 0x66;
        s = &D_80109208[D_8009D2B1];
        D_80111142 = 0x7892;
        D_8011113C.w = m->w10 + s->dw;
        D_80111144 = m->p54[D_80109228[D_8009D2B1]];
        func_800EB2F4(1);
        D_8011113Cx = s->x;
        D_8011113C.y = s->y;
        D_80111142 = 0x7C15;
        D_8011113C.w = m->w10 + s->dw;
        D_80111144 = m->p58[s->idx];
        func_800EB2F4(1);
        func_800E9CB4(&D_800B1410, 0x132, 0x50);
        drawHudLives(&D_800B1410, 0x120, 0x6A);
        D_8011113C.x = 0x10;
        D_8011113C.y = 0xC8;
        D_8011113C.w = m->w10 + 1;
        D_80111142 = 0x7C15;
        if (D_8009C960 == 9) D_8011114C = D_80108BC8[D_8009C962];
        else D_8011114C = D_80108B78[D_8009C960];
        D_8011113C.font = D_800FFD98;
        func_800EBCB8(5, 0);
        D_8011113C.x = 0x10;
        D_8011113C.y = 0xD8;
        D_8011113C.attr = 0x7C15;
        D_8011113C.w = m->w10 + 1;
        D_80111150 = D_800FFD98;
        D_8011114C = D_80108E5C[D_8009C960][D_8009C962];
        func_800EBCB8(5, 0);
        for (s = D_801091D0; s->x != -1; s++) {
            D_8011113Cx = s->x;
            D_8011113C.y = s->y;
            D_80111142 = 0x79D2;
            D_8011113C.w = m->w10 + s->dw;
            D_80111144 = m->p48[s->idx];
            func_800EB2F4(1);
        }
        D_80111142 = 0x7812;
        end = -1;
        for (s = D_80109C70; s->x != end; s++) {
            D_8011113C.x = s->x;
            D_8011113C.y = s->y;
            D_8011113C.w = m->w10 + s->dw;
            D_80111144 = D_800FE89C[s->idx];
            func_800EB2F4(3);
        }
        return;
    case 1:
        func_800E9EA0(m);
        if (D_8010A058 >= 0) func_800EBF34(m);
        pages = (D_8009D2A4 + 9) / 10;
        func_800EA8F4(m);
        func_800EAC9C(m);
        if (pages >= 2) {
            func_800EAF28(m, pages);
            s = D_80109BF8;
            while (s->x != -1) {
                if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                    s++;
                } else {
                    D_8011113C.x = s->x;
                    D_8011113C.y = s->y;
                    D_80111142 = 0x7C15;
                    D_8011113C.w = m->w10 + s->dw;
                    D_80111144 = m->p58[s->idx];
                    func_800EB2F4(1);
                    s++;
                }
            }
        }
        D_8011113C.attr = 0x7812;
        end = -1;
        for (s = D_80109C58; s->x != end; s++) {
            D_8011113C.x = s->x;
            D_8011113C.y = s->y;
            D_8011113C.w = m->w10 + s->dw;
            D_80111144 = D_800FE89C[s->idx];
            func_800EB2F4(3);
        }
        break;
    case 2:
        func_800E9EA0(m);
        {
        int j;
        last = 0;
        for (j = 0; j < 0x100; j++) {
            if (D_801086F0[j] != 0xFF && D_8009CDA3[D_801086F0[j]] != 0) last = j;
        }
        }
        pages = (last + 10) / 10;
        func_800EA168(m, last, 0xFF);
        func_800EA370(m);
        func_800EA5E8(m);
        if (pages >= 2) {
            func_800EAF28(m, pages);
            s = D_80109BF8;
            while (s->x != -1) {
                if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                    s++;
                } else {
                    D_8011113C.x = s->x;
                    D_8011113C.y = s->y;
                    D_80111142 = 0x7C15;
                    D_8011113C.w = m->w10 + s->dw;
                    D_80111144 = m->p58[s->idx];
                    func_800EB2F4(1);
                    s++;
                }
            }
        }
        D_8011113C.attr = 0x7812;
        end = -1;
        for (s = D_80109C58; s->x != end; s++) {
            D_8011113C.x = s->x;
            D_8011113C.y = s->y;
            D_8011113C.w = m->w10 + s->dw;
            D_8011113C.img = D_800FE89C[s->idx];
            func_800EB2F4(3);
        }
        break;
    case 3:
        func_800E9EA0(m);
        D_80111142 = 0x7812;
        end = -1;
        for (s = D_80109C70; s->x != end; s++) {
            D_8011113C.x = s->x;
            D_8011113E = s->y;
            D_8011113C.w = m->w10 + s->dw;
            D_8011113C.img = D_800FE89C[s->idx];
            func_800EB2F4(3);
        }
        if (D_8010A38E != 0) {
            func_800F0E0C(m, D_8010A38A, D_8010A38C);
            func_800F0F20(m, D_8010A38A, D_8010A38C);
        }
        if (m->b1 != 0) {
            getBaseMatrix(&D_1F8000C0);
            RotMatrixZ(D_80111100, &D_1F8000C0);
            RotMatrixX(D_801110F8, &D_1F8000C0);
            D_1F8000C0.t[0] = 0;
            D_1F800060.vx = 0;
            D_1F800060.vy = 0;
            D_1F800060.vz = 0;
            D_1F8000C0.t[1] = D_80111104;
            D_1F8000C0.t[2] = D_80111108 + 1000;
            ApplyMatrixSV(&D_1F8000C0, &D_1F800060, &D_1F800068);
            D_8010A3A8.rx = 0;
            D_8010A3A8.ry = -1000;
            D_8010A3A8.rz = 0;
            D_1F8000C0.t[0] += D_1F800068.vx;
            D_1F8000C0.t[1] += D_1F800068.vy;
            D_1F8000C0.t[2] += D_1F800068.vz;
            drawObjectModelRotated(&D_8010A3A8);
            drawObjectSprite(D_8010A47C);
            drawObjectSprite(D_8010A2D4);
            for (i = 0; i < 0x80; i++) {
                D_1F8002B4 = (rsin(D_801110F8 - 0x1AA) - D_8010A550[i].zi) * 3 + 0x1000;
                D_8010A550[i].b0a = 0;
                drawObjectSpriteScaled(&D_8010A550[i]);
            }
            func_800F0A5C();
        }
        break;
    }
}
