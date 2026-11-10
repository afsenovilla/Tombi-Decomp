// VERSION PCPX_96084 tomba 800222C4 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Memory card save: creates the file (blocks << 16 | O_CREAT), reopens it, writes the 0x80-byte header
// D_800E5A30 and then len bytes of buf after it; -1 when the file cannot be opened.
extern int open(char *, int);
extern int close(int);
extern int write(int, void *, int);
extern int lseek(int, int, int);
extern char D_800E5A30[];

int func_800222C4_pcpx96084(char *name, void *buf, int len, int blocks)
{
    int fd = open(name, (blocks << 16) | 0x200);
    if (fd != -1) close(fd);
    fd = open(name, 2);
    {
        int r;
        if (fd == -1) {
            close(fd);
            r = -1;
        } else {
            write(fd, D_800E5A30, 0x80);
            lseek(fd, 0x80, 0);
            write(fd, buf, len);
            close(fd);
            r = 0;
        }
        return r;
    }
}
#endif
