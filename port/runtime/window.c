/* --window: an SDL2 window in a separate 64-bit process (port/tools/view/tombi_view.c, built as tombi-view when
 * SDL2 is found).  The game binary is i386 and freestanding (no libc, no dynamic loader), so it cannot link
 * SDL itself; instead it forks the viewer and talks to it through three pipes:
 *   viewer stdin : frames, each "TBFR" + u32 width + u32 height + RGB24 pixels (the DISPENV display area)
 *   viewer fd 3  : audio, raw s16le stereo 44.1 kHz (the same stream as --pipe-audio)
 *   viewer fd 4  : pad, one u16 per host frame (PSX bit order, active high) for port 1
 * The viewer closing the window ends the frame pipe; the game then exits.  --window implies --realtime. */
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;

const char *port_window_viewer;     /* --viewer PATH (default: tombi-view next to the game binary) */
int port_window;                    /* --window */
static int fd_video = -1, fd_audio = -1, fd_pad = -1;
static int viewer_pid;
u16 port_window_pad;                /* last pad state received from the viewer (port 1) */

extern int port_spawn(const char *path, char *const argv[], int fds[3]);
extern int port_write_all(int fd, const void *b, int n);
extern int port_realtime;
extern void port_pipe_video_fd(int fd);   /* gpu.c */
extern void port_pipe_audio_fd(int fd);   /* sound.c */

static char viewer_path[512];

void port_window_start(const char *argv0) {
    const char *path = port_window_viewer;
    if (!path) {
        int n = 0, slash = -1;
        for (n = 0; argv0[n]; n++) if (argv0[n] == '/') slash = n;
        int i = 0;
        if (slash >= 0) { for (i = 0; i <= slash; i++) viewer_path[i] = argv0[i]; }
        const char *b = "tombi-view";
        for (int k = 0; b[k]; k++) viewer_path[i++] = b[k];
        viewer_path[i] = 0;
        path = viewer_path;
    }
    char *argv[] = { (char *)path, 0 };
    int fds[3];
    viewer_pid = port_spawn(path, argv, fds);
    if (viewer_pid < 0) port_fatal("--window: cannot start the viewer %s", path);
    fd_video = fds[0]; fd_audio = fds[1]; fd_pad = fds[2];
    port_pipe_video_fd(fd_video);
    port_pipe_audio_fd(fd_audio);
    port_realtime = 1;
    port_printf("[port] viewer %s started (pid %d)\n", path, viewer_pid);
}

/* save states keep this run's viewer */
void port_window_fds_get(int *v, int *a, int *p) { *v = fd_video; *a = fd_audio; *p = fd_pad; }
void port_window_fds_set(int v, int a, int p) { fd_video = v; fd_audio = a; fd_pad = p; }

/* called by gpu.c with the displayed frame */
void port_window_frame(const u8 *rgb, int w, int h) {
    if (fd_video < 0) return;
    u32 hdr[3] = { 0x52464254u /* "TBFR" */, (u32)w, (u32)h };
    if (port_write_all(fd_video, hdr, 12) != 12 || port_write_all(fd_video, rgb, w * h * 3) != w * h * 3) {
        port_printf("[port] viewer closed: exiting\n");
        port_exit(0);
    }
}

/* called by pad.c each vblank: drains what the viewer sent, keeps the last state */
int port_window_poll_pad(u16 *out) {
    if (fd_pad < 0) return 0;
    u16 v[64];
    int n = port_read(fd_pad, v, sizeof v);
    while (n > 0) {
        if (n >= 2) port_window_pad = v[n / 2 - 1];
        n = port_read(fd_pad, v, sizeof v);
    }
    if (n == 0) { port_printf("[port] viewer closed: exiting\n"); port_exit(0); }   /* EOF */
    *out = port_window_pad;
    return 1;
}
