// FUNC 8011e364 80 X006
// MATCHING 8011e364 80
extern short D_8009CFE2;
extern short D_8009CFD4;
extern short D_8009CFD2;
extern short D_8009CFD0;

int func_8011E364(void)
{
    short x = D_8009CFE2;
    int r;

    if (x > D_8009CFD4) {
        r = -1;
    } else if (D_8009CFD2 < x) {
        r = 2;
    } else {
        r = D_8009CFD0 < x;
    }
    return r;
}
