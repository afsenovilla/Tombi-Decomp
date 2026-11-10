// VERSION PCPX_96084 tomba 8009234C 168
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// CD file cache lookup: 1 + the index of the entry with this key and name in the 128-entry table, -1 if none.
typedef struct {
    int key;
    int pos;
    char name[36];
} Ent8009234C;
extern Ent8009234C D_800CAD8C[];
extern int strcmp(const char *, const char *);

int func_8009234C_pcpx96084(int key, char *name)
{
    int i;
    for (i = 0; i < 128; i++) {
        if (D_800CAD8C[i].key == 0) break;
        if (D_800CAD8C[i].key == key && strcmp(name, D_800CAD8C[i].name) == 0) return i + 1;
    }
    return -1;
}
#endif
