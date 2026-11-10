// VERSION PCPX_96084 tomba 8007CD48 536
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Bending branch (PAL func_80132BB8): the bend counters sit 4 bytes lower (+0xB2.. in place of +0xB6..).
typedef struct Br {
    char p0[5]; unsigned char step; char p6[0x20 - 6];
    short w20; char p22[0x69 - 0x22];
    unsigned char b69; char p6a; unsigned char b6b; char p6c[0x90 - 0x6c];
    struct Br *d90; struct Br *d94; char p98[0xb2 - 0x98];
    unsigned short wb6; char pb8[6]; unsigned short wbe; char pc0[6];
    unsigned short wc6; char pc8[6]; unsigned short wce;
} Br;

static __inline__ void bendUp(Br *o)
{
    Br *p5 = o->d94;
    Br *p3 = o->d90;
    if (o->b69 == 2) {
        p3->wc6++;
        o->wb6++; o->wc6++;
        p5->wb6++; p5->wc6++;
    } else if (o->b69 == 3) {
        p3->wce++;
        o->wbe++; o->wce++;
        p5->wbe++; p5->wce++;
    }
}

void func_8007CD48_pcpx96084(Br *o)
{
    unsigned char st = o->step;
    Br *p5, *p3;
    short w, q;

    switch (st) {
    case 0:
        if (o->b69) {
            o->b6b = o->b69;
            o->w20 = 0;
            o->step++;
        }
        break;
    case 1:
        if (o->b69 == 0) {
            o->step = st + 1;
            break;
        }
        w = o->w20;
        if (w < 32) {
            o->w20 = w + 4;
            bendUp(o);
        }
        break;
    case 2:
        o->step = st + 1;
        break;
    case 3:
        q = o->w20;
        if (q != 0) {
            o->w20 = --q;
            if ((q & 3) == 0) {
                p5 = o->d94;
                p3 = o->d90;
                if (o->b6b == 2) {
                    p3->wc6--;
                    o->wb6--; o->wc6--;
                    p5->wb6--; p5->wc6--;
                } else if (o->b6b == 3) {
                    p3->wce--;
                    o->wbe--; o->wce--;
                    p5->wbe--; p5->wce--;
                }
            }
        } else {
            o->step = 0;
        }
        break;
    }
}
#endif
