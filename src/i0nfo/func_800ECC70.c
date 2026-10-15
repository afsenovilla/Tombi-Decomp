// FUNC 800ecc70 164 I0NFO
// MATCHING 800ecc70 164
#include "TOBJ.H"
extern void func_8001FEC0(TObj *o);
extern unsigned char D_8009C942;

void func_800ECC70(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velX = 0;
        o->velY = 0;
        o->velH = 0;
        o->velV = 0;
        o->state++;
    case 1:
        if (D_8009C942 == 0) {
            func_8001FEC0(o);
            if (--o->timer == 0) {
                o->b6a = 0;
                o->step = 0;
                o->state = 0;
            }
        }
        break;
    }
}
