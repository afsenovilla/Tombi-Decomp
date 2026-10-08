// FUNC 8001fa20 64 MAIN0
typedef struct TObj {
 char pad0[0x14];
 int y;
 char pad1[0x28-0x18];
 short *movetab;
 char pad2[0x40-0x2c];
 int *h;
} TObj;
void FUN_8001fa20(TObj *o, unsigned short i){ short *p; p = (short*)((char*)o->movetab + i*4); *o->h += p[0]*256; o->y += p[1]*256; }
