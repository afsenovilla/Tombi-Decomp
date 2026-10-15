// FUNC 8011e514 56 X006
// MATCHING 8011e514 56

extern char PLAYER[];
extern void collidePlayerWithCat5Objects(void *);
extern void collidePlayerWithCat2Objects(void *);

void func_8011E514(void)
{
    char *p = PLAYER;

    collidePlayerWithCat5Objects(p);
    collidePlayerWithCat2Objects(p);
}
