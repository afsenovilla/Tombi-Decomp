// FUNC 8011e514 56 X006
// MATCHING 8011e514 56

extern char PLAYER[];
extern void func_80047A3C(void *);
extern void collidePlayerWithCat2Objects(void *);

void func_8011E514(void)
{
    char *p = PLAYER;

    func_80047A3C(p);
    collidePlayerWithCat2Objects(p);
}
