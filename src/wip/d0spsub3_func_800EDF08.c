// FUNC 800edf08 548 D0SPSUB3
typedef struct {
    unsigned char state, b1, b2, b3;
    char p4[0xc];
    short w10;
    char p12[2];
    short page;
    char p16[0x42];
    void **icons;
} Menu;
typedef struct { short x, y, dw, idx; } Slot;
extern Menu D_800A45D8;
extern Slot D_8010075C[], D_801007BC[];
extern void *D_800F5400[];
extern unsigned short D_8009D2A4;
extern short D_80100918;
extern short D_80100B70[];
extern short D_80100B72[];
extern short D_80100B74[];
extern short D_80100B76;
extern void * D_80100B78;
extern void func_800E9DF8(Menu *m);
extern void func_800EBE8C(Menu *m);
extern void func_800EA84C(Menu *m);
extern void func_800EABF4(Menu *m);
extern void func_800EAE80(Menu *m, int n);
extern void func_800EB24C(int n);

void func_800EDF08(void)
{
    Menu *m = &D_800A45D8;
    int pages;
    Slot *s;
    func_800E9DF8(m);
    if (D_80100918 >= 0) func_800EBE8C(m);
    pages = (D_8009D2A4 + 9) / 10;
    func_800EA84C(m);
    func_800EABF4(m);
    if (pages >= 2) {
        func_800EAE80(m, pages);
        s = D_8010075C;
        while (s->x != -1) {
            if ((s->idx == 0x16 && m->page == 0) || (s->idx == 0x17 && m->page >= pages - 1)) {
                s++;
            } else {
                D_80100B70[0] = s->x;
                D_80100B72[0] = s->y;
                D_80100B76 = 0x7C15;
                D_80100B74[0] = m->w10 + s->dw;
                D_80100B78 = m->icons[s->idx];
                s++;
                func_800EB24C(1);
            }
        }
    }
    D_80100B76 = 0x7812;
    for (s = D_801007BC; s->x != -1; s++) {
        D_80100B70[0] = s->x;
        D_80100B72[0] = s->y;
        D_80100B74[0] = m->w10 + s->dw;
        D_80100B78 = D_800F5400[s->idx];
        func_800EB24C(3);
    }
}
