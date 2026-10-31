/* .chd disc images (port2/shims/cd_chd.c, libchdr): raw 2352-byte sectors by disc LBA. */
#ifndef PORT_CHD_H
#define PORT_CHD_H
int port_chd_open(const char *path, int *sectors, char *err, int errlen);   /* 0, or -1 with err set */
int port_chd_read_raw(int lba, unsigned char *out2352);                      /* 0; -2 beyond the image; -1 error */
void port_chd_close(void);
#endif
