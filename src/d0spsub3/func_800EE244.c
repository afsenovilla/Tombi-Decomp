// FUNC 800ee244 260 D0SPSUB3
// MATCHING 800ee244 260
extern short D_800A604E;

int func_800EE244(int op, short val)
{
    short v;
    op &= 0xFF;
    v = D_800A604E;
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
