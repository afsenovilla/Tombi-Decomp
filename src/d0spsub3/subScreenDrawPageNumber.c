// FUNC 800eae80 544 D0SPSUB3
// MATCHING 800eae80 544
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[0x42]; unsigned short **icons; unsigned short **digits; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; unsigned short *img; } Text;
extern Slot D_80100774[];
extern Text D_80100B70;
extern unsigned short *D_80100B78;
extern void subScreenDrawSprite(int n);

void subScreenDrawPageNumber(Menu *m, int n)
{
    Slot *s;
    int t;
    for (s = D_80100774; s->x != -1; s++) {
        D_80100B70.x = s->x;
        D_80100B70.y = s->y;
        D_80100B70.w = m->w10 + s->dw;
        D_80100B70.attr = 0x7C15;
        switch (s->idx) {
        case -1:
            t = (m->page + 1) / 10;
            if (t != 0) {
                D_80100B70.img = m->digits[t];
                subScreenDrawSprite(1);
            }
            break;
        case -2:
            t = (m->page + 1) % 10;
            D_80100B70.img = m->digits[t];
            subScreenDrawSprite(1);
            break;
        case -3:
            t = n / 10;
            if (t != 0) {
                D_80100B70.img = m->digits[t];
                subScreenDrawSprite(1);
            }
            break;
        case -4:
            t = n % 10;
            D_80100B70.img = m->digits[t];
            subScreenDrawSprite(1);
            break;
        default:
            D_80100B78 = m->icons[s->idx];
            subScreenDrawSprite(1);
            break;
        }
    }
}
