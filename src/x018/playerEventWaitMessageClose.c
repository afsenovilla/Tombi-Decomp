// FUNC 80113a8c 224 X018
// MATCHING 80113a8c 224
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
extern char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP, DAT_8009c942, DAT_8009c93f;
extern short DAT_1f8001c6, DAT_800a60ea;
extern int DAT_800a6094;
extern void FUN_8001fec0();

void playerEventWaitMessageClose(O *o)
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
        PLAYER_STATE = 1;
        DAT_1f8001c6 = 0;
        DAT_8009c942 = 0;
        DAT_800a60ea = 0;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        if (DAT_800a6094 == 1) {
            DAT_8009c93f = 0;
            DAT_800a6094 = 0;
        }
        o->b68 = 0;
        o->b6a = 0;
        o->step = 0;
        o->state = 0;
    }
}
