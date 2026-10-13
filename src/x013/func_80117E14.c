// FUNC 80117e14 532 X013
// MATCHING 80117e14 532
extern void func_80118028(int, short, short);
void func_80117E14(int n, int m, short x, short y)
{
    int h, t, o, d, q;
    q = n / 100;
    t = n / 10 % 10;
    h = q % 10;
    o = n % 10;
    if (h) {
        func_80118028(h, x, y);
        func_80118028(t, x + 8, y);
        func_80118028(o, x + 16, y);
        x += 24;
    } else if (t) {
        func_80118028(t, x, y);
        func_80118028(o, x + 8, y);
        x += 16;
    } else {
        func_80118028(o, x, y);
        x += 8;
    }
    func_80118028(10, x, y);
    d = m / 10;
    x += 8;
    t = m % 10;
    h = d % 10;
    func_80118028(h, x, y);
    func_80118028(t, x + 8, y);
}
