#ifndef TOMBA_CAMERA_H
#define TOMBA_CAMERA_H
/* Camera state shared by the scroll / camera-mode code. Plain C types so self-contained sources can include it.
 * Offsets were checked against the sources that reached these fields by raw offset (initSectionCamera,
 * waypointRideStep, motocrossBikeRide, the D_800A45xx field symbols). */

#ifndef TOMBA_VEC3L_DEFINED
#define TOMBA_VEC3L_DEFINED
typedef struct { int x, y, z; } Vec3L;
#endif

/* CAMERA_SCROLL (0x800A4550, 0x84 bytes; cleared by clearLayer1Buffer, stepped by updateCameraController). */
typedef struct CameraScroll {
    /* 0x00 */ unsigned char active;
    /* 0x01 */ unsigned char unk01[7];
    /* 0x08 */ Vec3L pos;
    /* 0x14 */ int velX;
    /* 0x18 */ int velY;
    /* 0x1C */ unsigned char unk1C[4];
    /* 0x20 */ int rotX;
    /* 0x24 */ int rotY;
    /* 0x28 */ unsigned char unk28[4];
    /* 0x2C */ short minX;
    /* 0x2E */ short maxX;
    /* 0x30 */ short minY;
    /* 0x32 */ short maxY;
    /* 0x34 */ int *targetX;
    /* 0x38 */ int *targetZ;
    /* 0x3C */ unsigned char unk3C;
    /* 0x3D */ unsigned char settled;
    /* 0x3E */ unsigned char rising;
    /* 0x3F */ unsigned char unk3F[5];
    /* 0x44 */ unsigned short edgeFlags;
    /* 0x46 */ unsigned char unk46[6];
    /* 0x4C */ unsigned short unk4C;
    /* 0x4E */ unsigned char unk4E[0x1E];
    /* 0x6C */ unsigned char tiltTimer;
    /* 0x6D */ unsigned char unk6D;
    /* 0x6E */ signed char tiltLevel;
    /* 0x6F */ signed char tiltStep;
    /* 0x70 */ unsigned char unk70[8];
    /* 0x78 */ int rotXOffset;
    /* 0x7C */ int rotYOffset;
    /* 0x80 */ unsigned char unk80[4];
} CameraScroll;

extern CameraScroll CAMERA_SCROLL;


#endif
