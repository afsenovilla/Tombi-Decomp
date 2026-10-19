// FUNC 800eaf28 544 D0SPSUB
// MATCHING 800eaf28 544
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[0x42]; unsigned short **icons; unsigned short **digits; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; unsigned short *img; } Text;
extern Slot D_80109C10[];
extern Text D_8011113C;
extern unsigned short *D_80111144;
extern void func_800EB2F4(int n);

void func_800EAF28(Menu *m, int n)
{
    Slot *s;
    int t;
    for (s = D_80109C10; s->x != -1; s++) {
        D_8011113C.x = s->x;
        D_8011113C.y = s->y;
        D_8011113C.w = m->w10 + s->dw;
        D_8011113C.attr = 0x7C15;
        switch (s->idx) {
        case -1:
            t = (m->page + 1) / 10;
            if (t != 0) {
                D_8011113C.img = m->digits[t];
                func_800EB2F4(1);
            }
            break;
        case -2:
            t = (m->page + 1) % 10;
            D_8011113C.img = m->digits[t];
            func_800EB2F4(1);
            break;
        case -3:
            t = n / 10;
            if (t != 0) {
                D_8011113C.img = m->digits[t];
                func_800EB2F4(1);
            }
            break;
        case -4:
            t = n % 10;
            D_8011113C.img = m->digits[t];
            func_800EB2F4(1);
            break;
        default:
            D_80111144 = m->icons[s->idx];
            func_800EB2F4(1);
            break;
        }
    }
}
