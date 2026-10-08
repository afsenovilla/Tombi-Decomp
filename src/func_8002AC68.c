// FUNC 8002ac68 480 MAIN0
// MATCHING 8002ac68 480
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7;
    int d8;
    int dc;
    int d10;
    char p14[0x3c - 0x14];
    unsigned char b3c, b3d, b3e, b3f;
    char p40[0x58 - 0x40];
    unsigned short w58;
    char p5a[0x6c - 0x5a];
    unsigned char b6c, b6d, b6e, b6f;
    unsigned char b70, b71, b72, b73;
    unsigned char b74, b75, b76, b77;
} S;
extern unsigned short D_800A6066;
extern short DAT_1f800286;
extern int D_8009C960;
extern unsigned short D_8009C960_u;
extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
extern void (*D_80079AEC[])(S *);
extern int DAT_1f800174[];
extern int DAT_1f800178[];
extern int DAT_1f80017c[];
extern int DAT_1f800180;
extern int DAT_1f800184[];
extern int DAT_1f8000e4;
extern void SetGeomScreen(int);
extern void FUN_8002757c(S *);

void func_8002AC68(S *o)
{
    unsigned short w;
    int t;

    switch (o->b4) {
    case 0:
        o->b3c = 0;
        o->b3d = 0;
        o->b3e = 0;
        w = D_800A6066;
        o->b3 = 0;
        o->b70 = 0;
        o->b71 = 0;
        o->b72 = 0;
        o->b73 = 0;
        o->b6c = 10;
        o->b6d = 0;
        o->b6e = 0;
        o->b6f = 0;
        o->b74 = 0;
        o->b75 = 0;
        o->b76 = 0;
        o->b77 = 0;
        DAT_1f800286 = 0;
        o->w58 = w;
        o->b4++;
        if (D_8009C960 == 6) {
            o->b4 = 2;
            SetGeomScreen(0x220);
        } else if (D_8009C960 == 0x60009) {
            o->b4 = 2;
            D_8009E375 = 0;
            D_8009E376 = 0;
            D_8009E377 = 0;
            D_8009F085 = 0;
            D_8009F086 = 0;
            D_8009F087 = 0;
        } else {
            SetGeomScreen(0x220);
        }
        break;
    case 1:
        D_80079AEC[D_8009C960_u](o);
        FUN_8002757c(o);
        goto tail;
    case 2:
        D_80079AEC[D_8009C960_u](o);
    tail:
        DAT_1f800174[0] = o->d8;
        t = o->dc;
        DAT_1f800178[0] = t;
        DAT_1f80017c[0] = o->d10;
        DAT_1f800184[0] = DAT_1f8000e4 + t;
        DAT_1f800180 = o->d8;
        break;
    }
}
