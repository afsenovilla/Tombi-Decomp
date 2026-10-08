// FUNC 80117c10 516 X013
// MATCHING 80117c10 516
#include "TOBJ.H"
extern void func_80118028(int, short, short);

void func_80117C10(int n, short x, short y)
{
    int d[8];
    int i;
    int lead;

    d[0] = (n / 10000000) % 10;
    d[1] = n / 1000000 - n / 10000000 * 10;
    d[2] = n / 100000 - n / 1000000 * 10;
    d[3] = n / 10000 - n / 100000 * 10;
    d[4] = n / 1000 - n / 10000 * 10;
    d[5] = n / 100 - n / 1000 * 10;
    d[6] = n / 10 - n / 100 * 10;
    d[7] = n - n / 10 * 10;
    lead = 0;
    for (i = 0; i < 8; i++) {
        if (lead || d[i] || i == 7) {
            func_80118028(d[i], x, y);
            x += 8;
            lead = 1;
        }
    }
}
