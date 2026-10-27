// FUNC 800e9a08 516 D0SPSUB3
// MATCHING 800e9a08 516
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern int GAME_AREA00_EVENT_CONTROL;
extern unsigned char D_8009CDAC, D_8009CDB1, D_8009CE3D;
extern char D_800F68FC[];
extern int D_800FFD08[];
extern int D_800FFD0C, D_800FFD20, D_800FFD24;
extern Text D_80100B70;
extern void subScreenDrawString(int a, int b);

void subScreenDrawStatusNotes(Menu *m)
{
    int n;
    if (GAME_AREA00_EVENT_CONTROL & 0x40) {
        D_80100B70.x = 0x60;
        D_80100B70.y = 0x98;
        D_80100B70.w = m->w10 + 1;
        D_80100B70.attr = 0x7C15;
        D_80100B70.p14 = D_800F68FC;
        D_80100B70.str = D_800FFD0C;
        subScreenDrawString(5, 1);
    }
    n = D_8009CDB1;
    if (n >= 2) {
        D_80100B70.x = 0x60;
        D_80100B70.y = 0xA8;
        D_80100B70.w = m->w10 + 1;
        D_80100B70.attr = 0x7C15;
        if (n == 0xFF) n = 5;
        D_80100B70.str = D_800FFD08[n];
        D_80100B70.p14 = D_800F68FC;
        subScreenDrawString(5, 1);
    }
    if (D_8009CDAC == 0xFF) {
        D_80100B70.x = 0xF0;
        D_80100B70.y = 0x98;
        D_80100B70.w = m->w10 + 1;
        D_80100B70.attr = 0x7C15;
        D_80100B70.p14 = D_800F68FC;
        D_80100B70.str = D_800FFD20;
        subScreenDrawString(5, 1);
    }
    if (D_8009CE3D == 0xFF) {
        D_80100B70.x = 0xF0;
        D_80100B70.y = 0xA8;
        D_80100B70.w = m->w10 + 1;
        D_80100B70.attr = 0x7C15;
        D_80100B70.p14 = D_800F68FC;
        D_80100B70.str = D_800FFD24;
        subScreenDrawString(5, 1);
    }
}
