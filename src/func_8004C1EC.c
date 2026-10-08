// FUNC 8004c1ec 556 MAIN0
// MATCHING 8004c1ec 556
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7;
    short w08;
    short w0a;
    unsigned short w0c;
    unsigned char b0e;
    signed char b0f;
} S;
extern unsigned short D_8009C960[];
extern unsigned short D_8009C962[];
extern unsigned short D_8009C962s;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009CDB5;
extern unsigned short D_80078792;
extern int D_1F800358[];
extern int D_1F80035C;
extern int D_1F800370, D_1F800374, D_1F800378;
extern void FUN_8004c570(S *);
extern void FUN_8004c6c8(S *);
extern void FUN_8004c418(S *);
extern void FUN_8004c848(S *);
extern void PoolFree_1F800210(S *);

void func_8004C1EC(S *o)
{
    unsigned char t = o->b4;

    switch (t) {
    case 0:
        o->w0c = D_8009C960[0];
        o->b0e = D_8009C962[0];
        o->b4++;
        o->b5 = 0;
        if (D_8009C960[0] == 1 && !(D_8009D2C3 & 1) && D_8009CDB5 == 0 && D_8009C962s < 2) {
            D_1F800358[0] = D_1F800370;
            D_1F80035C = D_1F800374;
            D_1F800358[D_80078792] = D_1F800378;
        }
        FUN_8004c570(o);
        FUN_8004c6c8(o);
        break;
    case 1:
        switch (o->b5) {
        case 0:
            o->b0f = o->b0e - *(unsigned char *)D_8009C962;
            if (o->b0f == 0) break;
            if (o->b0f < 0) o->b0f = 0;
            o->b0e = D_8009C962[0];
            o->w08 = 2;
            o->b5++;
            FUN_8004c418(o);
            break;
        case 1:
            if (--o->w08 == 0) {
                o->b5--;
                if (o->b0f == 0) {
                    FUN_8004c570(o);
                } else {
                    FUN_8004c848(o);
                }
            }
            break;
        }
        break;
    case 2:
        o->b4 = t - 1;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
