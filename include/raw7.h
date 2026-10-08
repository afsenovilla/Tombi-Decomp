/* accesos por offset (shard7) */
#ifndef RAW7_H
#define RAW7_H
#define U8(o,n)  (*(unsigned char *)((char *)(o) + (n)))
#define S8(o,n)  (*(signed char *)((char *)(o) + (n)))
#define U16(o,n) (*(unsigned short *)((char *)(o) + (n)))
#define S16(o,n) (*(short *)((char *)(o) + (n)))
#define S32(o,n) (*(int *)((char *)(o) + (n)))
#define PTR(o,n) (*(void **)((char *)(o) + (n)))
#endif
