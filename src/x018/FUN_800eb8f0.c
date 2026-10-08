// FUNC 800eb8f0 256 X018
// MATCHING 800eb8f0 256
typedef struct N {
    unsigned char b0, b1, b2, b3;
    char pad4[6];
    unsigned char ba;
    char padb;
    unsigned char bc;
    char padd[2];
    unsigned char bf;
    int x, y, z;
    char pad1c[2];
    short h1e;
    char pad20[0x1c];
    int w3c;
} N;
extern short DAT_80114c08[];
extern N *FUN_80018448(void);

void FUN_800eb8f0(N *o, short x, short y, short z)
{
    short *p;
    int i;
    N *n;

    p = DAT_80114c08;
    i = 0;
    do {
        n = FUN_80018448();
        if (n != 0) {
            n->b0 = 1;
            n->b2 = 0x1e;
            n->bc = o->bc & 0x7f;
            n->x = (x + *p++) << 16;
            n->y = (y + *p++) << 16;
            n->z = z << 16;
            n->h1e = o->h1e;
            n->ba = 2;
            n->b3 = i;
            n->w3c = o->w3c;
            n->bf = o->bf;
        }
        i++;
    } while (i < 5);
}
