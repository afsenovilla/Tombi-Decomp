// FUNC 800e87bc 280 D0SPSUB
typedef struct { unsigned short a, b; } Pair;
extern Pair D_8007A024[];
extern unsigned char D_8009C990, D_8009CF06, D_8009D006, D_8009D090, D_8009D2B3;
extern char D_800A603B;
extern unsigned short D_800A6040, D_800A60FC;

void func_800E87BC(void)
{
    if (D_8009D006 == 1) {
        D_8009D2B3 = D_8009D090;
        if (D_8009CF06 == 0) {
            D_800A6040 = D_8007A024[D_8009C990].a;
            D_800A603B = 0;
            D_800A60FC = D_8007A024[D_8009D090].b;
        } else if (D_8009C990 == 1 || D_8009C990 == 2 || D_8009C990 == 3) {
            D_800A6040 = D_8007A024[D_8009C990].a;
            D_800A603B = 0;
            D_800A60FC = D_8007A024[D_8009D090].b;
        } else {
            D_800A603B = 1;
            D_800A6040 = D_8007A024[4].a;
            D_800A60FC = D_8007A024[4].b;
        }
        D_8009D006 = 0;
    }
}
