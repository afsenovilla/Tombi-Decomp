// FUNC 80123b64 188 X000
// MATCHING 80123b64 188
typedef struct Q {
    unsigned char b0, b1, b2, b3;
    char pad4[8];
    unsigned char bc;
    char pad5[2];
    unsigned char bf;
    char pad6[4];
    int w14;
    char pad7[5];
    unsigned char b1d;
    char pad8[0x2e - 0x1e];
    short s2e;
    char pad9[0x40 - 0x30];
    int *p40;
    int *p44;
} Q;
extern Q *FUN_80018568(void);
extern unsigned char DAT_8007b09a;
extern unsigned char *DAT_8007b14c[];
extern unsigned short DAT_8009c962;

void FUN_80123b64(short *o)
{
    Q *q = FUN_80018568();
    if (q) {
        q->b0 = 1;
        q->b2 = 0xc;
        q->b3 = 8;
        q->bc = 0x80;
        q->bf = DAT_8007b14c[DAT_8007b09a][3];
        q->s2e = 0;
        *q->p40 = o[1] << 16;
        q->w14 = o[3] << 16;
        *q->p44 = o[5] << 16;
        q->b1d = DAT_8009c962;
    }
}
