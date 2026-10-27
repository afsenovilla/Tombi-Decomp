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
extern Info MEMCARD_FILE_INFOS[][15];
extern char *MEMCARD_PORT_PREFIXES[];
extern char *MEMCARD_SAVE_PREFIXES[];
extern char *MEMCARD_SLOT_SUFFIXES[];
extern char MEMCARD_DIR_PATH[];
extern char MEMCARD_DIR_WILDCARD[];
extern Hdr MEMCARD_SAVE_HEADER;
extern char *strcpy(char *d, char *s);
extern char *strcat(char *d, char *s);
extern char *strncat(char *d, char *s, int n);
extern char *strncpy(char *d, char *s, int n);
extern char *strstr(char *s, char *f);
extern struct DIRENTRY *psyq_8005ba74(char *name, struct DIRENTRY *d);
extern struct DIRENTRY *nextfile(struct DIRENTRY *d);
extern void memCardReadFile(char *name, void *MEMCARD_DIR_PATH, int ofs, int len);

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
        MEMCARD_FILE_INFOS[card][i].icon = -1;
        used[i] = 0;
    }
    strcpy(MEMCARD_DIR_PATH, MEMCARD_PORT_PREFIXES[card]);
    strcat(MEMCARD_DIR_PATH, MEMCARD_DIR_WILDCARD);
    if (psyq_8005ba74(MEMCARD_DIR_PATH, &d) == &d) {
        do {
            if (strstr(d.name, MEMCARD_SAVE_PREFIXES[*(short *)0x1F8003D8])) {
                strcpy(MEMCARD_DIR_PATH, MEMCARD_PORT_PREFIXES[card]);
                strncat(MEMCARD_DIR_PATH, d.name, 20);
                c->slot[n] = 1;
                strncpy(c->names[n], d.name, 20);
                memCardReadFile(MEMCARD_DIR_PATH, &MEMCARD_SAVE_HEADER, 0x200, 0x80);
                f = &MEMCARD_FILE_INFOS[card][n];
                f->a = MEMCARD_SAVE_HEADER.a;
                f->b = MEMCARD_SAVE_HEADER.b;
                f->f = MEMCARD_SAVE_HEADER.f;
                f->d = MEMCARD_SAVE_HEADER.d;
                f->c = MEMCARD_SAVE_HEADER.c;
                f->e = MEMCARD_SAVE_HEADER.e;
                for (i = 0; i < 15; i++) {
                    if (strstr(MEMCARD_DIR_PATH, MEMCARD_SLOT_SUFFIXES[i])) {
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
                    MEMCARD_FILE_INFOS[card][n + i].icon = -2;
                }
            }
            total += d.size;
            n += d.size / 8192;
        } while (nextfile(&d) == &d);
    }
    c->blocks = total / 8192;
    if (c->blocks < 15) for (i = c->blocks; i < 15; i++) {
        f = &MEMCARD_FILE_INFOS[card][i];
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
