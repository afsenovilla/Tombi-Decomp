// FUNC 800203dc 108 MAIN0
typedef struct O {
    char act;
    char pad0[0x16 - 1];
    unsigned short y;
    char pad1[0x40 - 0x18];
    unsigned short *p;
} O;
extern unsigned short DAT_1f800176;
extern unsigned short DAT_1f800186;

int FUN_800203dc(O *o)
{
    return o->act != 0 && (unsigned short)(o->p[1] - DAT_1f800176 + 0x40) <= 0x1c0 && (unsigned short)(DAT_1f800186 - o->y + 0x40) < 0x171;
}
