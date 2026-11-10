// VERSION PCPX_96084 tomba 80035D58 384
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x017 updateFlashEffect: the sound call takes a leading 0.
#include "TOBJ.H"
extern void func_80018550(TObj *);
extern int func_800180D8_pcpx96084(TObj *);
extern void func_80022FC8(int, int, int);
extern void func_8001A87C_pcpx96084(TObj *);

void updateFlashEffect(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        func_80018550(o);
        if (o->visible) {
            switch (o->subtype) {
            case 0:
                switch (o->state) {
                case 0:
                    o->state++;
                    break;
                case 1:
                    func_800180D8_pcpx96084(o);
                    break;
                }
                break;
            case 1:
                switch (o->state) {
                case 0:
                    func_80022FC8(0, 0xe, 4);
                    o->state++;
                    break;
                case 1:
                    if (func_800180D8_pcpx96084(o))
                        o->b04 = 2;
                    break;
                }
                break;
            case 2:
                switch (o->state) {
                case 0:
                    func_80022FC8(0, 0xe, 0);
                    o->state++;
                    break;
                case 1:
                    if (func_800180D8_pcpx96084(o))
                        o->b04 = 2;
                    break;
                }
                break;
            }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
