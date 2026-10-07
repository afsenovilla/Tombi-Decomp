// FUNC 8001fb20 40 MAIN0
// MATCHING 8001fb20 40
typedef struct E { short a, b; } E;
typedef struct O {
    char pad0[0x14];
    int y;
    char pad1[0x28 - 0x18];
    E *tab;
    char pad2[0x2e - 0x2c];
    unsigned short fr;
} O;

void FUN_8001fb20(O *o)
{
    E *e = o->tab + o->fr;
    o->y += e->b << 8;
}
