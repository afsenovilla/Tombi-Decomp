// JPDEMO slpm80165 80023EC8 124
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
