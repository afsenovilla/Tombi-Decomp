// FUNC 80101780 236 X017
// MATCHING 80101780 236
typedef struct O {
    char pad0[4];
    unsigned char b04;
    unsigned char step;
    char pad1[0x9e - 6];
    unsigned char b9e;
    char pad2[0xa6 - 0x9f];
    unsigned char ba6;
    char pad3;
    unsigned char wa8;
} O;
extern void playerStepHurtKnockback(), playerStepHurtStun(), playerStepHazardRespawn(), playerStepDie(), playerStepHurtSpin(), objCollideSideWalls();

void playerHurtModeDispatch(O *o)
{
    register char r;
    switch (o->step) {
    case 0: playerStepHurtKnockback(o); break;
    case 1: playerStepHurtStun(o); break;
    case 2: playerStepHazardRespawn(o); break;
    case 3: playerStepDie(o); break;
    case 4: playerStepHurtSpin(o); break;
    }
    r = 0;
    o->wa8 = 0;
    o->ba6 = 0;
    if (o->b9e == 0 && o->b04 == 5) {
        r = o->step == 0x40;
        if (o->step == 0x65)
            r++;
    }
    if (!r)
        objCollideSideWalls(o);
}
