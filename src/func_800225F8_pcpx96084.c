// VERSION PCPX_96084 tomba 800225F8 120
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
extern int open(char *, int);
extern int read(int, void *, int);
extern int close(int);

int func_800225F8_pcpx96084(char *name, void *buf)
{
    int fd = open(name, 3);
    if (fd == -1) return -1;
    if (read(fd, buf, 0x200) != 0x200) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
#endif
