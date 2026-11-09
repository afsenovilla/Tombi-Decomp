// FUNC 8010bf70 196 X004
// MATCHING 8010bf70 196
// VERSION BETA_980203 x00 8010FE58 176
#if defined(VERSION_BETA_980203)
// Beta version of PAL x004 8010BF70 (playerGrabSpinEntry.c): PAL C of src/x004/playerGrabSpinEntry.c (playerGrabSpinEntry, twin score 0.717), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
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
extern P *PLAYER_EXT;
extern char *GRABBED_OBJECT;
extern void FUN_80010dac();
extern void FUN_8001fe94();
extern void FUN_8001e560(int, int);

void playerGrabSpinEntry(O *o)
{
    P *p; short v;
    PLAYER_EXT->b4 = 1;
    GRABBED_OBJECT[0x69] = 4;
    FUN_8001e560(0x1e, 8);
    p = PLAYER_EXT;
    v = (*(short *)(GRABBED_OBJECT + 0x6e) << 8) / 0x18;
    if (o->animFrame & 1)
        v = -v;
    p->s10 = v;
    o->d88 = 0;
    o->state++;
}
#else
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
extern P *PLAYER_EXT;
extern char *GRABBED_OBJECT;
extern void FUN_80010dac();
extern void FUN_8001fe94();
extern void FUN_8001e560(int, int);

void playerGrabSpinEntry(O *o)
{
    P *p; short v;
    o->anim = (void *)FUN_80010dac;
    FUN_8001fe94(o, 0);
    PLAYER_EXT->b4 = 1;
    GRABBED_OBJECT[0x69] = 4;
    FUN_8001e560(0x1e, 8);
    p = PLAYER_EXT;
    v = (*(short *)(GRABBED_OBJECT + 0x6e) << 8) / 0x18;
    if (o->animFrame & 1)
        v = -v;
    p->s10 = v;
    o->d88 = 0;
    o->state++;
}
#endif
