// FUNC 8002ee50 208 MAIN0
// MATCHING 8002ee50 208
typedef struct O {
    char act;
    char pad0;
    char type;
    char sub;
    char pad1[0xc - 4];
    unsigned char b0c;
    char pad2[0x14 - 0xd];
    int y;
    char pad3[0x20 - 0x18];
    short w20;
    char pad4[0x40 - 0x22];
    int *h;
    int *d;
} O;
extern O *FUN_80018448();

void FUN_8002ee50(char a, int x, int y, int z)
{
    O *p = FUN_80018448();
    short s;
    if (p == 0) return;
    p->act = 1;
    p->type = 0x32;
    p->sub = 0;
    p->b0c = a;
    *p->h = x << 16;
    p->y = y << 16;
    *p->d = z << 16;
    switch (p->b0c) {
    case 0: s = 0x10; break;
    case 1: s = 0x18; break;
    case 2: s = 0x20; break;
    default: return;
    }
    p->w20 = s;
}
