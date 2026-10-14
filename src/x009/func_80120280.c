// FUNC 80120280 140 X009
// MATCHING 80120280 140
extern long *func_8011F96C(long *, unsigned long *);
extern long *func_8011FB4C(long *, unsigned long *);
extern long *func_8011FDC4(long *, unsigned long *);
extern long *func_8011FFE0(long *, unsigned long *);
extern long *modelDrawF3Lit(long *, long *, unsigned long *);
extern long *modelDrawF4Lit(long *, long *, unsigned long *);
extern long *modelDrawG3Lit(long *, long *, unsigned long *);
extern long *modelDrawG4Lit(long *, long *, unsigned long *);

void func_80120280(long *f, unsigned long *ot)
{
    f = func_8011F96C(f, ot);
    f = func_8011FB4C(f, ot);
    f = func_8011FDC4(f, ot);
    f = func_8011FFE0(f, ot);
    f = modelDrawF3Lit(f, f + 1, ot);
    f = modelDrawF4Lit(f, f + 1, ot);
    f = modelDrawG3Lit(f, f + 1, ot);
    modelDrawG4Lit(f, f + 1, ot);
}
