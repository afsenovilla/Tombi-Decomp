// FUNC 800ecd94 212 D0SPSUB2
// MATCHING 800ecd94 212
extern int open(char *name, int mode);
extern int write(int fd, void *buf, int n);
extern int close(int fd);

int memCardWriteFile(char *name, void *buf, int len, int blocks)
{
    int retry = 3;
    int fd;
    register int err asm("s1");
again:
    fd = open(name, (blocks << 16) | 0x200);
    err = fd;
    if (fd == -1) {
        fd = open(name, 2);
        if (fd == err) goto next;
        if (write(fd, buf, len) != err) {
            close(fd);
            return 0;
        }
    }
    close(fd);
next:
    retry--;
    if (retry > 0) goto again;
    return -1;
}
