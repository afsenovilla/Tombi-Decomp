// FUNC 800eee40 80 X013
// MATCHING 800eee40 80
typedef struct { char pad[4]; unsigned char b4, b5, b6; } E;
extern E *D_800A611C;
extern E *D_8009D2E8;
typedef struct { char pad[0xac]; unsigned char bac; } O;

void func_800EEE40(O *o)
{
    if (o->bac >= 2) {
        D_8009D2E8 = D_800A611C;
        D_8009D2E8->b4 = 2;
        D_8009D2E8->b5 = 2;
        D_8009D2E8->b6 = 0;
    }
    o->bac = 0;
}
