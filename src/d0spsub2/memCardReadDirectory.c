// FUNC 800ec8fc 972 D0SPSUB2
// MATCHING 800ec8fc 972
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
typedef struct { unsigned short a, b; char p4[4]; int e, d; char p10[3]; unsigned char c; char p14[0xC]; unsigned char f; } Hdr;
extern Info D_800F74B8[][15];
extern char *D_800F7090[];
extern char *D_800F7048[];
extern char *D_800F7050[];
extern char D_800F7160[];
extern char D_800E82A0[];
extern Hdr D_800F7180;
extern char *strcpy(char *d, char *s);
extern char *strcat(char *d, char *s);
extern char *strncat(char *d, char *s, int n);
extern char *strncpy(char *d, char *s, int n);
extern char *strstr(char *s, char *f);
extern struct DIRENTRY *psyq_8005ba74(char *name, struct DIRENTRY *d);
extern struct DIRENTRY *nextfile(struct DIRENTRY *d);
extern void memCardReadFile(char *name, void *D_800F7160, int ofs, int len);

int memCardReadDirectory(Card *c, int card)
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
        D_800F74B8[card][i].icon = -1;
        used[i] = 0;
    }
    strcpy(D_800F7160, D_800F7090[card]);
    strcat(D_800F7160, D_800E82A0);
    if (psyq_8005ba74(D_800F7160, &d) == &d) {
        do {
            if (strstr(d.name, D_800F7048[*(short *)0x1F8003D8])) {
                strcpy(D_800F7160, D_800F7090[card]);
                strncat(D_800F7160, d.name, 20);
                c->slot[n] = 1;
                strncpy(c->names[n], d.name, 20);
                memCardReadFile(D_800F7160, &D_800F7180, 0x200, 0x80);
                f = &D_800F74B8[card][n];
                f->a = D_800F7180.a;
                f->b = D_800F7180.b;
                f->f = D_800F7180.f;
                f->d = D_800F7180.d;
                f->c = D_800F7180.c;
                f->e = D_800F7180.e;
                for (i = 0; i < 15; i++) {
                    if (strstr(D_800F7160, D_800F7050[i])) {
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
                    D_800F74B8[card][n + i].icon = -2;
                }
            }
            total += d.size;
            n += d.size / 8192;
        } while (nextfile(&d) == &d);
    }
    c->blocks = total / 8192;
    if (c->blocks < 15) for (i = c->blocks; i < 15; i++) {
        f = &D_800F74B8[card][i];
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
