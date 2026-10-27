/* Runtime interface of the native-pointer port (port2/runtime, port2/shims).  Only port2 code includes this. */
#ifndef PORT2_RUNTIME_H
#define PORT2_RUNTIME_H

typedef unsigned int u32p;

/* generated tables (build/port2/gen/funcmap.c by port/tools/gen_port.py, addrmap.c by port2/tools/gen2.py) */
typedef struct { u32p addr; void *fn; const char *name; } PortFunc;          /* MAIN0/X000 function by retail address */
extern const PortFunc port_funcmap[];
extern const int port_funcmap_count;
typedef struct { u32p id; u32p addr; void *fn; const char *name; } PortOvlFunc;   /* SYSTEM overlay set: header id + address */
typedef struct { u32p id; const char *name; } PortOvl;
extern const PortOvlFunc port_ovlmap[];
extern const int port_ovlmap_count;
extern const PortOvl port_ovls[];
typedef struct { int area; u32p addr; void *fn; } PortAreaFunc;      /* areas: sorted by (area, addr); area 0 = X000 */
typedef struct { int idx; const char *name; u32p sig[2][2]; } PortArea;
extern const PortAreaFunc port_areamap[];
extern const int port_areamap_count;
extern const PortArea port_areas[];
extern const int port_areas_count;
extern const unsigned int port_vary_addrs[];
extern const int port_vary_count;
typedef struct { u32p addr; void *fn; } PortAddr;                    /* sorted by addr */
extern const PortAddr port_addrmap[];        /* every MAIN0/X000 function (outside the overlay region) with native code */
extern const int port_addrmap_count;
extern const PortAddr port_thunkmap[];       /* overlay-region function start -> its dispatch thunk */
extern const int port_thunkmap_count;
typedef struct { int kind; u32p key; const char *name; const unsigned int *ranges; int n;
                 const unsigned int *starts; int nstarts; } PortOvlCode;   /* kind 0: area index, 1: SYSTEM id; ranges: code [s,e) pairs; starts: its function starts */
extern const PortOvlCode port_ovlcode[];
extern const int port_ovlcode_count;

/* the data image (build/port2/gen/data.c): the retail RAM layout as relocatable arrays */
extern unsigned int port_ram[0x80000];     /* the 2 MB of RAM in the retail layout; the regions are offsets into it */
extern unsigned int port_bios[], port_main0_image[], port_main0_bss[], port_x000_image[], port_x000_bss[], port_heap[],
    port_hiram[], port_scratch[], port_io[];
#define PORT_MAIN0_LOAD 0x80010000u
#define PORT_OVL_BASE 0x800E8028u
#define PORT_OVL_END 0x80140000u
#define PORT_AREA_OWN_CODE 0x80115EA4u   /* X0nn: first per-area function (the code before it is shared with X000) */
/* native pointer for a retail RAM / scratchpad address (runtime and debugging aids only; the game never does this) */
void *port_ram_ptr(u32p addr);
int port_is_ram_ptr(const void *p);
int port_is_missing(void *fn);                    /* fn is the dispatcher's "no native code" placeholder */
void *port_exec_from_ram(unsigned long eip, unsigned long fault);   /* lowmem.c: a call into RAM, dispatched */
int port_exec_from_ram_calls(void);
#define PORT_M0(a) ((unsigned char *)port_main0_image + ((a) - PORT_MAIN0_LOAD))
#define PORT_SPAD(a) ((unsigned char *)port_scratch + (a))
#define SPAD8(a) (*(volatile unsigned char *)PORT_SPAD(a))
#define SPAD16(a) (*(volatile unsigned short *)PORT_SPAD(a))
#define SPAD32(a) (*(volatile unsigned int *)PORT_SPAD(a))

/* overlay dispatch (runtime/ovl.c) */
void *port_resolve_call(u32p addr);              /* called by the thunks: native code of `addr` in the resident overlay */
int port_current_area(void);                     /* index into port_areas (0 = X000), -1 if none matches */
const char *port_resident_overlay(void);
void port_relocate_overlay(void *dst, int bytes);   /* after a CD read into the overlay region: retail pointers -> native */
const char *port_sym_near(void *fn, unsigned long *off);

/* logging, exit */
void port_printf(const char *fmt, ...);
void port_fatal(const char *fmt, ...) __attribute__((noreturn));
void port_quit(int code) __attribute__((noreturn));
void port_stub_hit(const char *name);
void port_stub_report(void);
void port_low_report(void);
extern volatile unsigned long port_low_count, port_low_writes;   /* lowmem.c: emulated null-page accesses so far */
void port_low_second(char *buf, int size);                       /* " lowmem: N top sym+off->addr:n ..." since the last call */
extern volatile int port_log_direct;         /* > 0: port_printf writes straight to fd 2 / the stderr handle (no CRT stream lock) */
extern unsigned long port_log_direct_tid;    /* Win32: the host thread whose port_printf always goes the direct way (the watchdog) */
void port_dump_stack(unsigned long esp);     /* return addresses into the image found above esp (lowmem.c) */
void port_flush_log(void);
void port_trace(const char *what, int arg);  /* the last shim calls, for the watchdog report (watchdog.c) */
const char *port_trace_last(int *arg, int *frame);   /* the newest one */
extern unsigned long long port_prof_draw_us, port_prof_wait_us, port_prof_present_us;   /* per-frame breakdown of the fps line */
void port_watchdog_start(void);
extern int port_watchdog_seconds;
int port_check_layout(void);
extern int port_verbose;

/* time */
unsigned long long port_time_us(void);
void port_sleep_us(unsigned long long us);
void port_sleep_until_us(unsigned long long deadline);

/* cooperative threads (Psy-Q OpenTh/ChangeTh) */
int port_thread_open(void *fn);
void port_thread_change(int handle);
void port_thread_close(int handle);
int port_thread_current(void);
int port_thread_used(int n);
extern unsigned long port_thread_yield_site[][4];   /* threads.c: where each coroutine last yielded */
extern unsigned port_thread_switches, port_ot_signature;

/* frame pacing / vblank */
void port_vblank(void);
extern int port_frame;
extern int port_max_frames;
extern int port_realtime, port_hz;

/* CD image */
int port_cd_open(const char *path);
int port_cd_open_data(const char *dir);          /* data/ folder of tools/extract_data.py (manifest.txt) */
int port_cd_read_sectors(int lba, int count, void *dst, int sector_bytes);
unsigned char *port_cd_read_named(const char *path, unsigned *size);   /* a whole file by ISO path (disc directory / data folder), malloc'd */

/* the game's language (runtime/lang.c): MAINn's file table and data, the language's overlays over the MAIN0 layout */
extern int port_lang;                          /* 0 English (MAIN0, as generated), 5 Espanol, 6 Portugues, 7 Svenska, 8 Dansk, 1..4 the other MAINn */
extern int port_lang_box_pad[5];               /* printInfoMessage's box width by message kind, from MAINn's code */
const char *port_lang_name(int n);
void port_lang_init(void);                     /* after the game data is open, before the game starts */
int port_lang_overlay_loaded(void *dst, int bytes, int id);   /* CdRead into the overlay region: compose over the MAIN0 twin; 1 if done */
int port_lang_take_composed(void);
int port_lang_xlate_word(u32p *v, void **native);             /* a word of a language file -> MAIN0 layout (1: *native is final) */
const PortOvlCode *port_ovl_code_ranges(void);                /* ovl.c: code ranges of the resident overlay */
int port_ovl_in_ranges(const PortOvlCode *c, u32p a);
extern int port_trace_cd;
extern const char *port_disc_path, *port_data_dir;

/* GPU (shims/gpu.c; the backends are in shims/gpu_backend.h: software, or OpenGL in shims/gpu_gl.c) */
extern const char *port_dump_dir;
extern int port_dump_every, port_dump_vram;
void port_gpu_start(void);                       /* after the window exists: picks the backend (--renderer soft|gl) */
void port_gpu_stop(void);
void port_gpu_vblank(void);
int port_gpu_frame(unsigned char *rgb, int *w, int *h);

/* sound (shims/sound.c): a software SPU + libsnd on the sample clock */
void port_sound_vblank(void);                    /* no audio device: mixes 44100/hz frames per vblank */
int port_sound_pull(short *out, int frames);     /* the audio device's callback: mixes `frames` stereo frames */
void port_sound_pulled(int on);                  /* 1 while an audio device drives the mixer through port_sound_pull */
void port_sound_xa_sector(int coding, const unsigned char *payload);   /* an XA audio sector (2324 bytes) the stream ring read */
void port_sound_xa_reset(void);                  /* a new stream starts */
void port_sound_xa_end(void);                    /* the stream stopped: the queued audio plays out */
void port_sound_cd_mute(int mute);               /* CdlMute / CdlDemute */
void port_audio_lock(void);                      /* runtime/sdl.c: the game thread holds the device's callback off */
void port_audio_unlock(void);

/* pad (shims/pad.c) */
void port_pad_vblank(void);
extern unsigned short port_pad_buttons[2];

/* SDL2 window + audio + input (runtime/sdl.c) */
extern int port_window;                          /* --window */
extern int port_window_scale, port_window_fullscreen;
void port_window_keys(const char *spec);          /* --keys up=Up,cross=X,... */
void port_window_start(void);
void port_window_frame(const unsigned char *rgb, int w, int h);
int port_window_poll_pad(unsigned short *out);   /* 1 when a window is open */
void port_window_stop(void);
int port_window_gl_start(void);                  /* OpenGL context on the window (a hidden one when headless); < 0 = none */
void port_window_gl_size(int *w, int *h);
void port_window_gl_swap(void);
#endif
