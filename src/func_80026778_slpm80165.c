// VERSION SLPM_80165 slpm80165 80026778 136
#if defined(VERSION_SLPM_80165)
// Demo-only helper: finds where value v ranks in the 10-entry score table (shifting the lower entries down); -1 if it does not.
typedef struct { unsigned int v; int b; } E;
extern E D_8009F740[];
int func_80026778_slpm80165(unsigned int v)
{
    int i, j;
    for (i = 0; i < 10; i++) {
        if (D_8009F740[i].v < v) {
            for (j = 9; i < j; j--)
                D_8009F740[j] = D_8009F740[j - 1];
            return i;
        }
    }
    return -1;
}
#endif
