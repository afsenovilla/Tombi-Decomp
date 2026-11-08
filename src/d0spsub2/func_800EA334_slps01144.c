// VERSION SLPS_01144 dspsub2 800EA334 348
#if defined(VERSION_SLPS_01144)
// JP memCardProbe (PAL d0spsub2 800ec77c): no w142 writes
typedef struct { char p[0x142]; short w142; char p144[2]; short w146; } Card;
extern void func_80059B38(int chan); /* _card_info */
extern void func_80059B58(int chan); /* _card_clear */
extern void func_80059B48(int chan); /* _card_load */
extern int memCardWaitHwEvent(void);
extern void memCardWaitSwEvent(void);
extern void memCardClearHwEvents(void);
extern void memCardClearSwEvents(void);

int func_800EA334(Card *c, int port)
{
    int chan = (port != 0) << 4;
    func_80059B38(chan);
    switch (memCardWaitHwEvent()) {
    case 0:
        if (c->w146 > 0) return 0;
        if (c->w146 == -1) return 1;
        break;
    case 1:
        c->w146 = 0;
        return -2;
    case 2:
        c->w146 = 0;
        return -1;
    case 3:
        if (c->w146 == -1) return 1;
        memCardClearSwEvents();
        func_80059B58(chan);
        memCardWaitSwEvent();
        break;
    default:
        c->w146 = 0;
        return -2;
    }
    memCardClearHwEvents();
    func_80059B48(chan);
    switch (memCardWaitHwEvent()) {
    case 0:
        c->w146 = 1;
        return 0;
    case 2:
        c->w146 = 0;
        return -1;
    case 3:
        c->w146 = -1;
        return 1;
    case 1:
    default:
        c->w146 = 0;
        return -2;
    }
}
#endif
