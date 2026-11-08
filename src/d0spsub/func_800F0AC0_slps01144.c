// VERSION SLPS_01144 dspsub 800F0AC0 628
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800efaa4 (subScreenUpdate.c): extra p20 frame table, held pad 0xF000, mode 6
typedef struct { short x, y, w, h; } RECT;
typedef struct { unsigned char state; char p1[0x11]; short mode; } Menu;
extern Menu D_800A6120;
extern unsigned char D_800A6121, D_800A6122, D_800A6123;
extern short D_800A6130, D_800A613E;
extern void *D_800A6140, *D_800A6168, *D_800A616C, *D_800A6170, *D_800A6174, *D_800A6178, *D_800A617C, *D_800A6180, *D_800A6184;
extern short D_8010A31C;
extern int D_801113F0, D_801113F4;
extern unsigned short D_800A1380;
extern unsigned short JOYPAD_STATE;
extern void *D_1F8003A0, *D_1F8003A4;
extern char D_800F2FB0[], D_800F3BB0[], D_800FBFC0[], D_80103BD8[], D_801042E8[], D_80104320[], D_80104308[], D_80104314[], D_80104330[], D_80104394[], D_801046C0[], D_801043C0[];
extern void LoadImage(RECT *r, void *p);
extern int func_800EE084(Menu *m);
extern int func_800EE1B4(Menu *m);
extern int func_800EF048(Menu *m);
extern int func_800EF9B0(Menu *m);
extern int func_800F0324(Menu *m);

int func_800F0AC0(void)
{
    Menu *m = &D_800A6120;
    RECT r;
    int ret;
    int pad;
    switch (m->state) {
    case 0:
        r.x = 0x120;
        r.y = 0x1E0;
        r.w = 0x30;
        r.h = 0x1F;
        LoadImage(&r, D_800F2FB0);
        D_1F8003A0 = D_800F3BB0;
        D_1F8003A4 = D_800FBFC0;
        D_800A6140 = D_80103BD8;
        D_800A6168 = D_801042E8;
        D_800A616C = D_80104320;
        D_800A6170 = D_80104308;
        D_800A6174 = D_80104314;
        D_800A6178 = D_80104330;
        D_800A617C = D_80104394;
        D_800A6180 = D_801046C0;
        D_800A6184 = D_801043C0;
        D_800A6130 = 1;
        D_800A613E = -1;
        D_8010A31C = -1;
        D_801113F0 = 0;
        D_801113F4 = 0;
        D_800A6121 = 0;
        D_800A6122 = 0;
        D_800A6123 = 0;
        m->state++;
    case 1:
        pad = *(volatile unsigned short *)&D_800A1380 & 0xF000;
        if (pad != D_801113F0) {
            D_801113F0 = pad;
            D_801113F4 = 0x20;
        } else if (--D_801113F4 == 0) {
            D_801113F4 = 4;
            JOYPAD_STATE |= pad;
        }
        switch (m->mode) {
        case 0:
            ret = func_800EE084(m);
            break;
        case 1:
            ret = func_800EE1B4(m);
            break;
        case 2:
            ret = func_800EF048(m);
            break;
        case 3:
            ret = func_800EF9B0(m);
            break;
        case 6:
            ret = func_800F0324(m);
            break;
        }
        break;
    }
    return ret;
}
#endif
