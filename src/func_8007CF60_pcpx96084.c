// VERSION PCPX_96084 tomba 8007CF60 432
// FLAGS -O2 -G0 -fno-strength-reduce
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Bending branch, one-sided form (beta func_801365F0): the bend counters sit 4 bytes lower than in later builds.
typedef struct Par {
    char pad0[0xc2];
    unsigned short hc6;
    char pad1[6];
    unsigned short hce;
} Par;
typedef struct Obj {
    char pad0[5];
    unsigned char st;
    char pad1[0x20 - 6];
    short cnt;
    char pad2[0x69 - 0x22];
    unsigned char k;
    char pad3;
    unsigned char k2;
    char pad4[0x90 - 0x6c];
    Par *par;
    char pad5[0xb2 - 0x94];
    unsigned short hb6;
    char pad6[6];
    unsigned short hbe;
    char pad7[6];
    unsigned short hc6;
    char pad8[6];
    unsigned short hce;
} Obj;

void func_8007CF60_pcpx96084(Obj *o)
{
    Par *p;
    short c;

    switch (o->st) {
    case 0:
        if (o->k != 0) {
            o->k2 = o->k;
            o->cnt = 0;
            o->st++;
        }
        break;
    case 1:
        if (o->k == 0) {
            o->st = o->st + 1;
        } else if ((c = o->cnt) < 32) {
            o->cnt = c + 4;
            asm("");
            p = o->par;
            if (o->k == 2) {
                p->hc6++;
                o->hb6++;
                o->hc6++;
            } else if (o->k == 3) {
                p->hce++;
                o->hbe++;
                o->hce++;
            }
        }
        break;
    case 2:
        o->st = o->st + 1;
        break;
    case 3:
        if (o->cnt != 0) {
            o->cnt -= 1;
            if ((o->cnt & 3) == 0) {
                p = o->par;
                if (o->k2 == 2) {
                    p->hc6--;
                    o->hb6--;
                    o->hc6--;
                } else if (o->k2 == 3) {
                    p->hce--;
                    o->hbe--;
                    o->hce--;
                }
            }
        } else {
            o->st = 0;
        }
        break;
    }
}
#endif
