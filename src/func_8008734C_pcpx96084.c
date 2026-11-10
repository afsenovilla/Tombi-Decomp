// VERSION PCPX_96084 tomba 8008734C 60
// FLAGS -O2 -G0
#if defined(VERSION_PCPX_96084)
extern char D_800C8074[];
extern char *strcpy(char *, char *);
extern void EnterCriticalSection(void);
extern void func_8008746C_pcpx96084(void);

void func_8008734C_pcpx96084(void)
{
    strcpy((char *)0xA000DF00, D_800C8074);
    EnterCriticalSection();
    func_8008746C_pcpx96084();
}
#endif
