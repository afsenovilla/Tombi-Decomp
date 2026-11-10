// VERSION PCPX_96084 tomba 8005A624 104
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern unsigned char D_800F4FD9;
extern char D_800F4FD8[];
extern unsigned short D_800F4FE2;
extern void func_8005A68C(void *);
extern void func_8005B3F8(void *);

void func_8005A624_pcpx96084(void)
{
    char *p = D_800F4FD8;
    if (D_800F4FD9 != 0) {
        switch (D_800F4FE2) {
        case 0:
            func_8005A68C(p);
            break;
        case 1:
            func_8005B3F8(p);
            break;
        }
    }
}
#endif
