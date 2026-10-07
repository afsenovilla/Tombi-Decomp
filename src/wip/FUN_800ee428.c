// FUNC 800ee428 44 X000
typedef struct O { char p0[0x20]; short t; char p1[0x2e - 0x22]; unsigned short f; char p2[0x84 - 0x30]; int a; int b; int c; } O;
void FUN_800ee428(O *o){ int v = 0x10; o->t = 10; o->a = 0; if (o->f & 1) v = 0xf0; o->b = v; o->c = 0; }
