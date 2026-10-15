// FUNC 80120280 140 X009
// MATCHING 80120280 140
// VERSION SLPM_80165 slpm80165 80023EC8 124
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x009 80120280 (func_80120280.c): the last four draw calls take (f, ot).
extern long *func_8011F96C(long *, unsigned long *);
extern long *func_8011FB4C(long *, unsigned long *);
extern long *func_8011FDC4(long *, unsigned long *);
extern long *func_8011FFE0(long *, unsigned long *);
extern long *func_80022ABC(long *, unsigned long *);
extern long *FUN_80022c80(long *, unsigned long *);
extern long *FUN_80022ea8(long *, unsigned long *);
extern long *func_80023084(long *, unsigned long *);

void func_80120280(long *f, unsigned long *ot)
{
    f = func_8011F96C(f, ot);
    f = func_8011FB4C(f, ot);
    f = func_8011FDC4(f, ot);
    f = func_8011FFE0(f, ot);
    f = func_80022ABC(f, ot);
    f = FUN_80022c80(f, ot);
    f = FUN_80022ea8(f, ot);
    func_80023084(f, ot);
}
#else
extern long *drawArea9ModelFT3(long *, unsigned long *);
extern long *drawArea9ModelFT4(long *, unsigned long *);
extern long *drawArea9ModelGT3(long *, unsigned long *);
extern long *drawArea9ModelGT4(long *, unsigned long *);
extern long *modelDrawF3Lit(long *, long *, unsigned long *);
extern long *modelDrawF4Lit(long *, long *, unsigned long *);
extern long *modelDrawG3Lit(long *, long *, unsigned long *);
extern long *modelDrawG4Lit(long *, long *, unsigned long *);

void drawArea9SectionModel(long *f, unsigned long *ot)
{
    f = drawArea9ModelFT3(f, ot);
    f = drawArea9ModelFT4(f, ot);
    f = drawArea9ModelGT3(f, ot);
    f = drawArea9ModelGT4(f, ot);
    f = modelDrawF3Lit(f, f + 1, ot);
    f = modelDrawF4Lit(f, f + 1, ot);
    f = modelDrawG3Lit(f, f + 1, ot);
    modelDrawG4Lit(f, f + 1, ot);
}
#endif
