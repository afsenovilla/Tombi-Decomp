// FUNC 8006a59c 212 MAIN0
/* score 46: library code from a newer compiler (jr ra; addiu sp in delay slot with s-regs saved): not reproducible with CC1PSX 4.3. */
typedef struct S {
    int a0;
    int a4;
    int a8;
    char pad[0x14 - 0xc];
    int a14;
    int a18;
    char pad1[0x46 - 0x1c];
    char b46, b47;
    char pad2;
    char b49;
    char pad3[0xe3 - 0x4a];
    unsigned char be3;
    char pad4[5];
    unsigned char be9;
} S;
extern int (*DAT_800981c8)(void);
extern void LAB_8006a670(void);
extern void LAB_8006a718(void);

int FUN_8006a59c(S *s, int n)
{
    int v;
    if (n != 0) {
        if (s->a4 != 0)
            return 0;
        if (DAT_800981c8() == 0) {
            s->b49 = 4;
            s->b46 = 1;
            s->a14 = (int)LAB_8006a670;
            s->a18 = (int)LAB_8006a718;
            v = (n + 3 >> 2) * 4;
            s->a0 = v;
            s->b47 = 0;
            v = v + ((s->be3 + 1) >> 1) * 4;
            s->a4 = v;
            s->a8 = v + (s->be9 * 5 + 3 & 0xffc);
            return 1;
        }
    }
    return 0;
}
