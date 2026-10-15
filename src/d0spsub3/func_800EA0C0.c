// FUNC 800ea0c0 520 D0SPSUB3
// MATCHING 800ea0c0 520
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[0x4e]; unsigned short **icons; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; } Text;
extern Slot D_800FF354[];
extern unsigned char D_800FF254[];
extern Text D_80100B70;
extern short D_80100B76;
extern unsigned short *D_80100B78;
extern unsigned char D_8009CDA4[];
extern void func_800EB24C(int n);

void func_800EA0C0(Menu *m)
{
    Slot *s;
    for (s = D_800FF354; s->x != -1; s++) {
        int id;
        short k;
        D_80100B70.x = s->x;
        D_80100B70.y = s->y;
        D_80100B70.w = m->w10 + s->dw + 2;
        id = D_800FF254[m->page * 10 + s->idx];
        k = id - 1;
        if (id - 1 >= 0xFF) continue;
        switch (id) {
        case 0x57: D_80100B76 = 0x7D12; break;
        case 0x58: D_80100B76 = 0x7D52; break;
        case 0x59: D_80100B76 = 0x7D92; break;
        case 0x75: D_80100B76 = 0x7DD2; break;
        case 0x98: D_80100B76 = 0x7A93; break;
        case 0x9E: D_80100B76 = 0x7AD3; break;
        case 0xA0: D_80100B76 = 0x7B13; break;
        case 0xA5: D_80100B76 = 0x7B53; break;
        default: D_80100B76 = 0; break;
        }
        if (k < 0) k = 0;
        if (D_8009CDA4[k] == 0xFF) {
            D_80100B78 = m->icons[k];
            func_800EB24C(1);
        }
    }
}
