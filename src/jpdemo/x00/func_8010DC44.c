// JPDEMO x00 8010DC44 200
// Demo version of PAL x000 80113A8C (FUN_80113a8c.c): the player is released only when GVARS.b3f is clear.
typedef struct O {
    char pad0[5];
    unsigned char step;
    unsigned char state;
    char pad1[0x68 - 7];
    unsigned char b68;
    char pad2;
    unsigned char b6a;
    char pad3[0x90 - 0x6b];
    char *d90;
} O;
extern char DAT_800a603c, DAT_800a603d, DAT_800a603e, DAT_8009c942, DAT_8009c93f;
extern short DAT_1f8001c6, DAT_800a60ea;
extern int DAT_800a6094;
extern void FUN_8001fec0();

void FUN_80113a8c(O *o)
{
    char pad[16];
    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
    case 1:
        break;
    default:
        return;
    }
    FUN_8001fec0(o);
    if (o->d90[4] == 2) {
        o->d90[4] = 3;
        {
            unsigned char f = DAT_8009c93f;
            DAT_1f8001c6 = 0;
            DAT_8009c942 = 0;
            DAT_800a60ea = 0;
            if (f == 0) {
                DAT_800a603c = 1;
                DAT_800a603d = 0;
                DAT_800a603e = 0;
            }
        }
        o->b68 = 0;
        o->b6a = 0;
        o->step = 0;
        o->state = 0;
    }
}
