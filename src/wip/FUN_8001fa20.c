// FUNC 8001fa20 64 MAIN0
typedef struct M {short x,y;} M;
typedef struct TObj {
 char pad0[0x14];
 int y;
 char pad1[0x28-0x18];
 M *movetab;
 char pad2[0x40-0x2c];
 int *h;
} TObj;
void FUN_8001fa20(TObj *o, unsigned short i){ M *m = &o->movetab[i]; *o->h += m->x<<8; o->y += m->y<<8; }
