// VERSION PCPX_96084 tomba 800221A0 292
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Memory card load: the 0x80-byte header into D_800E5A30, its 1..3 icon frames (type byte 0x11..0x13) after it,
// then len bytes of save data from offset 0x80 into buf; -1 when the file cannot be opened.
extern int open(char *, int);
extern int close(int);
extern int read(int, void *, int);
extern int lseek(int, int, int);
extern unsigned char D_800E5A30[];
extern unsigned char D_800E5A32;

int func_800221A0_pcpx96084(char *name, void *buf, int len)
{
    int fd = open(name, 1);
    if (fd != -1) {
        unsigned char *h = D_800E5A30;
        read(fd, h, 0x80);
        lseek(fd, 0x80, 0);
        switch (D_800E5A32) {
        case 0x11:
            read(fd, h + 0x80, 0x80);
            break;
        case 0x12:
            read(fd, h + 0x80, 0x100);
            break;
        case 0x13:
            read(fd, h + 0x80, 0x180);
            break;
        }
        lseek(fd, 0x80, 0);
        read(fd, buf, len);
        close(fd);
        return 0;
    }
    close(fd);
    return -1;
}
#endif
