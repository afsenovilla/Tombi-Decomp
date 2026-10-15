// VERSION SLPM_80165 x00 8011BE68 80
#if defined(VERSION_SLPM_80165)
// Demo-only touch handler (no PAL C counterpart): starts the touched object (state 1) once the overlap test passes.
extern short func_8004ACAC(unsigned char *, unsigned char *);

void func_8011BE68_slpm80165(unsigned char *a, unsigned char *b)
{
    if (b[4] == 0 && func_8004ACAC(a, b) >= 0) {
        b[4] = 1;
        b[5] = 0;
    }
}
#endif
