// FUNC 80020eac 104 MAIN0
// MATCHING 80020eac 104
typedef struct { unsigned short a, b, c, d; } E;
extern short IDX;
extern E T[];
extern unsigned short S3C4, S3C6, S3C8, S3CA;

void FUN_80020eac(void)
{
    int i = IDX;
    S3C4 = T[i].a;
    S3C6 = T[i].b;
    S3C8 = T[i].c;
    S3CA = T[i].d;
}
