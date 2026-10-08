// FUNC 80102eb4 140 X000
// MATCHING 80102eb4 140
typedef struct O {
    char pad0[6];
    unsigned char state;
    char pad1[0x16 - 7];
    unsigned short y;
    char pad2[0x20 - 0x18];
    short timer;
    char pad3[0x2e - 0x22];
    unsigned short animFrame;
    char pad4[0x40 - 0x30];
    struct H { short pad; unsigned short w; } *h;
    char pad5[0x7c - 0x44];
    short velX;
    short velY;
    char pad6[0x9c - 0x80];
    char b9c;
    char pad7[0xac - 0x9d];
    char wac;
    char pad8[0xba - 0xad];
    unsigned short wba;
} O;
extern O *DAT_8009d2e8;
extern void FUN_800eeb5c();
extern void FUN_800ee4e0();

void FUN_80102eb4(O *o)
{
    O *p;
    o->timer = 0;
    FUN_800eeb5c(o, 0x23);
    o->wac = 3;
    o->b9c = 0;
    p = DAT_8009d2e8;
    o->velX = 0;
    o->velY = 0;
    *(unsigned short *)((char *)p + 0x2e) = o->animFrame & 1;
    o->h->w = p->h->w;
    p->y = o->y + o->wba;
    FUN_800ee4e0(o, 0);
    o->state = 2;
}
