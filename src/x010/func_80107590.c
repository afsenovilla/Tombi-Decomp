// FUNC 80107590 72 X010
// MATCHING 80107590 72
#include "TOBJ.H"
void func_80107590(TObj *o)
{
    o->b9c = 1;
    o->d88 = -0x100;
    o->ba4 = 0;
    if (o->ba6 & 6) {
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
    }
    o->step = 0x10;
    o->state = 2;
}
