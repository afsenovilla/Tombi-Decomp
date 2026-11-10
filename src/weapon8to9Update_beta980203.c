// VERSION BETA_980203 beta980203 800395B4 300
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/weapon8to9Update.c without the state 6/7 early outs.
#include "TOBJ.H"
extern void weapon8to9Swing(TObj *);
extern void weapon8to9FollowTarget(TObj *);

void weapon8to9Update(TObj *o)
{
    switch (o->step) {
    case 0:
        weapon8to9Swing(o);
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3: o->d8c = 0; break;
        case 4: o->d8c = 0x20; break;
        case 5: o->d8c = 0xe0; break;
        case 6: o->d8c = 0x40; break;
        case 7: o->d8c = 0xc0; break;
        }
        break;
    case 1:
        weapon8to9Swing(o);
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3: o->d8c = 0; break;
        case 4: o->d8c = 0x20; break;
        case 5: o->d8c = 0xe0; break;
        case 6: o->d8c = 0x40; break;
        case 7: o->d8c = 0xc0; break;
        }
        break;
    case 2:
        weapon8to9Swing(o);
        switch (o->animFrame) {
        case 0: case 1: case 2: case 3: o->d8c = 0; break;
        case 4: o->d8c = 0x20; break;
        case 5: o->d8c = 0xe0; break;
        case 6: o->d8c = 0x40; break;
        case 7: o->d8c = 0xc0; break;
        }
        break;
    case 3:
    case 4:
        weapon8to9FollowTarget(o);
        break;
    }
}
#endif
