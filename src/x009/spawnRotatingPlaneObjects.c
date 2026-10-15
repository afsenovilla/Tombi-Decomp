// FUNC 80117d48 376 X009
// MATCHING 80117d48 376
#include "gvars.h"
#include "TOBJ.H"

extern int spawnRotatingPlaneChild(TObj *, int);

void spawnRotatingPlaneObjects(TObj *o)
{
    switch (o->subtype) {
    case 0:
        spawnRotatingPlaneChild(o, 0);
        break;
    case 1:
        spawnRotatingPlaneChild(o, 1);
        spawnRotatingPlaneChild(o, 2);
        spawnRotatingPlaneChild(o, 3);
        break;
    case 2:
        spawnRotatingPlaneChild(o, 4);
        spawnRotatingPlaneChild(o, 5);
        spawnRotatingPlaneChild(o, 6);
        spawnRotatingPlaneChild(o, 7);
        break;
    case 4:
        spawnRotatingPlaneChild(o, 8);
        spawnRotatingPlaneChild(o, 9);
        break;
    case 5:
        (*(int*)&GVARS.p48) = spawnRotatingPlaneChild(o, 10);
        (*(int*)&GVARS.p4c) = spawnRotatingPlaneChild(o, 11);
        spawnRotatingPlaneChild(o, 12);
        break;
    case 8:
        spawnRotatingPlaneChild(o, 13);
        spawnRotatingPlaneChild(o, 14);
        spawnRotatingPlaneChild(o, 15);
        spawnRotatingPlaneChild(o, 16);
        spawnRotatingPlaneChild(o, 17);
        spawnRotatingPlaneChild(o, 18);
        break;
    case 9:
        spawnRotatingPlaneChild(o, 19);
        break;
    case 10:
        spawnRotatingPlaneChild(o, 20);
        break;
    case 11:
        spawnRotatingPlaneChild(o, 21);
        spawnRotatingPlaneChild(o, 22);
        break;
    }
}
