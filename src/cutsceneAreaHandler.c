// FUNC 8001baf4 484 MAIN0
// MATCHING 8001baf4 484
extern unsigned char D_8009C967;
extern unsigned char D_800B1410;
extern volatile unsigned short D_8009D674;

extern unsigned short D_8009D670;
extern int D_8009C968[];
extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
void InitSubsystems(void);
void initHud(void);
void func_800243E8(void);
void func_80021B20(void);
void FUN_8004bfe4(void);
void FUN_800263bc(void);
void FUN_8001eb64(void);
unsigned char *FUN_80018678(void);
void FUN_8001bcd8(void);
extern unsigned char *D_1F8001D4;
#define G D_1F8001D4
void cutsceneAreaHandler(void)
{
    unsigned char *e;
    volatile unsigned short *k;
    unsigned short t;
    char pad[8];
    switch (*(unsigned short *)(G + 0x4e)) {
    case 0:
        InitSubsystems();
        initHud();
        func_800243E8();
        func_80021B20();
        FUN_8004bfe4();
        FUN_800263bc();
        D_800B1410 = 0;
        *(unsigned char *)0x1F8001CF = 1;
        if (D_8009C967 != 1) FUN_8001eb64();
        (*(unsigned short *)(G + 0x4e))++;
        e = FUN_80018678();
        if (e != 0) {
            e[0] = 1;
            e[2] = 0xd;
            e[3] = 0;
            *(short *)(e + 0x12) = 0;
            *(short *)(e + 0x16) = 0;
            *(short *)(e + 0x1a) = 0;
        }
        D_8009D674 = 0;
        t = D_8009D674;
        *(unsigned short *)0x1F8001FC = 0;
        *(volatile unsigned short *)&D_8009D670 = t;
        break;
    case 1:
        D_8009C968[0]++;
        FUN_8001bcd8();
        if (*(unsigned char *)0x1F8001C2 != 0) {
            k = &D_8009D670;
            if ((*k & 8) && (*k & 0x800)) *(unsigned short *)(G + 0x4e) = 3;
        }
        break;
    case 2:
    case 3:
        D_8009E375 = 0;
        D_8009E376 = 0;
        D_8009E377 = 0;
        D_8009F085 = 0;
        D_8009F086 = 0;
        D_8009F087 = 0;
        *(unsigned short *)(G + 0x4c) = 8;
        *(unsigned short *)(G + 0x4e) = 0;
        break;
    }
}
