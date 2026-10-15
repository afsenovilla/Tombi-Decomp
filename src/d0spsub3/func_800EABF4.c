// FUNC 800eabf4 652 D0SPSUB3
// MATCHING 800eabf4 652
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[8]; short sel; char p20[0x3C]; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; int pC; int str; void *p14; } Text;
extern Slot D_800FF3AC[];
extern Text D_80100B70;
extern unsigned short D_8009D2A4;
extern unsigned char D_80100988[];
extern unsigned char D_80100238[];
extern int D_80100300[];
extern unsigned char D_8010065C[];
extern unsigned char D_8009D0A4[];
extern char D_800F68FC[];
extern void func_800EBC10(int a, int b);
extern void func_800EB24C(int n);

void func_800EABF4(Menu *m)
{
    Slot *s;
    for (s = D_800FF3AC; s->x != -1; s++) {
        int i;
        int n;
        D_80100B70.x = s->x + 0x18;
        D_80100B70.y = s->y;
        D_80100B70.w = m->w10 + 1;
        i = m->page * 10 + s->idx;
        if (i >= D_8009D2A4) continue;
        D_80100B70.str = D_80100300[D_80100238[D_80100988[i]]];
        D_80100B70.p14 = D_800F68FC;
        if (i == m->sel) D_80100B70.attr = 0x7C15;
        else D_80100B70.attr = (((D_8010065C[D_80100988[i]] & 0x7F) + 0x1F0) << 6) | 0x15;
        func_800EBC10(5, 1);
        n = D_8009D0A4[D_80100988[i]];
        if (D_8010065C[D_80100988[i]] & 0x80) {
            D_80100B70.x += 0xB0;
            D_80100B70.y += 0x10;
            D_80100B70.w = m->w10;
            D_80100B70.img = m->p5c[10];
            func_800EB24C(1);
            D_80100B70.x += 0xA;
            if (n >= 10) {
                D_80100B70.img = m->p5c[n / 10 % 10];
                func_800EB24C(1);
            }
            D_80100B70.x += 0xA;
            D_80100B70.img = m->p5c[n % 10];
            func_800EB24C(1);
        }
    }
}
