// VERSION PCPX_96084 tomba 80021928 68
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
typedef struct { short s; char pad[8]; } Rec10;
extern Rec10 D_800A1B0C[];
extern void func_8002196C(int);

void func_80021928_pcpx96084(int i)
{
    if (D_800A1B0C[i].s == 0)
        func_8002196C(i);
}
#endif
