// FUNC 8002623c 384 MAIN0
// MATCHING 8002623c 384
typedef struct {
    unsigned short type;
    char pad[6];
    unsigned char x;
    unsigned char y;
    char pad2[2];
} PadInfo;

extern PadInfo DAT_8009d610[];
extern unsigned char DAT_8009f7f0[];
extern unsigned char DAT_8009f812[];
extern void FUN_8006911c(int, int, int);

short FUN_8002623c(short port)
{
    PadInfo *p = &DAT_8009d610[port];
    unsigned short btn;
    unsigned short ty;
    unsigned short a, b, m;

    switch (port) {
    case 0:
        if (DAT_8009f7f0[0] != 0)
            return 0;
        FUN_8006911c(0, 2, 0);
        btn = ~*(unsigned short *)&DAT_8009f7f0[2];
        ty = (DAT_8009f7f0[1] >> 4) & 7;
        break;
    case 1:
        if (DAT_8009f812[0] != 0)
            return 0;
        FUN_8006911c(1, 2, 0);
        btn = ~*(unsigned short *)&DAT_8009f812[2];
        ty = (DAT_8009f812[1] >> 4) & 7;
        break;
    default:
        return 0;
    }
    p->type = ty;
    switch (ty) {
    case 4:
        p->x = 0x80;
        p->y = 0x80;
        break;
    case 7:
        if (port == 0) {
            p->x = DAT_8009f7f0[6];
            p->y = DAT_8009f7f0[7];
            if ((btn & 0xf0) == 0) {
                m = btn & 0xff0f;
                a = (p->x == 0) << 7;
                if (p->x == 0xff)
                    a = 0x20;
                btn = a | m;
                b = (p->y == 0) << 4;
                if (p->y == 0xff)
                    b = 0x40;
                btn |= b;
            }
        }
        break;
    default:
        return 0;
    }
    return btn;
}
