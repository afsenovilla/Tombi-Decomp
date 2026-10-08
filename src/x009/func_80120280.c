// FUNC 80120280 140 X009
// MATCHING 80120280 140
extern long *func_8011F96C(long *, unsigned long *);
extern long *func_8011FB4C(long *, unsigned long *);
extern long *func_8011FDC4(long *, unsigned long *);
extern long *func_8011FFE0(long *, unsigned long *);
extern long *func_80022ABC(long *, long *, unsigned long *);
extern long *FUN_80022c80(long *, long *, unsigned long *);
extern long *FUN_80022ea8(long *, long *, unsigned long *);
extern long *func_80023084(long *, long *, unsigned long *);

void func_80120280(long *f, unsigned long *ot)
{
    f = func_8011F96C(f, ot);
    f = func_8011FB4C(f, ot);
    f = func_8011FDC4(f, ot);
    f = func_8011FFE0(f, ot);
    f = func_80022ABC(f, f + 1, ot);
    f = FUN_80022c80(f, f + 1, ot);
    f = FUN_80022ea8(f, f + 1, ot);
    func_80023084(f, f + 1, ot);
}
