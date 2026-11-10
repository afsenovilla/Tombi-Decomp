// VERSION PCPX_96084 tomba 8006BBE8 116
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern int func_8006BA14(char *, char *);
extern short func_80022F5C_pcpx96084(int, int);
extern void func_80017C14_pcpx96084(int, int, int, int);

void func_8006BBE8_pcpx96084(char *a, char *b)
{
    int r = func_8006BA14(a, b);
    if ((short)r >= 0) {
        *(short *)(b + 46) = r & 1;
        if (a[2] == 0) func_80022F5C_pcpx96084(0, 5);
        func_80017C14_pcpx96084(1, *(short *)(a + 18), *(short *)(a + 22), *(short *)(a + 26));
    }
}
#endif
