// FUNC 8010dd44 72 X006
// MATCHING 8010dd44 72
typedef struct { char p0[5]; char b5; char b6; char p1[0x69-7]; char b69; char p2[0x9c-0x6a]; char b9c; char p3[0xac-0x9d]; char bac; } TO;
extern void fa(TO *o, int a, int b);

void FUN_8010dd44(TO *o)
{
    fa(o, 0x44, 0);
    o->b69 = 0;
    o->b9c = 0;
    o->bac = 0;
    o->b5 = 0x3e;
    o->b6 = 0;
}
