// FUNC 800ee13c 264 D0SPSUB3
// MATCHING 800ee13c 264
typedef struct { short x, y; } Pos;
extern Pos *D_800A6078;

int func_800EE13C(unsigned char op, short val)
{
    short v = D_800A6078->y;
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
