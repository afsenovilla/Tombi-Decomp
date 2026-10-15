// FUNC 800efaa4 620 D0SPSUB
// MATCHING 800efaa4 620
typedef struct { short x, y, w, h; } RECT;
typedef struct { unsigned char state; char p1[0x11]; short mode; } Menu;
extern Menu D_800A45D8;
extern unsigned char D_800A45D9, D_800A45DA, D_800A45DB;
extern short D_800A45E8, D_800A45F6;
extern void *D_800A4620, *D_800A4624, *D_800A4628, *D_800A462C, *D_800A4630, *D_800A4634, *D_800A4638, *D_800A463C;
extern short D_8010A058;
extern int D_8011112C, D_80111130;
extern unsigned short D_8009D670;
extern unsigned short D_1F8001FC;
extern void *D_1F8003A0, *D_1F8003A4;
extern char D_800F18E0[], D_800F24E0[], D_800F7B78[];
extern char D_800FE8D0[], D_800FE8E0[]; /* aliases of D_800FE8D8, D_800FE8E4 (no CSE) */
extern char D_800FE8A4[], D_800FE8E4[], D_800FE8D8[], D_800FE948[], D_800FEC74[], D_800FE974[];
extern void LoadImage(RECT *r, void *p);
extern int func_800ECF88(Menu *m);
extern int func_800ED0C4(Menu *m);
extern int func_800EE028(Menu *m);
extern int func_800EE9AC(Menu *m);

int func_800EFAA4(void)
{
    Menu *m = &D_800A45D8;
    RECT r;
    int ret;
    int pad;
    switch (m->state) {
    case 0:
        r.x = 0x120;
        r.y = 0x1E0;
        r.w = 0x30;
        r.h = 0x1F;
        LoadImage(&r, D_800F18E0);
        D_1F8003A0 = D_800F24E0;
        D_1F8003A4 = D_800F7B78;
        D_800A4620 = D_800FE8A4;
        D_800A4624 = D_800FE8E4;
        D_800A4628 = D_800FE8D8;
        D_800A462C = D_800FE8D0 + 8;
        D_800A4630 = D_800FE8E0 + 4;
        D_800A4634 = D_800FE948;
        D_800A4638 = D_800FEC74;
        D_800A463C = D_800FE974;
        D_800A45E8 = 1;
        D_800A45F6 = -1;
        D_8010A058 = -1;
        D_8011112C = 0;
        D_80111130 = 0;
        D_800A45D9 = 0;
        D_800A45DA = 0;
        D_800A45DB = 0;
        m->state++;
    case 1:
        pad = *(volatile unsigned short *)&D_8009D670 & 0xF0;
        if (pad != D_8011112C) {
            D_8011112C = pad;
            D_80111130 = 0x20;
        } else if (--D_80111130 == 0) {
            D_80111130 = 4;
            D_1F8001FC |= pad;
        }
        switch (m->mode) {
        case 0:
            ret = func_800ECF88(m);
            break;
        case 1:
            ret = func_800ED0C4(m);
            break;
        case 2:
            ret = func_800EE028(m);
            break;
        case 3:
            ret = func_800EE9AC(m);
            break;
        }
        break;
    }
    return ret;
}
