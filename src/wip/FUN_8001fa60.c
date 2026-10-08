// FUNC 8001fa60 40 MAIN0
typedef struct E { short a, b; } E;
typedef struct O { char p0[0x14]; int y; char p1[0x28-0x18]; E *t; } O;
void FUN_8001fa60(O *o, unsigned short i){ o->y += ((E*)((int)o->t + (i << 2)))->b << 8; }
