// FUNC 800eccc8 204 D0SPSUB2
// MATCHING 800eccc8 204
extern int open(char *name, int mode);
extern int lseek(int fd, int ofs, int whence);
extern int read(int fd, void *buf, int n);
extern int close(int fd);

int memCardReadFile(char *name, void *buf, int ofs, int len)
{
    int retry;
    int fd;
    for (retry = 3; retry > 0; retry--) {
        fd = open(name, 1);
        if (fd != -1) {
            if (lseek(fd, ofs, 0) != -1 && read(fd, buf, len) != -1) {
                close(fd);
                return 0;
            }
            close(fd);
        }
    }
    return -1;
}
