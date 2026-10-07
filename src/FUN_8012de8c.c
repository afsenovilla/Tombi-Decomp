// FUNC 8012de8c 80 X000
// MATCHING 8012de8c 80
#include "TOBJ.H"
extern void FUN_8012dcfc(TObj *);
extern void FUN_8012d9fc(TObj *);

void FUN_8012de8c(TObj *o)
{
    switch (o->step) {
    case 0:
        FUN_8012dcfc(o);
        break;
    case 1:
        FUN_8012d9fc(o);
        break;
    }
}
