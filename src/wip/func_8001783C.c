// FUNC 8001783c 776 MAIN0
/* score 6 (was 74 under current ncheck): b45 random hill-climb + exhaustive swap/move/volatile-toggle descent over
   the pre-if store order and per-global volatile mask. Register pattern matches the game; only the stores to
   DAT_1f8001c8 (game +0x128) and D_8009CEAF (game +0x158) are exchanged. Tried every position pair of those two lines
   x volatile toggles: none better. Volatile externs are debt (volatile mems are chained in sched, which keeps the
   zero stores early as in the game).
   b50: simulated annealing over statement order x per-global kind {scalar, [0] array (in-struct: ordered against
   the sh 0(s0) store), volatile, volatile array} from this version and from game order: nothing below 6; the CEAF
   store position is chaotic (only index after C980 works); moving 1c8 before C973 gives 7. */
/* o36: all CEAF/CDA4 position pairs x {CEAF,CDA4} volatile: nothing below 6. */
extern volatile unsigned short D_8009C960[];
extern char D_8009C930[];
extern char D_1F8000C0[];
extern unsigned short D_8009F3DC;
extern unsigned char DAT_1f8001b3;
extern unsigned char DAT_1f8001ab;
extern short D_8009D2A4;
extern unsigned short D_8009D2A6;
extern short DAT_1f8000ea;
extern volatile short DAT_1f8000ee;
extern volatile short DAT_1f8000f2;
extern int DAT_1f800200;
extern volatile short DAT_1f8000e2;
extern volatile short DAT_1f8000e6;
extern volatile short DAT_1f8000f6;
extern unsigned char DAT_1f8003d2;
extern unsigned char DAT_1f8003d3;
extern volatile int D_8009C96C;
extern volatile unsigned char D_8009C977;
extern unsigned char D_8009D090;
extern unsigned char D_8009C980;
extern unsigned char D_8009C972;
extern volatile unsigned char D_8009C971;
extern volatile unsigned char D_8009C970;
extern short D_8009CFD0;
extern volatile short D_8009CFD2;
extern unsigned char DAT_1f8003ce;
extern volatile short DAT_1f8001c8;
extern unsigned char DAT_1f8003d1;
extern short D_8009F838;
extern short D_8009C962;
extern unsigned char D_8009CDA3;
extern volatile unsigned char D_8009C973;
extern unsigned char D_8009CEAF;
extern unsigned char D_8009CDA4;
extern short D_8009CFD4;
extern unsigned char D_8009CF1C;
extern unsigned char D_8009D1A4;
extern unsigned char D_8009D1A5;
extern unsigned char D_8009D1A6;
extern unsigned char D_8009D1A7;
extern unsigned char D_8009D1A8;
extern unsigned char D_8009D1A9;
extern unsigned char D_8009D1AA;
extern unsigned char D_8009D1AB;
extern unsigned char D_8009D1AC;
extern unsigned char D_8009D1AD;
extern unsigned char D_8009D1AE;
extern unsigned char D_8009D1AF;
extern unsigned char D_8009D1B0;
extern unsigned char D_8009D1B1;
extern unsigned char D_8009D0BF;
extern unsigned char D_8009D0C5;
extern unsigned char D_8009D0C6;
extern unsigned char D_8009D0B9;
extern unsigned char D_8009D0BA;
extern unsigned char D_8009D0BB;
extern unsigned char D_8009D0BC;
extern unsigned char D_8009D0C4;
extern unsigned char D_8009D0C1;
extern unsigned char D_8009D0C0;
extern unsigned char D_8009D0A9;
extern unsigned char D_8009D0AA;
extern unsigned char D_8009D0C8;
extern unsigned char D_8009D0B8;
extern void *memset(void *, int, int);
extern void FUN_80021f5c(void *);

void func_8001783C(void)
{
    memset(D_8009C960, 0, 0x988);
    memset(D_8009C930, 0, 0x2c);
    FUN_80021f5c(D_1F8000C0);
    DAT_1f8000ea = -544;
    DAT_1f8000ee = 160;
    D_8009CF1C = D_8009F3DC;
    DAT_1f8000f2 = -128;
    DAT_1f8000e2 = 0;
    DAT_1f8000e6 = 0;
    DAT_1f800200 = 69;
    DAT_1f8003ce = 0;
    DAT_1f8000f6 = 0;
    DAT_1f8003d2 = 255;
    DAT_1f8003d3 = 255;
    D_8009C96C = 0;
    D_8009C960[0] = 0;
    D_8009C977 = 255;
    D_8009C980 = 3;
    D_8009CEAF = 1;
    D_8009C972 = 9;
    D_8009C971 = 4;
    D_8009C970 = 4;
    DAT_1f8003d1 = 0;
    D_8009CFD0 = 1498;
    D_8009F838 = 0;
    D_8009C962 = 0;
    D_8009CFD2 = 1593;
    D_8009D090 = 255;
    D_8009CDA3 = 0;
    D_8009C973 = 1;
    DAT_1f8001c8 = 0;
    D_8009CDA4 = 1;
    D_8009CFD4 = 1681;
    if (DAT_1f8001b3 == 0) {
        if (DAT_1f8001ab != 0) {
            D_8009D1A4 = 27;
            D_8009D1A5 = 33;
            D_8009D1A6 = 34;
            D_8009D1A7 = 21;
            D_8009D1A8 = 22;
            D_8009D1A9 = 23;
            D_8009D1AA = 32;
            D_8009D1AB = 29;
            D_8009D1AC = 24;
            D_8009D1AD = 28;
            D_8009D1AE = 5;
            D_8009D1AF = 6;
            D_8009D1B0 = 36;
            D_8009D1B1 = 20;
            D_8009D2A4 = 14;
            D_8009D0BF = 1;
            D_8009D0C5 = 1;
            D_8009D0C6 = 1;
            D_8009D0B9 = 1;
            D_8009D0BA = 1;
            D_8009D0BB = 1;
            D_8009D0BC = 1;
            D_8009D0C4 = 1;
            D_8009D0C1 = 1;
            D_8009D0C0 = 1;
            D_8009D0A9 = 1;
            D_8009D0AA = 1;
            D_8009D0C8 = 1;
            D_8009D0B8 = 1;
            D_8009D2A6 = 0x8000;
        } else {
            D_8009D1A4 = 27;
            D_8009D1A5 = 32;
            D_8009D2A4 = 2;
            D_8009D0BF = 1;
            D_8009D0C4 = 1;
            D_8009D2A6 = 0x8000;
        }
    }
}
