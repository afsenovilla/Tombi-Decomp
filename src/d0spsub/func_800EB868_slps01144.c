// VERSION SLPS_01144 dspsub 800EB868 496
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ea168 (subScreenDrawEventIcons.c), from the USA one
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[0x4e]; unsigned short **icons; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; } Text;
extern Slot D_80108E0C[];
extern unsigned char D_80108D0C[];
extern Text D_80111408;
extern short D_8011140E;
extern unsigned short *D_80111410;
extern unsigned char SCRIPT_FLAGS[];
extern void func_800EC9D8(int n);

void func_800EB868(Menu *m)
{
    Slot *s;
    for (s = D_80108E0C; s->x != -1; s++) {
        int id;
        short k;
        D_80111408.x = s->x;
        D_80111408.y = s->y;
        D_80111408.w = m->w10 + s->dw + 2;
        id = D_80108D0C[m->page * 16 + s->idx];
        k = id - 1;
        if (id - 1 >= 0xFF) continue;
        switch (id) {
        case 0x57: D_8011140E = 0x7D12; break;
        case 0x58: D_8011140E = 0x7D52; break;
        case 0x59: D_8011140E = 0x7D92; break;
        case 0x75: D_8011140E = 0x7DD2; break;
        case 0x98: D_8011140E = 0x7A93; break;
        case 0x9E: D_8011140E = 0x7AD3; break;
        case 0xA0: D_8011140E = 0x7B13; break;
        case 0xA5: D_8011140E = 0x7B53; break;
        default: D_8011140E = 0; break;
        }
        if (k < 0) k = 0;
        if (SCRIPT_FLAGS[k] == 0xFF) {
            D_80111410 = m->icons[k];
            func_800EC9D8(1);
        }
    }
}
#endif
