// FUNC 8002ef20 320 MAIN0
typedef struct T { unsigned short w; short i; int *a; int pad; } T;
extern T DAT_80079d50[];
extern int DAT_1f8002c8[];
extern void FUN_8001fe6c(void *);

void FUN_8002ef20(unsigned char *o)
{
    if (o[3] != 9) {
        *(unsigned short *)(o + 0x1e) = DAT_80079d50[o[3]].w;
        *(int *)(o + 0x3c) = DAT_1f8002c8[DAT_80079d50[o[3]].i];
        *(int *)(o + 0x24) = DAT_80079d50[o[3]].a[o[0xc]];
    } else {
        *(unsigned short *)(o + 0x1e) = DAT_80079d50[0].w;
        *(int *)(o + 0x3c) = DAT_1f8002c8[DAT_80079d50[0].i];
        *(int *)(o + 0x24) = *DAT_80079d50[0].a;
    }
    unsigned char t;
    FUN_8001fe6c(o);
    t = o[4];
    o[0xa] = 0xd;
    o[0xd] = 0x80;
    *(signed char *)(o + 0xf) = -7;
    *(int *)(o + 0x8c) = 0;
    o[0x6b] = 0;
    o[0x1c] |= 0x80;
    o[4] = t + 1;
}
