// FUNC 80070a00 264 MAIN0
// MATCHING 80070a00 264
// Portado de psx_tomba (psyq/libsnd/ut_gpa.c, SsUtGetProgAtr); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

short SsUtGetProgAtr(short vabId, short prog, ProgAtr* pProgAttr) {
    if (_svm_vab_used[vabId] == 1) {
        _SsVmVSetUp(vabId, prog);
        pProgAttr->tones = _svm_pg[prog].tones;
        pProgAttr->mvol = _svm_pg[prog].mvol;
        pProgAttr->prior = _svm_pg[prog].prior;
        pProgAttr->mode = _svm_pg[prog].mode;
        pProgAttr->mpan = _svm_pg[prog].mpan;
        pProgAttr->attr = _svm_pg[prog].attr;
        return 0;
    }
    return -1;
}
