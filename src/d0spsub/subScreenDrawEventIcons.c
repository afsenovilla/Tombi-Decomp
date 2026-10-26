// FUNC 800ea168 520 D0SPSUB
// MATCHING 800ea168 520
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[0x4e]; unsigned short **icons; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; } Text;
extern Slot D_801087F0[];
extern unsigned char D_801086F0[];
extern Text D_8011113C;
extern short D_80111142;
extern unsigned short *D_80111144;
extern unsigned char SCRIPT_FLAGS[];
extern void subScreenDrawSprite(int n);

void subScreenDrawEventIcons(Menu *m)
{
    Slot *s;
    for (s = D_801087F0; s->x != -1; s++) {
        int id;
        short k;
        D_8011113C.x = s->x;
        D_8011113C.y = s->y;
        D_8011113C.w = m->w10 + s->dw + 2;
        id = D_801086F0[m->page * 10 + s->idx];
        k = id - 1;
        if (id - 1 >= 0xFF) continue;
        switch (id) {
        case 0x57: D_80111142 = 0x7D12; break;
        case 0x58: D_80111142 = 0x7D52; break;
        case 0x59: D_80111142 = 0x7D92; break;
        case 0x75: D_80111142 = 0x7DD2; break;
        case 0x98: D_80111142 = 0x7A93; break;
        case 0x9E: D_80111142 = 0x7AD3; break;
        case 0xA0: D_80111142 = 0x7B13; break;
        case 0xA5: D_80111142 = 0x7B53; break;
        default: D_80111142 = 0; break;
        }
        if (k < 0) k = 0;
        if (SCRIPT_FLAGS[k] == 0xFF) {
            D_80111144 = m->icons[k];
            subScreenDrawSprite(1);
        }
    }
}
