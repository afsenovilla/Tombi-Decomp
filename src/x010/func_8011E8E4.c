// FUNC 8011e8e4 172 X010
// MATCHING 8011e8e4 172
#include "TOBJ.H"
extern void func_8011E000(TObj *, TObj *, int, int);

void func_8011E8E4(TObj *o, TObj *p)
{
    switch (p->subtype) {
    case 0:
        func_8011E000(o, p, 0, p->d30);
        return;
    case 1:
    case 2:
        func_8011E000(o, p, p->subtype, p->d30);
        func_8011E000(o, p, 2, (p->d30 + 0x400) & 0xfff);
        return;
    }
    func_8011E000(o, p, 2, p->d30);
}
