// VERSION PCPX_96084 tomba 80022034 244
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Memory card probe of port 0 / 1: card info (a new card is cleared first), then a load test. 0 ready,
// 1 unformatted, -1 no card, -2 error.
extern void _card_info(int);
extern void _card_clear(int);
extern void _card_load(int);
extern int func_80022398_pcpx96084(void);
extern void func_800224F8_pcpx96084(void);
extern void func_8002241C_pcpx96084(void);
extern void memCardClearHwEvents(void);

int func_80022034_pcpx96084(int port)
{
    int ch = (port != 0) << 4;
    _card_info(ch);
    switch (func_80022398_pcpx96084()) {
    case 3:
        func_800224F8_pcpx96084();
        _card_clear(ch);
        func_8002241C_pcpx96084();
    case 0:
        break;
    case 1:
        return -2;
    case 2:
        return -1;
    default:
        return -2;
    }
    memCardClearHwEvents();
    _card_load(ch);
    switch (func_80022398_pcpx96084()) {
    case 0:
        return 0;
    case 1:
        return -2;
    case 2:
        return -1;
    case 3:
        return 1;
    }
    return -2;
}
#endif
