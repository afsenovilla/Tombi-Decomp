// VERSION SLPS_01144 x01 8011AD50 916
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define I32(p, o) (*(int *)((p) + (o)))
#define PTR(p, o) (*(unsigned char **)((p) + (o)))
extern void ObjCullRegister(unsigned char *);
extern void func_8011AC38_slps01144(unsigned char *);
extern void ObjFreeDup(unsigned char *);
extern unsigned char D_8009E95D, D_8009E94A;
extern unsigned short D_8009E93E, GAME_SELECTED_SECTION;

void func_8011AD50_slps01144(unsigned char *o)
{
    switch (o[4]) {
    case 0:
        o[4]++;
        I32(o, 0x84) = 0;
        I32(o, 0x88) = 0;
        I32(o, 0x8c) = 0;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o[3]) {
        case 0:
            switch (o[6]) {
            case 0:
                if (D_8009E95D == 0) {
                    o[6] = 3;
                    break;
                }
                if (*(short *)0x1f80016e < -320) break;
                if ((unsigned short)(*(short *)(PTR(o, 0x40) + 2) - *(unsigned short *)0x1f80016a + 64) < 128)
                    o[6]++;
                break;
            case 1:
                I32(o, 0x88) = (I32(o, 0x88) - 16) & 0xfff;
                if (I32(o, 0x88) < 3073) o[6]++;
                break;
            case 2:
                if (o[1] == 0) {
                    I32(o, 0x88) = 0;
                    o[6] = 0;
                }
                break;
            }
            break;
        case 1:
            func_8011AC38_slps01144(o);
            break;
        case 2:
            switch (o[6]) {
            case 0:
                switch (GAME_SELECTED_SECTION) {
                case 0:
                case 1:
                    o[6]++;
                    break;
                case 2:
                    I32(o, 0x88) = 0x800;
                    o[6] = 4;
                    break;
                }
                break;
            case 1:
                o[6] = PTR(o, 0x90)[6];
                break;
            case 2:
                I32(o, 0x88) = (I32(o, 0x88) - 8) & 0xfff;
                if (I32(o, 0x88) < 2561) o[6]++;
                break;
            case 3:
                if (o[1] == 0 && PTR(o, 0x90)[1] == 0) {
                    I32(o, 0x88) = 0;
                    o[6] = 1;
                }
                break;
            case 4:
                break;
            }
            break;
        case 3:
            switch (o[6]) {
            case 1:
                I32(o, 0x88) = (I32(o, 0x88) - 24) & 0xfff;
                if (I32(o, 0x88) < 2561) o[6]++;
                break;
            case 0:
                if (D_8009E93E != 2 && D_8009E94A) o[6]++;
                break;
            case 2:
                break;
            }
            break;
        case 4:
            switch (o[6]) {
            case 0:
                o[6] = PTR(o, 0x90)[6];
                break;
            case 1:
                I32(o, 0x88) = (I32(o, 0x88) + 24) & 0xfff;
                if (I32(o, 0x88) >= 1536) o[6]++;
                break;
            case 2:
                break;
            }
            break;
        case 5:
            switch (o[6]) {
            case 0:
                if (D_8009E93E != 4 && D_8009E94A) o[6]++;
                break;
            case 1:
                I32(o, 0x88) = (I32(o, 0x88) + 24) & 0xfff;
                if (I32(o, 0x88) >= 1536) o[6]++;
                break;
            case 2:
                break;
            }
            break;
        case 6:
            switch (o[6]) {
            case 0:
                o[6] = PTR(o, 0x90)[6];
                break;
            case 1:
                I32(o, 0x88) = (I32(o, 0x88) - 24) & 0xfff;
                if (I32(o, 0x88) < 2561) o[6]++;
                break;
            case 2:
                break;
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
#endif
