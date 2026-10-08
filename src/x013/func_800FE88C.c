// FUNC 800fe88c 108 X013
// MATCHING 800fe88c 108
typedef struct {
    unsigned char active, b1, b2, b3, b04, step, state, substep;
    char p08[0x20 - 8]; short timer; char p22[0x2e - 0x22]; unsigned short frame;
    char p30[0x84 - 0x30]; int d84; int d88; int d8c;
    char p90[0x9c - 0x90]; unsigned char b9c; char p9d[0xac - 0x9d]; unsigned char bac;
    char pad[0xd1 - 0xad]; unsigned char bd1; char pd2[0xe0 - 0xd2]; short we0;
} O;
typedef struct { char p[8]; unsigned char b; } G;
extern G *D_8009C330;
void func_800FE88C(O *o)
{
    int x;
    D_8009C330->b = 0;
    o->we0 = 0x8c;
    o->active = 3;
    o->b9c = 2;
    o->bac = 1;
    o->bd1 = 0;
    o->timer = 10;
    o->d84 = 0;
    x = 0x10;
    if (o->frame & 1) x = 0xf0;
    o->d88 = x;
    o->d8c = 0;
    o->b04 = 1;
    o->step = 2;
    o->state = 3;
    o->substep = 0;
}
