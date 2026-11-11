// VERSION SLPM_80165 slpm80165 8001C6E0 56
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: calls func_8001C718 for an entry whose flag byte is clear.
extern unsigned char D_8006A300[];
extern void func_8001C718(int i);

void func_8001C6E0(int i)
{
    if (D_8006A300[i] == 0) func_8001C718(i);
}
#endif
