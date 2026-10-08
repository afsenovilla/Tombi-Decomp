// FUNC 80117d48 376 X009
// MATCHING 80117d48 376
#include "TOBJ.H"

extern int D_8009C948;
extern int D_8009C94C;
extern int func_80117EC0(TObj *, int);

void func_80117D48(TObj *o)
{
    switch (o->subtype) {
    case 0:
        func_80117EC0(o, 0);
        break;
    case 1:
        func_80117EC0(o, 1);
        func_80117EC0(o, 2);
        func_80117EC0(o, 3);
        break;
    case 2:
        func_80117EC0(o, 4);
        func_80117EC0(o, 5);
        func_80117EC0(o, 6);
        func_80117EC0(o, 7);
        break;
    case 4:
        func_80117EC0(o, 8);
        func_80117EC0(o, 9);
        break;
    case 5:
        D_8009C948 = func_80117EC0(o, 10);
        D_8009C94C = func_80117EC0(o, 11);
        func_80117EC0(o, 12);
        break;
    case 8:
        func_80117EC0(o, 13);
        func_80117EC0(o, 14);
        func_80117EC0(o, 15);
        func_80117EC0(o, 16);
        func_80117EC0(o, 17);
        func_80117EC0(o, 18);
        break;
    case 9:
        func_80117EC0(o, 19);
        break;
    case 10:
        func_80117EC0(o, 20);
        break;
    case 11:
        func_80117EC0(o, 21);
        func_80117EC0(o, 22);
        break;
    }
}
