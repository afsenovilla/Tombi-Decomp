// FUNC 8006ba68 56 MAIN0
// MATCHING 8006ba68 56
typedef struct { char p[0xf0]; } E;
extern E PAD_PORT_BUFFERS[];
int padGetPortId(E *e)
{
    int i;
    for (i = 0; i < 2; i++) {
        if (e == &PAD_PORT_BUFFERS[i]) return (i + 1) * 0x10;
    }
    return 0xff;
}
