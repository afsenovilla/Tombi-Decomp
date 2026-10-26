// FUNC 800ee4e0 60 X011
// MATCHING 800ee4e0 60
typedef struct S { char pad[0x8c]; int v; char pad2[0xb0 - 0x90]; short idx; } S;
extern unsigned char SLOPE_TILT_TABLE[];
extern unsigned char SLOPE_TILT_TABLE_SLOW[];

void playerSetSlopeTilt(S *s, short f)
{
    unsigned char b;
    if (f) {
        b = SLOPE_TILT_TABLE_SLOW[s->idx];
    } else {
        b = SLOPE_TILT_TABLE[s->idx];
    }
    s->v = b;
}
