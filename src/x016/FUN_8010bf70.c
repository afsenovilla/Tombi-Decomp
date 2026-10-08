// FUNC 8010bf70 196 X016
// MATCHING 8010bf70 196
typedef struct O {
    char pad0[6];
    unsigned char state;
    char pad1[0x24 - 7];
    void *anim;
    char pad2[0x2e - 0x28];
    unsigned short animFrame;
    char pad3[0x88 - 0x30];
    int d88;
} O;
typedef struct P { char pad[4]; char b4; char pad1[0xb]; short s10; } P;
extern P *DAT_8009c330;
extern char *DAT_8009f0ec;
extern void FUN_80010dac();
extern void FUN_8001fe94();
extern void FUN_8001e560(int, int);

void FUN_8010bf70(O *o)
{
    P *p; short v;
    o->anim = (void *)FUN_80010dac;
    FUN_8001fe94(o, 0);
    DAT_8009c330->b4 = 1;
    DAT_8009f0ec[0x69] = 4;
    FUN_8001e560(0x1e, 8);
    p = DAT_8009c330;
    v = (*(short *)(DAT_8009f0ec + 0x6e) << 8) / 0x18;
    if (o->animFrame & 1)
        v = -v;
    p->s10 = v;
    o->d88 = 0;
    o->state++;
}
