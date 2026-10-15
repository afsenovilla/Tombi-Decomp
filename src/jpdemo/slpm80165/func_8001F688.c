// JPDEMO slpm80165 8001F688 24
// Demo-only helper: clears two scratchpad words, returns 0.
extern int D_1F8002A0, D_1F80029C;
int func_8001F688(void)
{
    D_1F8002A0 = 0;
    D_1F80029C = 0;
    return 0;
}
