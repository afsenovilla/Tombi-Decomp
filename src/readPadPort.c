// FUNC 8002623c 384 MAIN0
// MATCHING 8002623c 384
typedef struct {
    unsigned short type;
    char pad[6];
    unsigned char x;
    unsigned char y;
    char pad2[2];
} PadInfo;

extern PadInfo PAD_INFO[];
extern unsigned char PAD_BUFFERS[];
extern unsigned char DAT_8009f812[];
extern void PadInfoMode(int, int, int);

short readPadPort(short port)
{
    PadInfo *p = &PAD_INFO[port];
    unsigned short btn;
    unsigned short ty;
    unsigned short a, b, m;

    switch (port) {
    case 0:
        if (PAD_BUFFERS[0] != 0)
            return 0;
        PadInfoMode(0, 2, 0);
        btn = ~*(unsigned short *)&PAD_BUFFERS[2];
        ty = (PAD_BUFFERS[1] >> 4) & 7;
        break;
    case 1:
        if (DAT_8009f812[0] != 0)
            return 0;
        PadInfoMode(1, 2, 0);
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
            p->x = PAD_BUFFERS[6];
            p->y = PAD_BUFFERS[7];
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
