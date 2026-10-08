// FUNC 800eaffc 204 X002
// MATCHING 800eaffc 204
typedef struct Q {
    unsigned char b0, b1, b2, b3;
    char pad4[0x10];
    int w14;
    char pad5[8];
    short s20;
    char pad6[0x40 - 0x22];
    int *p40;
    int *p44;
} Q;
extern Q *FUN_80018448(void);
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern unsigned char DAT_8009d2c3;

void FUN_800eaffc(int a, int b, int c)
{
    Q *q;
    if (DAT_8009c960 == 1 && DAT_8009c962 < 2 && (DAT_8009d2c3 & 1) == 0 && (q = FUN_80018448()) != 0) {
        q->b0 = 1;
        q->b2 = 0xc;
        q->b3 = 2;
        *q->p40 = a << 16;
        q->w14 = b << 16;
        *q->p44 = c << 16;
        q->s20 = 8;
    }
}
