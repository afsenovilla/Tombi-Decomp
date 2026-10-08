// FUNC 8002ac68 480 MAIN0
extern unsigned short DAT_800a6066;
extern short DAT_1f800286;
extern int DAT_8009c960;
extern unsigned short DAT_8009c960_u;
extern unsigned char DAT_8009e375, DAT_8009e376, DAT_8009e377;
extern unsigned char DAT_8009f085, DAT_8009f086, DAT_8009f087;
extern void (*DAT_80079aec[])(unsigned char *);
extern void FUN_80063ffc(int);
extern void FUN_8002757c(unsigned char *);
extern int DAT_1f800174, DAT_1f800178, DAT_1f80017c, DAT_1f800180, DAT_1f800184, DAT_1f8000e4;

void FUN_8002ac68(unsigned char *o)
{
    switch (o[4]) {
    case 0:
        o[0x3c] = 0;
        o[0x3d] = 0;
        o[0x3e] = 0;
        o[3] = 0;
        o[0x70] = 0;
        o[0x71] = 0;
        o[0x72] = 0;
        o[0x73] = 0;
        o[0x6c] = 10;
        o[0x6d] = 0;
        o[0x6e] = 0;
        o[0x6f] = 0;
        o[0x74] = 0;
        o[0x75] = 0;
        o[0x76] = 0;
        o[0x77] = 0;
        DAT_1f800286 = 0;
        *(unsigned short *)(o + 0x58) = DAT_800a6066;
        o[4]++;
        if (DAT_8009c960 == 6) {
            o[4] = 2;
        } else if (DAT_8009c960 == 0x60009) {
            o[4] = 2;
            DAT_8009e375 = 0;
            DAT_8009e376 = 0;
            DAT_8009e377 = 0;
            DAT_8009f085 = 0;
            DAT_8009f086 = 0;
            DAT_8009f087 = 0;
            break;
        }
        FUN_80063ffc(0x220);
        break;
    case 1:
        DAT_80079aec[DAT_8009c960_u](o);
        FUN_8002757c(o);
        goto tail;
    case 2:
        DAT_80079aec[DAT_8009c960_u](o);
    tail:
        DAT_1f800174 = *(int *)(o + 8);
        DAT_1f800178 = *(int *)(o + 0xc);
        DAT_1f800184 = DAT_1f8000e4 + *(int *)(o + 0xc);
        DAT_1f80017c = *(int *)(o + 0x10);
        DAT_1f800180 = *(int *)(o + 8);
        break;
    }
}
