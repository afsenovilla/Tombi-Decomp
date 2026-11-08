// VERSION SLPS_01144 dspsub2 800EA490 900
#if defined(VERSION_SLPS_01144)
// JP memCardReadDirectory (PAL d0spsub2 800ec8fc): one file-info table, two save prefixes
struct DIRENTRY {
    char name[20];
    long attr;
    long size;
    struct DIRENTRY *next;
    long head;
    char system[4];
};
typedef struct { char names[15][20]; char p12c[0x18]; short blocks; char p146[2]; unsigned char slot[16]; unsigned short count; } Card;
typedef struct { short icon, a, b, c; int d, e; short p10, f; } Info;
extern Info D_800F3388[15];      /* file infos */
extern char *D_800EE2A8[];       /* port prefixes */
extern char *D_800EE268[];       /* slot suffixes */
extern char D_800F2FA0[];        /* dir path */
extern char D_800E9B8C[];        /* wildcard */
extern char D_800E9B90[], D_800E9BA0[]; /* save prefixes */
extern unsigned short D_800F2FC0[]; /* save header (field a) */
/* the header fields, each named by retail; arrays so that gcc keeps them ordered with the f-> stores */
extern unsigned short D_800F2FC2[];
extern int D_800F2FC8[], D_800F2FCC[];
extern unsigned char D_800F2FD3[], D_800F2FE0[];
extern char *func_80058874(char *d, char *s); /* strcpy */
extern char *func_800587CC(char *d, char *s); /* strcat */
extern char *func_800588E8(char *d, char *s, int n); /* strncat */
extern char *func_8005895C(char *d, char *s, int n); /* strncpy */
extern char *func_800589D4(char *s, char *f); /* strstr */
extern struct DIRENTRY *func_8005854C(char *name, struct DIRENTRY *d); /* firstfile */
extern struct DIRENTRY *func_8005855C(struct DIRENTRY *d); /* nextfile */
extern void memCardReadFile(char *name, void *buf, int ofs, int len);

int func_800EA490(Card *c, int card)
{
    struct DIRENTRY d;
    short used[15];
    int i, j, n, total;
    Info *f;
    n = 0;
    total = 0;
    c->count = 0;
    for (i = 0; i < 15; i++) {
        c->slot[i] = 0xFF;
        D_800F3388[i].icon = -1;
        used[i] = 0;
    }
    func_80058874(D_800F2FA0, D_800EE2A8[card]);
    func_800587CC(D_800F2FA0, D_800E9B8C);
    if (func_8005854C(D_800F2FA0, &d) == &d) {
        do {
            if (func_800589D4(d.name, D_800E9B90) || func_800589D4(d.name, D_800E9BA0)) {
                func_80058874(D_800F2FA0, D_800EE2A8[card]);
                func_800588E8(D_800F2FA0, d.name, 20);
                c->slot[n] = 1;
                func_8005895C(c->names[n], d.name, 20);
                memCardReadFile(D_800F2FA0, D_800F2FC0, 0x200, 0x80);
                f = &D_800F3388[n];
                f->a = D_800F2FC0[0];
                f->b = D_800F2FC2[0];
                f->f = D_800F2FE0[0];
                f->d = D_800F2FCC[0];
                f->c = D_800F2FD3[0];
                f->e = D_800F2FC8[0];
                for (i = 0; i < 15; i++) {
                    if (func_800589D4(D_800F2FA0, D_800EE268[i])) {
                        c->slot[n] = i;
                        f->icon = i;
                        used[i] = 1;
                        break;
                    }
                }
                c->count++;
            } else {
                for (i = 0; i < d.size / 8192; i++) {
                    c->slot[n + i] = 0xFF;
                    D_800F3388[n + i].icon = -2;
                }
            }
            total += d.size;
            n += d.size / 8192;
        } while (func_8005855C(&d) == &d);
    }
    c->blocks = total / 8192;
    if (c->blocks < 15) for (i = c->blocks; i < 15; i++) {
        f = &D_800F3388[i];
        f->a = -1;
        for (j = 0; j < 15; j++) {
            if (used[j] == 0) {
                c->slot[i] = j;
                f->icon = j;
                used[j] = 1;
                break;
            }
        }
    }
    return n;
}
#endif
