// VERSION PCPX_96084 tomba 800420E8 692
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of USA i0nfo overheadPlayerSetMoveAnim (src/i0nfo/overheadPlayerSetMoveAnim.c): pad bit 0x80, one walk-button animation for two direction pairs.
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *o);
extern void *D_80010C08, *D_80010C0C, *D_80010C10, *D_80010C14, *D_80010C18, *D_80010C1C, *D_80010C20, *D_80010C24;
extern void *D_80010C28, *D_80010C2C, *D_80010C30, *D_80010C34, *D_80010C38;
extern TObj *D_800CCCCC;
extern unsigned short D_800DE760;

void func_800420E8_pcpx96084(TObj *o)
{
    if (D_800DE760 & 0x80) {
        D_800CCCCC->animTimer = o->animFrame + 0x10;
        if (D_800CCCCC->animTimer == D_800CCCCC->animFrame) return;
        switch (o->animFrame) {
        case 0: o->anim = D_80010C34; break;
        case 1: o->anim = D_80010C30; break;
        case 2: o->anim = D_80010C30; break;
        case 3: o->anim = D_80010C30; break;
        case 4: o->anim = D_80010C34; break;
        case 5: o->anim = D_80010C38; break;
        case 6: o->anim = D_80010C38; break;
        case 7: o->anim = D_80010C38; break;
        case 8: o->anim = D_80010C10; break;
        case 9: o->anim = D_80010C0C; break;
        case 10: o->anim = D_80010C08; break;
        case 11: o->anim = D_80010C0C; break;
        case 12: o->anim = D_80010C10; break;
        case 13: o->anim = D_80010C14; break;
        case 14: o->anim = D_80010C18; break;
        case 15: o->anim = D_80010C14; break;
        }
    } else {
        D_800CCCCC->animTimer = o->animFrame;
        if (D_800CCCCC->animTimer == D_800CCCCC->animFrame) return;
        switch (o->animFrame) {
        case 0: o->anim = D_80010C24; break;
        case 1: o->anim = D_80010C20; break;
        case 2: o->anim = D_80010C1C; break;
        case 3: o->anim = D_80010C20; break;
        case 4: o->anim = D_80010C24; break;
        case 5: o->anim = D_80010C28; break;
        case 6: o->anim = D_80010C2C; break;
        case 7: o->anim = D_80010C28; break;
        case 8: o->anim = D_80010C10; break;
        case 9: o->anim = D_80010C0C; break;
        case 10: o->anim = D_80010C08; break;
        case 11: o->anim = D_80010C0C; break;
        case 12: o->anim = D_80010C10; break;
        case 13: o->anim = D_80010C14; break;
        case 14: o->anim = D_80010C18; break;
        case 15: o->anim = D_80010C14; break;
        }
    }
    AnimLoadDuration(o);
    D_800CCCCC->animFrame = D_800CCCCC->animTimer;
}
#endif
