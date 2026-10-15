// JPDEMO slpm80165 8003F818 36
// Demo-only helper: links a primitive into ordering table entry z with a tag.
void func_8003F818(int *p, int *ot, int unused, short z, int tag)
{
    int a, t;
    a = (z << 2) + (int)ot;
    t = *(int *)a;
    *(int *)a = (int)p;
    t |= tag;
    *p = t;
}
