// FUNC 8011ae94 132 X009
// MATCHING 8011ae94 132
#include "TOBJ.H"
extern void spawnRedFlowerOnPlatform(TObj *, int);

void spawnFlowerPlatformRedFlower(TObj *o)
{
    switch (o->subtype) {
    case 1:
        spawnRedFlowerOnPlatform(o, 0);
        break;
    case 2:
        spawnRedFlowerOnPlatform(o, 1);
        break;
    case 5:
        spawnRedFlowerOnPlatform(o, 2);
        break;
    case 6:
        spawnRedFlowerOnPlatform(o, 3);
        break;
    case 7:
        spawnRedFlowerOnPlatform(o, 4);
        break;
    case 8:
        spawnRedFlowerOnPlatform(o, 5);
        break;
    case 9:
        spawnRedFlowerOnPlatform(o, 6);
        break;
    }
}
