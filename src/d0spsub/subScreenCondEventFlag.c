// FUNC 800f1568 200 D0SPSUB
// MATCHING 800f1568 200
extern unsigned char SCRIPT_FLAGS[];

int subScreenCondEventFlag(int op, short i, short val)
{
    int v;
    op &= 0xFF;
    v = SCRIPT_FLAGS[i];
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
