// FUNC 80136bc0 288 X000
typedef struct Q { char p0[0x16]; short y; } Q;
typedef struct R { char p0[2]; unsigned short w2; } R;
typedef struct O { char p0[0x1c]; unsigned short w1c; char p1[2]; Q *q; } O;
extern unsigned char D_800A603C;
extern short D_800A604E;
extern R *D_800A6078;
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC, D_1F8003C4;
extern char D_800A60F8;

int func_80136BC0(O *o)
{
    int r = 0;
    if (o->q->y < -500) {
        if (D_800A604E != -0x2e3)
            goto end;
        if ((short)D_800A6078->w2 < 0xbf9)
            goto end;
        if (o->w1c) {
            r = 1;
            goto end;
        }
    } else {
        if (D_800A603C == 5)
            goto end;
        if (D_800A604E < -0x12b)
            goto end;
        if ((unsigned short)(D_800A6078->w2 - 0xa1a) >= 0x28)
            goto end;
        if (o->w1c) {
            r = 1;
            goto end;
        }
    }
    if (D_8009D2B0 == 1 && (D_1F8001FC & D_1F8003C4)) {
        r = 1;
        D_800A60F8 = 0;
        o->w1c = 1;
    }
end:
    return r;
}
