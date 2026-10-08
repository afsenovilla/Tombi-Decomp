// FUNC 8011ae94 132 X009
// MATCHING 8011ae94 132
#include "TOBJ.H"
extern void func_8011AF18(TObj *, int);

void func_8011AE94(TObj *o)
{
    switch (o->subtype) {
    case 1:
        func_8011AF18(o, 0);
        break;
    case 2:
        func_8011AF18(o, 1);
        break;
    case 5:
        func_8011AF18(o, 2);
        break;
    case 6:
        func_8011AF18(o, 3);
        break;
    case 7:
        func_8011AF18(o, 4);
        break;
    case 8:
        func_8011AF18(o, 5);
        break;
    case 9:
        func_8011AF18(o, 6);
        break;
    }
}
