// VERSION SLPM_80165 slpm80165 8003FBCC 104
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of PrePre 8 func_8005A624 (src/func_8005A624_pcpx96084.c): the mode selector is a byte here.
extern unsigned char D_800A57B9;
extern char D_800A57B8[];
extern unsigned char D_800A57BA;
extern void func_8003FC34(void *);
extern void func_800E8CC8(void *);

void func_8003FBCC(void)
{
    char *p = D_800A57B8;
    if (D_800A57B9 != 0) {
        switch (D_800A57BA) {
        case 0:
            func_8003FC34(p);
            break;
        case 1:
            func_800E8CC8(p);
            break;
        }
    }
}
#endif
