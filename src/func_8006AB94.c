// FUNC 8006ab94 200 MAIN0
// MATCHING 8006ab94 200
int func_8006AB94(unsigned char *o)
{
    int i;
    int j, cnt, n;
    unsigned char *p;
    i = 0;
    if (o[0xe9] != 0) {
        do {
            p = *(unsigned char **)(o + 0x20);
            cnt = 0;
            for (j = 0; j < 6; j++) {
                if (*p++ == i) cnt++;
            }
            n = (*(unsigned char **)(o + 4))[i * 5 + 2];
            p = *(unsigned char **)(o + 0x20);
            if (n == 0) n = 1;
            for (j = 0; j < 6; j++) {
                if (*p++ == i) {
                    if (cnt < n) {
                        o[0x5d + j] = 0xff;
                        cnt--;
                    } else {
                        o[0x5d + j] = i;
                    }
                }
            }
        } while (++i < o[0xe9]);
    }
    o[0x46] = 0xfe;
    return 0;
}
