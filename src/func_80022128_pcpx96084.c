// VERSION PCPX_96084 tomba 80022128 120
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Counts the files that match the wildcard name D_800A21E8[i] (firstfile / nextfile into D_800E21F8).
typedef struct { char name[20]; int attr; int size; void *next; int head; char *system[1]; } DIRENT40;
extern char *D_800A21E8[];
extern DIRENT40 D_800E21F8[];
extern DIRENT40 *firstfile(char *, DIRENT40 *);
extern DIRENT40 *nextfile(DIRENT40 *);
int func_80022128_pcpx96084(int i)
{
    int n = 0;
    DIRENT40 *b = D_800E21F8;
    DIRENT40 *p = firstfile(D_800A21E8[i], b);
    if (p == b) {
        DIRENT40 *d = p;
        do {
            d++;
            n++;
        } while (nextfile(d) == d);
    }
    return n;
}

#endif
