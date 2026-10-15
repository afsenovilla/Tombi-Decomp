// VERSION SLPM_80165 slpm80165 8002F1DC 80
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 8012DE8C (FUN_8012de8c.c): step byte at +2 (PAL +5).
extern void func_80109E18(char *);
extern void func_800E8BFC(char *);

void func_8002F1DC_slpm80165(char *o)
{
    switch (o[2]) {
    case 0:
        func_80109E18(o);
        break;
    case 1:
        func_800E8BFC(o);
        break;
    }
}
#endif
