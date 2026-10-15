// FUNC 80039338 584 MAIN0
// MATCHING 80039338 584
// VERSION SLPM_80165 slpm80165 80037394 524
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80039338 (loadAreaSoundBank.c): only the demo's areas 0-2 (no D_8009F838 offset); area 0 uses bank 0.
extern unsigned short DAT_8009c960, DAT_8009c962, DAT_8009f838;
extern int DAT_8009d69c;
extern unsigned char DAT_8009cdb1, DAT_8009cdb8, DAT_8009cdb9, DAT_8009cedd;
extern void loadWssFile(int, int);

void loadAreaSoundBank(void)
{
    DAT_8009d69c = 0;
    switch (DAT_8009c960) {
    case 0:
        switch (DAT_8009c962) {
        case 0:
        case 1:
        case 2:
            loadWssFile(15, 0);
            DAT_8009d69c = 1;
            break;
        }
        break;
    case 1:
        switch (DAT_8009c962) {
        case 0:
        case 1:
            if (DAT_8009cdb1 != 0xff) {
                loadWssFile(15, 0);
                DAT_8009d69c = 1;
            }
            if (DAT_8009cdb8 == 0xff) {
                if (DAT_8009cdb9 == 0) {
                    loadWssFile(15, 0);
                    DAT_8009d69c = 1;
                } else if (DAT_8009cedd == 2) {
                    loadWssFile(15, 0);
                    DAT_8009d69c = 1;
                }
            }
            break;
        case 2:
            loadWssFile(15, 5);
            DAT_8009d69c = 1;
            break;
        case 3:
            loadWssFile(15, 4);
            DAT_8009d69c = 1;
            break;
        case 4:
            loadWssFile(15, 3);
            DAT_8009d69c = 1;
            break;
        }
        break;
    case 2:
        switch (DAT_8009c962) {
        case 0:
            if (DAT_8009cdb8 == 1)
                loadWssFile(15, 8);
            else
                loadWssFile(15, 1);
            DAT_8009d69c = 2;
            break;
        case 1:
            loadWssFile(15, 2);
            DAT_8009d69c = 1;
            break;
        case 2:
            loadWssFile(15, 7);
            DAT_8009d69c = 1;
            break;
        }
        break;
    }
}
#else
extern unsigned short DAT_8009c960, DAT_8009c962, DAT_8009f838;
extern int DAT_8009d69c;
extern unsigned char DAT_8009cdb1, DAT_8009cdb8, DAT_8009cdb9, DAT_8009cedd;
extern void loadWssFile(int, int);

void loadAreaSoundBank(void)
{
    DAT_8009d69c = 0;
    switch (DAT_8009c960 + DAT_8009f838) {
    case 0:
        switch (DAT_8009c962) {
        case 0:
        case 1:
        case 2:
            loadWssFile(15, 6);
            DAT_8009d69c = 1;
            break;
        }
        break;
    case 1:
    case 7:
        switch (DAT_8009c962) {
        case 0:
        case 1:
            if (DAT_8009cdb1 != 0xff) {
                loadWssFile(15, 0);
                DAT_8009d69c = 1;
            }
            if (DAT_8009cdb8 == 0xff) {
                if (DAT_8009cdb9 == 0) {
                    loadWssFile(15, 0);
                    DAT_8009d69c = 1;
                } else if (DAT_8009cedd == 2) {
                    loadWssFile(15, 0);
                    DAT_8009d69c = 1;
                }
            }
            break;
        case 2:
            loadWssFile(15, 5);
            DAT_8009d69c = 1;
            break;
        case 3:
            loadWssFile(15, 4);
            DAT_8009d69c = 1;
            break;
        case 4:
            loadWssFile(15, 3);
            DAT_8009d69c = 1;
            break;
        }
        break;
    case 2:
        switch (DAT_8009c962) {
        case 0:
            if (DAT_8009cdb8 == 1)
                loadWssFile(15, 8);
            else
                loadWssFile(15, 1);
            DAT_8009d69c = 2;
            break;
        case 1:
            loadWssFile(15, 2);
            DAT_8009d69c = 1;
            break;
        case 2:
            loadWssFile(15, 7);
            DAT_8009d69c = 1;
            break;
        }
        break;
    case 19:
        switch (DAT_8009c962) {
        case 0:
            if (DAT_8009cdb8 == 1)
                loadWssFile(15, 8);
            else
                loadWssFile(15, 1);
            DAT_8009d69c = 2;
            break;
        case 1:
            loadWssFile(15, 2);
            DAT_8009d69c = 1;
            break;
        }
        break;
    }
}
#endif
