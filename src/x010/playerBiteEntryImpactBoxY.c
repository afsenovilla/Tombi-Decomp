// FUNC 801031fc 148 X010
// MATCHING 801031fc 148
typedef struct H { short pad; unsigned short w; } H;
typedef struct O {
    char pad0[6];
    unsigned char state;
    char pad1[0x12 - 7];
    short a;
    char pad1b[2];
    short y;
    char pad1c[2];
    short b;
    char pad2[0x20 - 0x1c];
    short timer;
    char pad3[0x40 - 0x22];
    H *h;
    char pad4[0x70 - 0x44];
    unsigned short w70;
    unsigned short w72;
    char pad5[0x7c - 0x74];
    short velX;
    short velY;
    char pad6[0x9c - 0x80];
    char b9c;
    char pad7[0xac - 0x9d];
    char wac;
} O;
extern O *BITTEN_OBJECT;
extern void FUN_800eeb5c();
extern void spawnImpactEffect();

void playerBiteEntryImpactBoxY(O *o)
{
    O *p;
    unsigned short s1, s2, s3;
    o->timer = 0;
    FUN_800eeb5c(o, 0xd);
    spawnImpactEffect(2, o->a, o->y, o->b);
    o->wac = 3;
    o->b9c = 0;
    p = BITTEN_OBJECT;
    o->velX = 0;
    o->velY = 0;
    o->h->w = p->h->w;
    s1 = p->w72;
    s2 = p->w70;
    s3 = p->y;
    o->state = 2;
    o->y = s3 - (s1 - s2);
}
