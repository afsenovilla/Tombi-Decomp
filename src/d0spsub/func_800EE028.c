// FUNC 800ee028 852 D0SPSUB
// MATCHING 800ee028 852
typedef struct { unsigned char state, sub; char p2[0x10]; short mode; short page; short cursor; char p18[6]; short sel; } Menu;
extern unsigned short D_1F8001FC;
extern unsigned char D_801086F0[];
extern unsigned char D_8009CDA3[];
extern unsigned char D_8010A200[];
extern unsigned char D_8010A204;
extern int func_800EC504(Menu *m, int a);
extern int func_800ECD94(Menu *m);
extern int func_800EFD10(void);
extern void func_800EDE64(int kind, int add, int bank);
extern void func_800EF538(void *o, int a);
extern void playSFX(int id);
extern void playSFXAndSetNote(int a, int b);
extern void SfxPlay2(int a, int b);

int func_800EE028(Menu *m)
{
    int pages;
    switch (m->sub) {
    case 0:
        m->cursor = 0;
        m->page = 0;
        m->sub++;
    case 1:
        if (func_800EC504(m, 1)) return 0;
        pages = (func_800EFD10() + 9) / 10;
        if (D_1F8001FC & 0x400) goto left;
        if (D_1F8001FC & 0x800) goto right;
        if (D_1F8001FC & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return 1;
        } else if (D_1F8001FC & 0x4008) {
            switch (m->cursor) {
            case 0:
                m->sel = m->page * 10;
                m->sub++;
                break;
            case 4:
                m->mode = 1;
                m->cursor = 0;
                m->sel = -1;
                m->sub = 0;
                break;
            case 5:
                m->mode = 2;
                m->cursor = 0;
                m->sel = -1;
                m->sub = 0;
                break;
            case 6:
                m->mode = 0;
                m->sub = 0;
                break;
            case 7:
                m->mode = 3;
                m->sub = 0;
                playSFXAndSetNote(10, 10);
                return 2;
            case 8:
            case 9:
            case 10:
            case 11:
                SfxPlay2(10, 10);
                return 1;
            }
            playSFXAndSetNote(10, 10);
        }
        return 0;
    case 2:
        if (func_800ECD94(m)) return 0;
        pages = (func_800EFD10() + 9) / 10;
        if (D_1F8001FC & 0x400) {
        left:
            if (m->page > 0) {
                playSFX(8);
                m->page--;
                m->sel -= 10;
            }
        } else if (D_1F8001FC & 0x800) {
        right:
            if (m->page < pages - 1) {
                playSFX(8);
                m->page++;
                m->sel += 10;
                if (m->sel >= pages * 10) m->sel = pages * 10;
            }
        } else if (D_1F8001FC & 0x2000) {
            m->sel = -1;
            SfxPlay2(0x1B, 0x1C);
            m->sub = 1;
        } else if (D_1F8001FC & 0x4008) {
            int k = D_801086F0[m->sel];
            if (D_8009CDA3[k]) {
                func_800EDE64(0, k - 1, 3);
                D_8010A204 = 0;
                SfxPlay2(10, 10);
                m->sub++;
            }
        }
        return 0;
    case 3:
        func_800EF538(D_8010A200, 3);
        if (D_8010A204 == 2) m->sub = 2;
        return 0;
    }
    return 0;
}
