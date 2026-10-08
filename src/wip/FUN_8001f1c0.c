// FUNC 8001f1c0 300 MAIN0
extern short DAT_800a3428;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c960b;
extern unsigned short DAT_8009c962;
extern short DAT_80078334[];
extern short DAT_8009bd0c;
extern void SsSeqSetVol(short, short, short);

void FUN_8001f1c0(void)
{
    int v;
    short sv;
    short d;

    if (DAT_800a3428 == -1)
        return;
    v = DAT_80078334[DAT_8009c960];
    switch (DAT_8009c960b) {
    case 0:
    case 2:
        v = v + 5;
        break;
    case 6:
        switch (DAT_8009c962) {
        case 0:
            v = v - 5;
            break;
        case 1:
            v = v + 12;
            break;
        case 2:
            v = v + 12;
            break;
        }
        break;
    case 9:
        switch (DAT_8009c962) {
        case 0:
            v = v - 11;
            break;
        case 1: case 2: case 3: case 4: case 5: case 6:
            v = v + 10;
            break;
        }
        break;
    }

    sv = v;
    d = DAT_800a3428;
    SsSeqSetVol(d, sv, sv);
    DAT_8009bd0c = 0;
}
