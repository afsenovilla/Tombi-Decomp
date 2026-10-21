// FUNC 800ee688 200 D0SPSUB3
// MATCHING 800ee688 200
extern unsigned char D_8009CDA4[];

int subScreenCondEventFlag(int op, short i, short val)
{
    int v;
    op &= 0xFF;
    v = D_8009CDA4[i];
    switch (op) {
    case 0: if (v == val) return 1; break;
    case 1: if (v != val) return 1; break;
    case 2: if (v < val) return 1; break;
    case 3: if (v <= val) return 1; break;
    case 4: if (v > val) return 1; break;
    case 5: if (v >= val) return 1; break;
    }
    return 0;
}
