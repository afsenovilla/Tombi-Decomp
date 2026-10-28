// FUNC 8006b464 352 MAIN0
// MATCHING 8006b464 352
/* PadInitDirect (US psx_tomba libpad/padportd.c): installs the direct-mode pad object table. */
// FLAGS -O2 -G0
// AS gnu
#include "padlocal.h"

int PadInitDirect(u8 *output0, u8 *output1) {
    u8 *savedOutput0 = output0;
    u8 *savedOutput1 = output1;
    register padPort *obj asm("$16");
    u8 *table3c;
    u8 *table40;
    int i;
    int fill;
    register u8 **field40 asm("$4");
    u8 *cursor;
    void (*swapCommand)(padPort *);
    padPort *clearObj;
    int count;

    D_800981E4 = 0;
    D_800981F8 = 0;
    _padInitDirSeq();

    obj = &D_8009C038[0];
    D_800981AC = padDirNextPort;
    D_800981B0 = padResetPortState;
    D_800981B4 = padGetSendByte;
    D_800981B8 = padBuildActuatorMap;
    D_800981BC = padGetPortId;
    D_800981C0 = padGetPortBuffer;
    swapCommand = _padLatchId;
    asm volatile("" : "=r"(clearObj) : "0"(obj));
    D_800981E0 = obj;
    D_800981D0 = swapCommand;

    bzero(clearObj, sizeof(padPort) * 2);
    i = 0;
    fill = 0xFF;
    field40 = &obj->statusBuffer;
    table40 = D_8009BFF0;
    table3c = D_8009BFA8;
    cursor = (u8 *)&obj[0].output_30;
    *(u8 **)cursor = savedOutput0;
    cursor = (u8 *)&obj[1].output_30;
    *(u8 **)cursor = savedOutput1;
    do {
        PADPORT_FROM_STATUS_BUFFER(field40)->subPorts = 0;
        PADPORT_FROM_STATUS_BUFFER(field40)->ownerPort = obj;
        PADPORT_FROM_STATUS_BUFFER(field40)->output_30[0] = fill;
        PADPORT_FROM_STATUS_BUFFER(field40)->output_30[1] = 0;
        PADPORT_FROM_STATUS_BUFFER(field40)->response_3c = table3c;
        *field40 = table40;
        cursor = obj->actuatorMap;
        count = 5;
        do {
            *cursor = fill;
            count--;
            cursor++;
        } while (count >= 0);
        table40 += 0x23;
        table3c += 0x23;
        i++;
        field40 = &PADPORT_FROM_STATUS_BUFFER(field40)[1].statusBuffer;
        obj++;
    } while (i < 2);

    _padSetVsyncParam();
    D_800981E4 = 1;
    return 1;
}
