/* Runtime interface of the native-pointer port (port2/runtime). */
#ifndef PORT2_RUNTIME_H
#define PORT2_RUNTIME_H
void port_printf(const char *fmt, ...);
void port_fatal(const char *fmt, ...) __attribute__((noreturn));
void port_stub_hit(const char *name);
int port_check_layout(void);
#endif
