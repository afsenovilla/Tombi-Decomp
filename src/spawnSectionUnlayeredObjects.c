// FUNC 8004bfe4 248 MAIN0
// MATCHING 8004bfe4 248
// VERSION PCPX_96084 tomba 8008299C 248
// ASPSX.PCPX_96084 2.34
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned char **PTR_8007bee4[];
extern void (*PTR_8007bf34[])(unsigned char *);
extern unsigned char *allocObjectUnlayered(void);

void spawnSectionUnlayeredObjects(void)
{
    unsigned char *e;
    unsigned char *p;

    p = PTR_8007bee4[GAME][GAME_SELECTED_SECTION];
    while (*p != 0xff) {
        e = allocObjectUnlayered();
        if (e) {
            *e = 1;
            e[2] = p[0];
            e[3] = p[1];
            if (p[3]) PTR_8007bf34[e[2]](e);
        }
        p += 4;
    }
}
