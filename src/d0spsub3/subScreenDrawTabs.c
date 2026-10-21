// FUNC 800e9df8 712 D0SPSUB3
// MATCHING 800e9df8 712
typedef struct { char p[0x10]; short w10; short mode; char p14[2]; short cursor; char p18[0x30]; int *p48; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; } Text;
extern Slot D_800FFDB0[], D_800FFE28[], D_800FFDF8[], D_800FFE58[];
extern short D_800FFE68[][12];
extern int D_800F5434[];
extern Text D_80100B70;
extern short D_80100B76;
extern int D_80100B78;
extern void subScreenDrawSprite(int n);
extern void subScreenDrawSpriteFT4(int n);

void subScreenDrawTabs(Menu *m)
{
    Slot *s;
    switch (m->mode) {
    case 0:
    case 1:
    case 2:
        s = D_800FFDB0;
        break;
    case 3:
        s = D_800FFE28;
        break;
    case 7:
        s = D_800FFDF8;
        break;
    }
    for (; s->x != -1; s++) {
        short id;
        int idx, mode;
        D_80100B70.x = s->x;
        D_80100B70.y = s->y;
        D_80100B70.w = m->w10 + s->dw;
        mode = m->mode;
        idx = s->idx;
        id = D_800FFE68[mode][idx];
        if (id != -1) {
            if (idx < 4) {
                if (idx == m->cursor) {
                    D_80100B70.attr = -0x6EE;
                    D_80100B70.img = m->p48[7];
                    subScreenDrawSprite(2);
                    D_80100B70.attr = 0x7BD2;
                } else {
                    D_80100B70.attr = 0x7912;
                }
            } else if (idx == m->cursor) {
                D_80100B70.attr = 0x7F52;
            } else if ((idx == 4 && mode == 1) || (idx == 5 && mode == 2) || (idx == 6 && mode == 0) ||
                       (idx == 7 && mode == 3) || (idx == 4 && mode == 7)) {
                D_80100B76 = 0x7F92;
            } else {
                D_80100B76 = 0x7852;
            }
            if (s->x == -2) {
                s = D_800FFE58;
                D_80100B70.x = s->x;
                D_80100B70.y = s->y;
                D_80100B70.w = m->w10 + s->dw;
                D_80100B70.img = D_800F5434[0];
                subScreenDrawSpriteFT4(1);
                s++;
                D_80100B70.x = s->x;
                D_80100B70.y = s->y;
                D_80100B70.w = m->w10 + s->dw;
                D_80100B70.img = D_800F5434[1];
                subScreenDrawSpriteFT4(1);
            } else {
                D_80100B78 = m->p48[id];
                subScreenDrawSprite(1);
            }
        }
    }
}
