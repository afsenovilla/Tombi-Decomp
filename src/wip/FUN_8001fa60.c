// FUNC 8001fa60 40 MAIN0
typedef struct E { short a, b; } E;
typedef struct O { char p0[0x14]; int y; char p1[0x28-0x18]; E *t; } O;
void FUN_8001fa60(O *o, unsigned short i){ int v = *(short*)((i << 2) + (int)o->t + 2); o->y = v * 256 + o->y; }
