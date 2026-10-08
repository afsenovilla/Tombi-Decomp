// FUNC 8011bc34 120 X006
// MATCHING 8011bc34 120

typedef struct {
    unsigned char b0;
    unsigned char pad[7];
    short w8;
} S;

extern S D_800B1410;
extern unsigned char D_8009C93A, D_8009C93F;
extern void func_8011BCAC(S *);

void func_8011BC34(void)
{
    S *p = &D_800B1410;

    if (D_8009C93A) {
        if (D_8009C93F) D_800B1410.w8 = 0x78;
        if (D_800B1410.w8 == 0 && (p->b0 & 1)) func_8011BCAC(p);
    }
}
