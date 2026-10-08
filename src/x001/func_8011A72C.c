// FUNC 8011a72c 304 X001
// MATCHING 8011a72c 304
#include "TOBJ.H"
extern short D_8007A3F0[];

int func_8011A72C(TObj *o)
{
    o->h->raw = o->d30 + ((D_8007A3F0[((unsigned char *)&o->d84)[1]] * *(signed char *)&o->ba6) << 4);
    o->d84 += o->velH;
    o->velH += o->velX;
    if (o->velH == o->w74) {
        if (--o->timer == -1) {
            o->w74 = 0;
            o->velH = 0;
            o->velX = 0;
            o->d84 = 0;
            return 1;
        }
        o->velX *= -1;
        o->velH /= 2;
        o->w74 /= 2;
        o->velX /= 2;
    } else if (o->velH == -o->w74) {
        o->velX *= -1;
    }
    return 0;
}
