// FUNC 8006e048 36 MAIN0
// MATCHING 8006e048 36
typedef struct { char pad[0x24]; } S24;
void SsUtSetReverbDelay(short);
void func_8006E048(int a, int b, int c, S24 s, unsigned char x)
{
    SsUtSetReverbDelay(x);
}
