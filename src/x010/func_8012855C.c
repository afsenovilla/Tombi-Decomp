// FUNC 8012855c 520 X010
// MATCHING 8012855c 520
extern unsigned char D_8009CDAC, D_8009D078, D_8009C93F;
extern unsigned short D_8009C962;
extern int D_1F8002E0;
extern char D_80132894[];
extern int ObjCullRegister(char *);
extern void FUN_80018790(char *);
extern void func_8012736C(char *);
extern void func_80127B78(char *);
extern void func_80127E08(char *);
extern void func_801280FC(char *);

void func_8012855C(char *o)
{
    unsigned char s = o[4];

    switch (s) {
    case 0:
        if (D_8009CDAC == 0xff || D_8009D078 == 4) {
            o[4] = 3;
            break;
        }
        o[0] = 2;
        *(short *)(o + 0x6c) = 10;
        *(short *)(o + 0x6e) = 0x14;
        *(short *)(o + 0x70) = 0x10;
        *(short *)(o + 0x72) = 0x20;
        *(short *)(o + 0x2e) = 1;
        *(int *)(o + 0x3c) = D_1F8002E0;
        o[0xd] = 0;
        o[0x6a] = 0;
        o[0xa] = 0;
        o[0x69] = 0;
        o[0x68] = 0;
        *(short *)(o + 0x1e) = 0xb;
        *(char **)(o + 0xa8) = D_80132894;
        o[0xa7] = 0;
        o[4]++;
        if (D_8009D078) {
            if (D_8009C962 == 0) {
                o[5] = 1;
                *(short *)(o + 0x12) = 0xc64;
                *(short *)(o + 0x16) = -0x6d;
                *(short *)(o + 0x2e) = 1;
                *(signed char *)(o + 0xf) = -7;
            } else {
                o[5] = 3;
                D_8009C93F = 1;
            }
        }
        break;
    case 1:
        ObjCullRegister(o);
        switch (o[5]) {
        case 0:
            func_8012736C(o);
            break;
        case 1:
            func_80127B78(o);
            break;
        case 2:
            func_80127E08(o);
            break;
        case 3:
            func_801280FC(o);
            break;
        }
        break;
    case 2:
    case 3:
        FUN_80018790(o);
        break;
    }
}
