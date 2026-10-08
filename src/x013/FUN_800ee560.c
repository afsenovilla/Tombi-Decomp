// FUNC 800ee560 104 X013
// MATCHING 800ee560 104
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

void FUN_800ee560(O *o)
{
    FUN_8001e5f4(0x1c, 0x7f);
    o->b9c = 0;
    o->anim = DAT_80010c50;
    FUN_8001fe94(o, 4);
    o->d30 = 0;
    o->d34 = 0;
    o->d88 = 0;
    o->d8c = 0;
    o->baa = 0;
    o->state = 5;
}
