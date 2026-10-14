// FUNC 8006a2f4 624 MAIN0
// MATCHING 8006a2f4 624
/* Older revision of _padRecvAtLoadInfo (US psx_tomba libpad/padcmd.c): validates the AtLoadInfo replies and tracks changes in lastExtendedId (0xEE). Register pins / the empty asm volatile only steer allocation and load order (crutches). */
// FLAGS -O2 -G0
// AS gnu
#include "padlocal.h"

#define CARD_RESPONSE(obj) (*(unsigned char * volatile *)&(obj)->response_3c)

extern int _padGetActSize(padPort *);
extern int _padLoadActInfo(padPort *, u8 *);

int _padRecvAtLoadInfo(padPort *obj)
{
    register u8 *r asm("$4");
    u8 *r3;
    u8 *r4;
    int v;
    unsigned int chunk;
    register unsigned int next asm("$2");
    unsigned int responseValue;

    switch (obj->sequenceStep) {
    case 2:
        r = obj->response_3c;
        if (r[7] != 0) {
            goto return_zero;
        }
        if (obj->modeCount == r[3] && obj->currentModeIndex == r[4] && obj->actuatorCount == r[5] &&
            obj->combinationCount == r[6]) {
            obj->lastExtendedId = 0;
        } else {
            obj->lastExtendedId = 0xFFFF;
        }
        obj->modeCount = CARD_RESPONSE(obj)[3];
        obj->currentModeIndex = CARD_RESPONSE(obj)[4];
        obj->currentExtendedId = 0;
        obj->actuatorCount = CARD_RESPONSE(obj)[5];
        obj->combinationCount = CARD_RESPONSE(obj)[6];
        obj->combinationStorageBytes = 0;
        if (obj->lastExtendedId != 0) {
            goto return_zero;
        }
        obj->pad_eb[0] = 0;
        break;
    case 3:
        r3 = obj->response_3c;
        if (r3[2] != 0) {
            goto return_zero;
        }
        if (r3[3] != 0) {
            goto return_zero;
        }
        v = r3[5] + (r3[4] << 8);
        obj->currentExtendedId = v;
        if (obj->lastExtendedId != (v & 0xFFFF)) {
            obj->lastExtendedId = v;
        return_zero:
            return 0;
        }
        obj->lastExtendedId = 0xFFFF;
        obj->pad_eb[0] = 0;
        obj->infoRecordIndex = 0;
        break;
    case 4:
        r4 = obj->response_3c;
        if (r4[2] != 0) {
            goto return_zero;
        }
        if (r4[3] != 0) {
            goto return_zero;
        }
        chunk = obj->combinationStorageBytes;
        responseValue = r4[4];
        asm volatile("");
        next = obj->infoRecordIndex;
        next++;
        obj->infoRecordIndex = next;
        chunk += 8;
        chunk += (responseValue + 3) & 0x1FC;
        obj->combinationStorageBytes = chunk;
        if ((next & 0xFF) < obj->combinationCount) {
            goto return_zero;
        }
        if (_padGetActSize(obj) >= 0x81) {
            D_800981B0(obj);
            obj->sequenceStep = 0xFE;
            obj->communicationState = 2;
            goto return_zero;
        }
        if (obj->lastExtendedId != obj->combinationStorageBytes) {
            obj->lastExtendedId = obj->combinationStorageBytes;
            obj->infoRecordIndex = 0;
            obj->combinationStorageBytes = 0;
            return 0;
        }
        obj->lastExtendedId = 0;
        obj->pad_eb[0] = 0;
        obj->sequenceStep = 0xFF;
        _padLoadActInfo(obj, (u8 *)obj + 0x63);
        obj->sequenceStep = 2;
        goto return_zero;
    }
    return 1;
}
