// FUNC 8006baa0 32 MAIN0
// MATCHING 8006baa0 32
extern char PAD_PORT_BUFFERS[];

char *padGetPortBuffer(int a)
{
    char *p = PAD_PORT_BUFFERS;
    if (a & 0xf0) {
        p += 0xf0;
    }
    return p;
}
