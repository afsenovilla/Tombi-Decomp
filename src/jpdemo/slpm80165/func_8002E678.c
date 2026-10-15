// JPDEMO slpm80165 8002E678 120
// Demo-only object init (type 49): active, CLUT row 508 + palette, animation D_80011F58.
extern void *D_80011F58;
extern int D_1F8002D8;
extern void AnimLoadDuration(unsigned char *);
void func_8002E678(unsigned char *o)
{
    o[0] = 1;
    o[2] = 49;
    *(short *)(o + 0x2e) = 1;
    o[0xa] = 0;
    o[0xb] = 0;
    o[0xf] = 0;
    *(short *)(o + 0x1e) = 20;
    o[0xd] = 1;
    *(short *)(o + 8) = ((o[0xc] + 508) << 6) | 0x17;
    {
        void *a = D_80011F58;
        int d = D_1F8002D8;
        *(void **)(o + 0x24) = a;
        *(int *)(o + 0x3c) = d;
    }
    AnimLoadDuration(o);
}
