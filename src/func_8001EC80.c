// FUNC 8001ec80 888 MAIN0
// MATCHING 8001ec80 888
/* debt: register asm on v; without it CSE/local-alloc make x a block-local copy, global alloc gives v x's s0 (game: v in a1).
   Tried: v/x int/short/ushort combos, x set earlier, x = v in per-case tails with goto, SsSeqSetVol(v, v).
   w: the tail stores written in every case (cross-jumped) give w the refs to win over the switch value (was asm $3).
   c: the call written in both branches lets the byte shift tie to a1 (was asm $5). */
extern short D_800A3428;
extern unsigned char D_800784C8, D_800784C9;
extern unsigned char D_800784CA[];
extern unsigned short D_1F8003A8[];
extern short D_8009BD0C, D_8009BD24, D_8009BD28, D_8009BD14, D_8009BD10;
extern unsigned short D_8009C8F0;
extern void *D_8009D630[];
extern unsigned short D_8009C960;
extern unsigned short D_8009C962;
extern short D_8007833C[];
extern unsigned char *D_80078428[];
extern int D_80078478[];
void SsSeqStop(short a);
void SsSeqClose(short a);
short SsSeqOpen(void *p, short v);
void SsSetMVol(short a, short b);
void SsSeqSetVol(short a, short b, short c);
void SsSeqPlay(short a, int b, int c);
void FUN_8001f5b8(int a, short b);

int func_8001EC80(int arg)
{
    unsigned short *p;
    unsigned short t;
    register int v asm("$5");
    int w;
    int x;
    int b;
    if (D_800A3428 != -1) {
        SsSeqStop(D_800A3428);
        SsSeqClose(D_800A3428);
        D_800A3428 = -1;
    }
    D_8009BD0C = 0;
    t = D_1F8003A8[D_800784C9];
    D_8009C8F0 = t;
    D_800A3428 = SsSeqOpen(D_8009D630[D_800784C8], t);
    SsSetMVol(100, 100);
    p = &D_8009C960;
    if (D_800784CA[*p * 4] != 0) {
        SsSeqSetVol(D_800A3428, 0, 0);
        D_8009BD24 = 0;
        w = D_8007833C[*p];
        switch (D_8009C960) {
        case 0:
        case 2:
            w += 5;
            D_8009BD28 = w;
            D_8009BD14 = 1;
            goto done;
        case 6:
            switch (D_8009C962) {
            case 0: w -= 5; D_8009BD28 = w; D_8009BD14 = 1; goto done;
            case 1: w += 12; D_8009BD28 = w; D_8009BD14 = 1; goto done;
            case 2: w += 12; D_8009BD28 = w; D_8009BD14 = 1; goto done;
            }
            break;
        case 9:
            switch (D_8009C962) {
            case 0: w -= 11; D_8009BD28 = w; D_8009BD14 = 1; goto done;
            case 1: case 2: case 3: case 4: case 5: case 6: w += 10; D_8009BD28 = w; D_8009BD14 = 1; goto done;
            }
            break;
        }
        D_8009BD28 = w;
        D_8009BD14 = 1;
    done:;
    } else {
        v = D_8007833C[*p];
        switch (D_8009C960) {
        case 0:
        case 2:
            v += 5;
            break;
        case 6:
            switch (D_8009C962) {
            case 0: v -= 5; break;
            case 1: v += 12; break;
            case 2: v += 12; break;
            }
            break;
        case 9:
            switch (D_8009C962) {
            case 0: v -= 11; break;
            case 1: case 2: case 3: case 4: case 5: case 6: v += 10; break;
            }
            break;
        }
        x = v;
        SsSeqSetVol(D_800A3428, x, x);
        D_8009BD10 = x;
        D_8009BD14 = 0;
    }
    b = (short)arg;
    if (b < 0) {
        unsigned char *t = D_80078428[D_8009C960];
        FUN_8001f5b8(D_80078478[D_8009C960], (short)(t[D_8009C962] << 8));
    } else {
        FUN_8001f5b8(1, b);
    }
    SsSeqPlay(D_800A3428, 1, 1);
    return 0;
}
