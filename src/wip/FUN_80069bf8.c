// FUNC 80069bf8 240 MAIN0
// CC gcc-2.8.1
// FLAGS -O2 -G8
/* score 33 (n66; was 51 with 2.7.2). gcc-2.8.1 -G8 gives the right epilogue and all the lui/lw macro forms, but the
   two first accesses differ: game keeps the counter address in a1 (`lui/addiu a1; lw v1,0(a1)` ... `sw v1,0(a1)` in
   the jalr slot) and indexes the table as `lui at; addu at,at,v0; lw v0,lo(at)` (symbol+reg macro), while 2.8.x emit
   `lw $5,DAT` macros for the counter and an explicit la for the table (2.7.2 does the opposite for the table but has
   the wrong epilogue and a `sw; j` tail). Tried: int/volatile pointers, [0] arrays, struct/array sizes vs -G,
   -G0/4/8/16, -O1/-O3, -mgpopt/-mno-gpopt, -fno-* flags, gcc-2.8.0, gcc-2.7.2-cdk. */
extern int DAT_800981f0;
extern int (*PTR_LAB_80098230[])(int *);
extern void (*DAT_800981ac)(int);
extern void FUN_8006bf84(int);
extern int FUN_8006a170(void);

void FUN_80069bf8(int *param_1)
{
    int r;
    int *p = &DAT_800981f0;
    int n = *p;
    *p = n + 1;
    r = PTR_LAB_80098230[n](param_1);
    if (r >= 0) {
        if (DAT_800981f0 != 0 && (DAT_800981f0 != 3 || **(unsigned char **)(param_1 + 0xf) != 0x80)) {
            FUN_8006bf84(0x3c);
            r = FUN_8006a170();
            if (r == 0)
                (*DAT_800981ac)(-3);
        }
        if (4 < DAT_800981f0)
            DAT_800981f0 = DAT_800981f0 - 1;
    } else {
        (*DAT_800981ac)(r);
    }
}
