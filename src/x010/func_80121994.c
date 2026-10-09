// FUNC 80121994 172 X010
// MATCHING 80121994 172
#include "TOBJ.H"
extern void func_80121534(TObj *, TObj *, int, int);

void func_80121994(TObj *o, TObj *p)
{
    switch (p->subtype) {
    case 0:
        func_80121534(o, p, 0, p->d30);
        return;
    case 1:
    case 2:
        func_80121534(o, p, p->subtype, p->d30);
        func_80121534(o, p, 2, (p->d30 + 0x400) & 0xfff);
        return;
    }
    func_80121534(o, p, 2, p->d30);
}
