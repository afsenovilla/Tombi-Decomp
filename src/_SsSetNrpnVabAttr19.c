// FUNC 8006e06c 36 MAIN0
// MATCHING 8006e06c 36
typedef struct { char pad[0x24]; unsigned char b; } S;
extern void SsUtSetReverbDelay();

void _SsSetNrpnVabAttr19(int a, int b, int c, S s)
{
    SsUtSetReverbDelay(s.b);
}
