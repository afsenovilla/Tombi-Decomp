// FUNC 800f1434 260 D0SPSUB
// MATCHING 800f1434 260
extern short D_8009C962;

int func_800F1434(int op, short val)
{
    short v;
    op &= 0xFF;
    v = D_8009C962;
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
