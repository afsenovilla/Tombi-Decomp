Headers of psx_tomba at commit 3aa3d19 (2026-10-09, PR #24). They supersede include/tomba/game.h for files that
carry a `// HEADERS tomba24` line: tools/ncheck.py overlays this directory onto include/tomba for those files only,
so the files ported earlier (which declare types the new game.h now also declares) keep compiling unchanged.
