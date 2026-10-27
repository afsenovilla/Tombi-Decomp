// FUNC 800ee348 264 D0SPSUB3
// MATCHING 800ee348 264
typedef struct { short x, y; } Pos;
extern Pos *PLAYER_D;

int subScreenCondPlayerD(unsigned char op, short val)
{
    short v = PLAYER_D->y;
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
