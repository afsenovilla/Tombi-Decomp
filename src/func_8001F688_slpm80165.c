// VERSION SLPM_80165 slpm80165 8001F688 24
#if defined(VERSION_SLPM_80165)
// Demo-only helper: clears two scratchpad words, returns 0.
extern int D_1F8002A0, D_1F80029C;
int func_8001F688_slpm80165(void)
{
    D_1F8002A0 = 0;
    D_1F80029C = 0;
    return 0;
}
#endif
