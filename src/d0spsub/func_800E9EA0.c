// FUNC 800e9ea0 712 D0SPSUB
// MATCHING 800e9ea0 712
typedef struct { char p[0x10]; short w10; short mode; char p14[2]; short cursor; char p18[0x30]; int *p48; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; } Text;
extern Slot D_8010924C[], D_801092C4[], D_80109294[], D_801092F4[];
extern short D_80109304[][12];
extern int D_800FE8D0[];
extern Text D_8011113C;
extern short D_80111142;
extern int D_80111144;
extern void func_800EB2F4(int n);
extern void func_800EB4E8(int n);

void func_800E9EA0(Menu *m)
{
    Slot *s;
    switch (m->mode) {
    case 0:
    case 1:
    case 2:
        s = D_8010924C;
        break;
    case 3:
        s = D_801092C4;
        break;
    case 7:
        s = D_80109294;
        break;
    }
    for (; s->x != -1; s++) {
        short id;
        int idx, mode;
        D_8011113C.x = s->x;
        D_8011113C.y = s->y;
        D_8011113C.w = m->w10 + s->dw;
        mode = m->mode;
        idx = s->idx;
        id = D_80109304[mode][idx];
        if (id != -1) {
            if (idx < 4) {
                if (idx == m->cursor) {
                    D_8011113C.attr = -0x6EE;
                    D_8011113C.img = m->p48[7];
                    func_800EB2F4(2);
                    D_8011113C.attr = 0x7BD2;
                } else {
                    D_8011113C.attr = 0x7912;
                }
            } else if (idx == m->cursor) {
                D_8011113C.attr = 0x7F52;
            } else if ((idx == 4 && mode == 1) || (idx == 5 && mode == 2) || (idx == 6 && mode == 0) ||
                       (idx == 7 && mode == 3) || (idx == 4 && mode == 7)) {
                D_80111142 = 0x7F92;
            } else {
                D_80111142 = 0x7852;
            }
            if (s->x == -2) {
                s = D_801092F4;
                D_8011113C.x = s->x;
                D_8011113C.y = s->y;
                D_8011113C.w = m->w10 + s->dw;
                D_8011113C.img = D_800FE8D0[0];
                func_800EB4E8(1);
                s++;
                D_8011113C.x = s->x;
                D_8011113C.y = s->y;
                D_8011113C.w = m->w10 + s->dw;
                D_8011113C.img = D_800FE8D0[1];
                func_800EB4E8(1);
            } else {
                D_80111144 = m->p48[id];
                func_800EB2F4(1);
            }
        }
    }
}
