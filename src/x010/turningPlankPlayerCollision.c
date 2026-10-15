// FUNC 8011e8e4 172 X010
// MATCHING 8011e8e4 172
#include "TOBJ.H"
extern int collidePlayerWithTiltedBar(TObj *a, TObj *b, unsigned char mode, int ang);

void turningPlankPlayerCollision(TObj *a, TObj *o)
{
    switch (o->subtype) {
    case 0:
        collidePlayerWithTiltedBar(a, o, 0, o->d30);
        return;
    case 1:
    case 2:
        collidePlayerWithTiltedBar(a, o, o->subtype, o->d30);
        collidePlayerWithTiltedBar(a, o, 2, (o->d30 + 0x400) & 0xfff);
        return;
    }
    collidePlayerWithTiltedBar(a, o, 2, o->d30);
}
