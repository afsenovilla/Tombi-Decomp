// VERSION SLPS_01144 dspsub3 800EBF08 852
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ea8f4 (dspsub3 copy; subScreenDrawItemGrid.c), from the USA one
typedef struct { char p[0x10]; short w10; char p12[2]; short page; short cursor; char p18[6]; short sel; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; } Text;
extern Slot D_800FE144[];
extern Text D_800FF57C;
extern int D_80011F0C;
extern unsigned short D_8009EE4C;
extern unsigned char D_800FF394[];
extern unsigned char D_8009EA9F, D_8009EBA0, D_8009EAA0, D_8009EBAE, D_8009EBA3, D_8009EE5B;
extern unsigned char D_8009E538[];
#define G30 D_8009E538
extern void subScreenDrawItemIcon(int n, int sel, int flag, int nocheck);

void func_800EBF08(Menu *m)
{
    Slot *s;
    for (s = D_800FE144; s->x != -1; s++) {
        short i;
        int flag, sel;
        D_800FF57C.x = s->x;
        D_800FF57C.y = s->y;
        D_800FF57C.img = D_80011F0C;
        D_800FF57C.w = m->w10 + 2;
        i = s->idx + m->page * 16;
        if (i >= D_8009EE4C) continue;
        sel = i == m->sel;
        flag = 0;
        switch (D_800FF394[i]) {
        case 10:
            if (D_8009EA9F == 1) flag = 1;
            break;
        case 12:
            if (D_8009EBA0 == 1) flag = 1;
            break;
        case 20:
            if (D_8009EAA0 == 1) flag = 1;
            break;
        case 24:
            if (D_8009EE5B == 2) flag = 1;
            break;
        case 28:
            if (D_8009EE5B == 3) flag = 1;
            break;
        case 29:
            if (D_8009EE5B == 1) flag = 1;
            break;
        case 32:
            if (D_8009EE5B == 0) flag = 1;
            break;
        case 98:
            if (D_8009E538[0] == 1) flag = 1;
            break;
        case 99:
            if (D_8009E538[0] == 2) flag = 1;
            break;
        case 100:
            if (D_8009E538[0] == 3) flag = 1;
            break;
        case 96:
            if (D_8009EBAE != 0) flag = 1;
            break;
        case 132:
            if (D_8009EBA3 != 0) flag = 1;
            break;
        case 27:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 0) flag = 1;
            break;
        case 33:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 8) flag = 1;
            break;
        case 34:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 9) flag = 1;
            break;
        case 21:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 5) flag = 1;
            break;
        case 22:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 6) flag = 1;
            break;
        case 23:
            if ((unsigned)(G30[0] - 1) >= 2 && G30[0x922] == 7) flag = 1;
            break;
        }
        subScreenDrawItemIcon(D_800FF394[i], sel, flag, m->cursor);
    }
}
#endif
