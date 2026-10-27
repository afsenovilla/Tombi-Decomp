// FUNC 800f101c 264 D0SPSUB
// MATCHING 800f101c 264
typedef struct { short x, y; } Pos;
extern Pos *PLAYER_H;

int subScreenCondPlayerH(unsigned char op, short val)
{
    short v = PLAYER_H->y;
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
