// JPDEMO slpm80165 8003F7E0 56
// Demo-only helper: links a primitive into ordering table entry x + z (clamped at 0) with a tag.
void func_8003F7E0(int *p, int *ot, int x, short z, int tag)
{
    int t;
    x = (x << 2) + (z << 2);
    if (x < 0) x = 0;
    x += (int)ot;
    t = *(int *)x;
    *(int *)x = (int)p;
    *p = t | tag;
}
