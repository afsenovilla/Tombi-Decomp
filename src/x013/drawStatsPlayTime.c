// FUNC 80117e14 532 X013
// MATCHING 80117e14 532
extern void drawStatsDigit(int, short, short);
void drawStatsPlayTime(int n, int m, short x, short y)
{
    int h, t, o, d, q;
    q = n / 100;
    t = n / 10 % 10;
    h = q % 10;
    o = n % 10;
    if (h) {
        drawStatsDigit(h, x, y);
        drawStatsDigit(t, x + 8, y);
        drawStatsDigit(o, x + 16, y);
        x += 24;
    } else if (t) {
        drawStatsDigit(t, x, y);
        drawStatsDigit(o, x + 8, y);
        x += 16;
    } else {
        drawStatsDigit(o, x, y);
        x += 8;
    }
    drawStatsDigit(10, x, y);
    d = m / 10;
    x += 8;
    t = m % 10;
    h = d % 10;
    drawStatsDigit(h, x, y);
    drawStatsDigit(t, x + 8, y);
}
