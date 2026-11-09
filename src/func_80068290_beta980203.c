// VERSION BETA_980203 beta980203 80068290 332
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/_padRecvAtLoadInfo_scus94236.c (_padRecvAtLoadInfo, twin score 0.900) [-O2 -G0] (tools/demo_sweep.py)
// Portado de psx_tomba (psyq/libpad/padcmd.c, _padRecvAtLoadInfo); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
#define CARD_RESPONSE(obj) \
    (*(unsigned char * volatile *)&(obj)->response_3c)
int _padRecvAtLoadInfo(padPort *obj) {
    unsigned int chunk;
    register unsigned int next asm("$2");
    unsigned int responseValue;
    switch (obj->sequenceStep) {
    case 2:
        obj->modeCount = CARD_RESPONSE(obj)[3];
        obj->currentModeIndex = CARD_RESPONSE(obj)[4];
        obj->currentExtendedId = 0;
        obj->actuatorCount = CARD_RESPONSE(obj)[5];
        obj->combinationCount = CARD_RESPONSE(obj)[6];
        obj->combinationStorageBytes = 0;
        break;

    case 3:
        responseValue = obj->response_3c[4];
        next = obj->response_3c[5];
        obj->infoRecordIndex = 0;
        obj->currentExtendedId = (responseValue << 8) + next;
        break;

    case 4:
        chunk = obj->combinationStorageBytes;
        next = obj->infoRecordIndex;
        responseValue = obj->response_3c[4];
        next++;
        obj->infoRecordIndex = next;
        chunk += 8;
        chunk += (responseValue + 3) & 0x1FC;
        obj->combinationStorageBytes = chunk;
        if ((next & 0xFF) < obj->combinationCount) {
return_zero:
            return 0;
        }

        if (_padGetActSize(obj) >= 0x81) {
            obj->sequenceStep = 0xFE;
            obj->communicationState = 2;
            goto return_zero;
        }

        obj->sequenceStep = 0xFF;
        _padLoadActInfo(obj, (unsigned char *)obj + 0x63);
        obj->sequenceStep = 2;
        goto return_zero;
    }

    return 1;
}
#endif
