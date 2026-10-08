// FUNC 800313c8 224 MAIN0
// MATCHING 800313c8 224
extern char DAT_80010ef8[], DAT_80010f00[], DAT_80010f08[];
extern char DAT_80010f88[], DAT_80010f90[], DAT_80010f98[];
extern char *DAT_800a605c;

void FUN_800313c8(unsigned char *o)
{
    switch (o[5]) {
    case 99:
        break;
    case 0:
        switch (*(unsigned short *)(o + 0x2e)) {
        case 0: case 1: case 2: case 3:
            DAT_800a605c = DAT_80010ef8;
            break;
        case 4: case 5:
            DAT_800a605c = DAT_80010f00;
            break;
        case 6: case 7:
            DAT_800a605c = DAT_80010f08;
        }
        break;
    case 1:
        switch (*(unsigned short *)(o + 0x2e)) {
        case 0: case 1: case 2: case 3:
            DAT_800a605c = DAT_80010f88;
            break;
        case 4: case 5:
            DAT_800a605c = DAT_80010f90;
            break;
        case 6: case 7:
            DAT_800a605c = DAT_80010f98;
        }
        break;
    }
}
