// FUNC 80121f2c 324 X010
// MATCHING 80121f2c 324
typedef struct O {
    char p0[6]; unsigned char state; char p1[0x19]; short timer; char p2[0x5a];
    short velX; unsigned short velY; short velH; char p3[0xa]; int d8c;
} O;
static __inline__ void body(O *o)
{
    short s;
    unsigned char t;
    switch (o->state) {
    case 0:
        o->velH = 0x80;
        o->velX = 4;
        o->d8c = 0;
        o->velY = 0;
        o->timer = 1 - *((unsigned char *)o + 0x6b);
        o->state++;
        break;
    case 1:
        s = o->velH - o->velX;
        o->velH = s;
        if (o->timer != 0)
            s = o->velY - s;
        else
            s = o->velY + s;
        o->velY = s;
        o->d8c = *(volatile unsigned short *)&o->velY >> 8;
        if (o->velH < 1) {
            o->velH = 0;
            o->state = o->state + 1;
        }
        break;
    case 2:
        s = o->velH + o->velX;
        o->velH = s;
        if (o->timer != 0)
            s = o->velY + s;
        else
            s = o->velY - s;
        o->velY = s;
        o->d8c = *(volatile unsigned short *)&o->velY >> 8;
        if (o->velH >= 0x80) {
            o->velH = 0x80;
            o->state = o->state - 1;
            o->timer = 1 - (unsigned short)o->timer;
        }
        break;
    }
}
void FUN_80121f2c(O *o) { body(o); }
