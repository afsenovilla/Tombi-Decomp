// FUNC 800ec77c 384 D0SPSUB2
// MATCHING 800ec77c 384
typedef struct { char p[0x142]; short w142; char p144[2]; short w146; } Card;
extern void _card_info(int chan);
extern void _card_clear(int chan);
extern void _card_load(int chan);
extern int func_800ECE68(void);
extern void func_800ECEEC(void);
extern void func_800ECF70(void);
extern void func_800ECFC8(void);

int func_800EC77C(Card *c, int port)
{
    int chan = (port != 0) << 4;
    _card_info(chan);
    switch (func_800ECE68()) {
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
        func_800ECFC8();
        _card_clear(chan);
        func_800ECEEC();
        break;
    default:
        c->w146 = 0;
        return -2;
    }
    func_800ECF70();
    _card_load(chan);
    switch (func_800ECE68()) {
    case 0:
        c->w142 = -2;
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
        c->w142 = -2;
        c->w146 = 0;
        return -2;
    }
}
