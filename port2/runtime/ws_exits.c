/* Widescreen exits (docs/PORT.md "Widescreen exits").  In some fixed-camera side-view rooms the exit sits at the 4:3
   screen edge; with --widescreen the exit test moves out by the pad (src/x002, src/x005 updateAreaScroll under PORT2)
   and cameraModeHandler6B lets Tomba walk that much further past the camera limits (its 4:3 margin is 150 units:
   in these rooms 1 unit = 1 column, so the pad in units is the pad in columns).  Only the rooms listed here: other
   rooms may have nothing built past the 4:3 edge. */
#include "port_runtime.h"

extern int port_ws_cull;

int port_ws_exit_room(void)
{
    if (!port_ws_cull) return 0;
    int area = *(unsigned short *)PORT_M0(PORT_A_GAME), sec = *(unsigned short *)PORT_M0(PORT_A_GAME + 2);
    return (area == 2 && sec == 5)                    /* AREA02 s5: stone door room, exit on the right */
        || (area == 5 && (sec == 1 || sec == 3));     /* AREA05 s1/s3: exit on the left */
}

/* cameraModeHandler6B: the margin past the camera limits Tomba may walk */
int port_ws_player_margin(void) { return port_ws_exit_room() ? port_ws_cull : 0; }
