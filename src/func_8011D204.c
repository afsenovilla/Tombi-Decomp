// FUNC 8011d204 252 X000
// MATCHING 8011d204 252
#include "TOBJ.H"

void func_8011D204(TObj *o)
{
    if (o->animFrame != 0x80) {
        if (o->animFrame < 0x81) o->animFrame++;
        else o->animFrame--;
    }
}

void func_8011D230(TObj *o)
{
    if (o->animFrame != 0x8f) o->animFrame++;
}

void func_8011D250(TObj *o)
{
    if (o->animFrame != 0x87) {
        if (o->animFrame < 0x87) o->animFrame++;
        else o->animFrame--;
    }
}

void func_8011D27C(TObj *o)
{
    if (o->animFrame != 0x83) {
        if (o->animFrame < 0x83) o->animFrame++;
        else o->animFrame--;
    }
}

void func_8011D2A8(TObj *o)
{
    o->animFrame += 2;
    if (o->animFrame >= 0x90) o->animFrame = 0x8f;
}

void func_8011D2D4(TObj *o)
{
    o->animFrame += 2;
    if (o->animFrame >= 0x88) o->animFrame = 0x87;
}
