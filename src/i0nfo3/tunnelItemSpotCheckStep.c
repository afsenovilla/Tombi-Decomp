// FUNC 800ec820 436 I0NFO3
// MATCHING 800ec820 436
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void func_80026C50(int item, int a, int b);
extern void func_8004D620(int a, int b);
extern void func_80020AEC(int bit);
extern void func_80026A10(int ap);
extern void func_8005A9A4(int e, int a);
extern unsigned char D_8009D0AA;
extern unsigned char D_8009D0C8;
extern unsigned char D_8009D0D0;
extern unsigned char D_8009D0D1;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned char D_800A60E4;
extern unsigned char TUNNEL_ITEM_SPOT_ITEMS[][2];
extern void *TUNNEL_ITEM_SPOT_ANIMS[][8];

void tunnelItemSpotCheckStep(TObj *o)
{
    unsigned char s = o->state;
    unsigned char ok;
    switch (s) {
    case 0:
        o->state = s + 1;
        o->b68 = 0;
        o->anim = TUNNEL_ITEM_SPOT_ANIMS[o->b0c][0];
        func_8001FE6C(o);
        break;
    case 1:
        if (o->b68 != 0) {
            switch (o->b0c) {
            case 0:
                ok = D_8009D0AA;
                break;
            case 1:
                ok = D_8009D0C8;
                break;
            case 2:
                ok = D_8009D0D0;
                break;
            case 3:
                ok = D_8009D0D1;
                break;
            default:
                func_8004D620(0, 2);
                ok = 0;
                break;
            }
            if (ok) {
                if (o->subtype == 7) {
                    func_8004D620(0x4E, 0);
                    func_80026A10(0x493E0);
                    func_8005A9A4(0xA1, 1);
                } else {
                    func_80026C50(TUNNEL_ITEM_SPOT_ITEMS[o->subtype][0], TUNNEL_ITEM_SPOT_ITEMS[o->subtype][1], 1);
                }
                func_80020AEC(o->b6b);
                o->step = 2;
                o->state = 0;
            }
            D_800A60E4 = 0;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
        }
        o->b68 = 0;
        break;
    }
}
