// FUNC 800ec084 432 I0NFO
#include "TOBJ.H"
extern void func_800174C0(void *p, int x, int y, int w, int h);
extern void func_8005F0FC(int a);
extern void func_800EB4C8(int n, void *p);
extern void func_800EBEDC(TObj *o);
extern short D_1F800176[];
extern short D_1F80017A[];
extern unsigned char D_800A60A3;
extern void *D_800EE8CC;

void func_800EC084(TObj *o)
{
    void *p;
    o->d30 = D_1F800176[0] + 0xA1;
    o->d34 = D_1F80017A[0] - 0xEF;
    o->h->p.whole = 0x1F - ((D_1F800176[0] + 0xA0) & 0x1F);
    o->y.p.whole = 0x1F - ((D_1F80017A[0] - 0xF0) & 0x1F);
    func_800EBEDC(o);
    p = D_800EE8CC;
    func_800EB4C8(0, p);
    func_800174C0(p, 0x300, 0, 0x80, 0x100);
    func_8005F0FC(0);
    p = D_800EE8CC;
    func_800EB4C8(1, p);
    func_800174C0(p, 0x380, 0, 0x40, 0x100);
    p = D_800EE8CC;
    func_800EB4C8(2, p);
    func_800174C0(p, 0x40, 0, 0x40, 0x20);
    p = D_800EE8CC;
    func_800EB4C8(3, p);
    func_800174C0(p, 0x40, 0x20, 0x40, 0x20);
    p = D_800EE8CC;
    func_800EB4C8(4, p);
    func_800174C0(p, 0x40, 0x40, 0x40, 0x20);
    if (D_800A60A3 != 0) {
        o->b6b = (D_800A60A3 == 1) ? 0x80 : 0xFF;
    } else {
        o->b6b = 0;
    }
}
