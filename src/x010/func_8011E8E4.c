// FUNC 8011e8e4 172 X010
// MATCHING 8011e8e4 172
#include "TOBJ.H"
extern int func_8011E000(TObj *a, TObj *b, unsigned char mode, int ang);

void func_8011E8E4(TObj *a, TObj *o)
{
    switch (o->subtype) {
    case 0:
        func_8011E000(a, o, 0, o->d30);
        return;
    case 1:
    case 2:
        func_8011E000(a, o, o->subtype, o->d30);
        func_8011E000(a, o, 2, (o->d30 + 0x400) & 0xfff);
        return;
    }
    func_8011E000(a, o, 2, o->d30);
}
