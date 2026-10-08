// FUNC 8003fd78 464 MAIN0
/* falta: el juego deja sh box3 justo antes del beqz (delay slot) y lee b9c en v1 antes de los sb 0; probado: -fno-schedule-insns, b9c en local, box3 = *p al final, store por cast, box3 en ambas ramas, v unsigned */
#include "TOBJ.H"
extern short DAT_80115320[];
extern short TileCollideAt(TObj *o, short x, short y);
extern void FUN_8003fb90(TObj *o);

int ObjTileCollide(TObj *o, short dy, int noMove)
{
    short *p;
    short r;
    short v;
    int a;
    int b;

    p = &DAT_80115320[((short *)o->anim)[1] * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    v = *p;
    o->b69 = 0;
    o->wb0 = 0;
    *(unsigned char *)&o->da0 = 0;
    o->bbe = 0;
    o->box3 = v;
    if (o->b9c) {
        if (o->velX >= 0) {
            a = 8;
            b = -8;
        } else {
            a = -8;
            b = 8;
        }
    } else {
        if (o->velH >= 0) {
            a = 8;
            b = -8;
        } else {
            a = -8;
            b = 8;
        }
    }
    r = TileCollideAt(o, o->h->p.whole, dy + (o->y.p.whole + o->box2));
    if (r == 0) {
        r = TileCollideAt(o, o->h->p.whole + a, dy + (o->y.p.whole + o->box2));
        if (r == 0) {
            r = TileCollideAt(o, o->h->p.whole + b, dy + (o->y.p.whole + o->box2));
            if (r == 0)
                return 0;
        }
    }
    if (noMove == 0)
        o->y.p.whole += dy;
    FUN_8003fb90(o);
    return r;
}
