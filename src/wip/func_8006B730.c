// FUNC 8006b730 188 MAIN0
typedef struct {
    char pad[0x28];
    unsigned char *t28;
    unsigned char *t2c;
    char pad30[4];
    unsigned char n34;
    char pad35;
    unsigned char n36;
    unsigned char kind;
    char pad38[0x45 - 0x38];
    unsigned char b45;
    char pad46[0x57 - 0x46];
    unsigned char flags[6];
} PadS;

int func_8006B730(PadS *p)
{
    int i = p->b45 - 3;
    int r;
    switch (p->kind) {
    case 0:
        if (i < 6 && p->flags[i] == 0) return 0;
        if (i < p->n34) r = p->t28[i];
        else r = 0;
        break;
    case 0x4d:
        if (i < p->n36) return p->t2c[i];
        return 0xff;
    default:
        if (i < p->n36) return p->t2c[i];
        return 0;
    }
    return r;
}
