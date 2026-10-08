// FUNC 8011b164 820 X001
// MATCHING 8011b164 820
#include "TOBJ.H"

extern short D_8007A3F0[];
extern short D_8007A5F0[];
extern unsigned short D_8009C962[];
extern void FUN_8001e4f0(int);

void func_8011B164(TObj *o)
{
    int s;
    int c;
    int b;
    short v;

    switch (o->state) {
    case 0:
        o->y.raw = o->d34;
        o->h->raw = o->d30;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->w74 = 0;
        if (o->w9a != 0) {
            if ((o->animFrame >> 1) & 1) {
                o->w74 = 0x300;
                o->velH = 0x300;
                o->velX = -0x18;
            } else {
                o->w74 = -0x300;
                o->velH = -0x300;
                o->velX = 0x18;
            }
            o->timer = 3;
        } else {
            if ((o->animFrame >> 1) & 1) {
                o->w74 = 0x200;
                o->velH = 0x200;
                o->velX = -0x10;
            } else {
                o->w74 = -0x200;
                o->velH = -0x200;
                o->velX = 0x10;
            }
            o->timer = 2;
        }
        o->ba6 = 0x40;
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        o->h->raw = o->d30 + ((D_8007A3F0[((unsigned char *)&o->d84)[1]] * (signed char)o->ba6) << 4);
        b = (signed char)o->ba6;
        c = D_8007A5F0[((unsigned char *)&o->d84)[1]] * b;
        o->d84 += o->velH;
        v = o->velH + o->velX;
        o->velH = v;
        o->y.raw = o->d34 - (b << 16) + (c << 4);
        if (v == o->w74) {
            if (--o->timer == -1) {
                o->state++;
                break;
            }
            o->velX *= -1;
            o->velH = o->velH / 2;
            o->w74 = o->w74 / 2;
            o->velX = o->velX / 2;
        } else if (v == -o->w74) {
            o->velX *= -1;
        }
        if (o->b6a != 0) {
            o->step = 1;
            o->state = 1;
            o->velV = 0x340;
            o->d88 = 0;
            o->ba6 = 0x20;
            o->b6b = 0;
            FUN_8001e4f0(D_8009C962[0] == 0 ? 0x42 : 0x47);
        }
        if (o->b68 != 0) {
            o->state = 0;
        }
        break;
    case 2:
        o->step = 0;
        o->state = 0;
        o->y.raw = o->d34;
        o->h->raw = o->d30;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->w74 = 0;
        break;
    }
}
