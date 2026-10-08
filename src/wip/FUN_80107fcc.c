// FUNC 80107fcc 412 X000
typedef struct { unsigned short frac; short whole; } FP;
typedef struct {
    unsigned char active; char p01[5]; unsigned char state; char p07[8];
    signed char b0f; char p10[0x2e - 0x10]; unsigned short animFrame;
    char p30[0x44 - 0x30]; FP *d; char p48[0x69 - 0x48]; unsigned char b69;
    char p6a[0x7c - 0x6a]; short velX; short velY; char p80[0x8c - 0x80]; int d8c;
    char p90[0x9c - 0x90]; unsigned char b9c, b9d, b9e, b9f; char pa0[2]; unsigned char ba2;
    char pa3[0xad - 0xa3]; unsigned char bad; char pae[0xb2 - 0xae]; short wb2;
} TO;
extern unsigned char *DAT_8009c330;
extern unsigned char DAT_8009cda2;
extern unsigned short DAT_1f8001c8;
extern void FUN_8001fec0(TO *);
extern void FUN_800eeb5c(TO *, int);

void FUN_80107fcc(TO *o)
{
    unsigned short f;
    FP *d;
    switch (o->state) {
    case 99:
        break;
    case 0:
        DAT_8009c330[8] = o->active;
        f = o->animFrame;
        o->velX = 0x78;

        o->active = 2;
        o->ba2 = 2;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        o->bad = 0;
        o->b69 = 0;
        o->b0f = -0x14;
        o->animFrame = f & 1;
        FUN_800eeb5c(o, 0x2b);
        o->state = o->state + 1;
    case 1:
        FUN_8001fec0(o);
        if (DAT_8009cda2 == 0)
            o->state = o->state + 1;
        break;
    case 2:
        FUN_8001fec0(o);
        if (DAT_1f8001c8 & 1) {
            d = o->d;
            d->whole = d->whole - 5;
        } else {
            d = o->d;
            d->whole = d->whole + 5;
        }
        o->velX = o->velX - 5;
        if (o->velX != 0)
            return;
        o->b0f = -8;
        o->active = DAT_8009c330[8];
        o->b9c = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        *(short *)(DAT_8009c330 + 0x20) = 0;
        break;
    }
}
