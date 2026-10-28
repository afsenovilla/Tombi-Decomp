// VERSION SLPM_80165 x00 800F4434 100
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800EE560 (playerStepEntrySfxAnim4State5.c): keeps +0x30/+0x34, advances the state instead of setting 5.
typedef struct O {
    char pad0[6];
    unsigned char state;
    char pad1[0x24 - 7];
    void *anim;
    char pad2[0x30 - 0x28];
    int d30;
    int d34;
    char pad3[0x88 - 0x38];
    int d88;
    int d8c;
    char pad4[0x9c - 0x90];
    char b9c;
    char pad5[0xaa - 0x9d];
    char baa;
} O;
extern char DAT_80010c50[];
extern void FUN_8001e5f4();
extern void FUN_8001fe94();

void playerStepEntrySfxAnim4State5(O *o)
{
    FUN_8001e5f4(0x1c, 0x7f);
    o->b9c = 0;
    o->anim = DAT_80010c50;
    FUN_8001fe94(o, 4);
    o->d88 = 0;
    o->d8c = 0;
    o->baa = 0;
    o->state++;
}
#endif
