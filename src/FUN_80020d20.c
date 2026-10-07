// FUNC 80020d20 120 MAIN0
// MATCHING 80020d20 120
typedef struct O { unsigned char a; char p1; unsigned char t; char pad[0x10 - 3]; int x; int y; int z; } O; extern O *ObjAlloc(void);
void f(unsigned char t,int x,int y,int z){ O*o=ObjAlloc(); if(o){ o->a=1; o->t=t; o->x=x<<16; o->y=y<<16; o->z=z<<16; } }
