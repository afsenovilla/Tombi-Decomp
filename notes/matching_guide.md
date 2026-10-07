# Guía de matching (para trabajar de forma autónoma)

Objetivo: escribir C en `src/<Nombre>.c` que, compilado con **GCC 2.7.2.SN.1** (flags por defecto
`-O2 -G0`), produzca **exactamente** los bytes de la función en el juego.

## Ciclo de trabajo
1. `python3 tools/fn.py <addr>` -> ensamblador (capstone) + decompilado de Ghidra.
2. Escribe `src/<Nombre>.c` (ver `src/MulCos.c`, `src/ObjApplyVelocity.c`). Cabecera obligatoria:
   `// FUNC <addr> <size> [MAIN0|X000]` y opcional `// FLAGS -O1 -G0`.
   Para objetos: `#include "TOBJ.H"` (include/tobj.h; campos `o->h->raw`, `o->y.raw`, ...). Los
   globales: `extern int DAT_8007a5f0;` o arrays (el nombre del simbolo no importa para los bytes).
3. `WORK=/opt/psyq/w_<id> python3 tools/matchcheck.py src/<Nombre>.c [--asm]` (usa un WORK propio si
   trabajas en paralelo). `--asm` deja el .s en `build/` (ignorado por git).
   `DIFF` muestra las palabras que difieren (ya sin relocaciones); compara con `tools/fn.py`.
4. Cuando de `MATCH`: `python3 tools/matchcheck.py --mark src/<Nombre>.c` (añade `// MATCHING addr size`).

## Pistas de GCC 2.7.2 (que cambian los bytes)
- Prueba `-O1`, `-O2`, `-O0` y `-G0`/`-G8`/`-G4`... Si el original usa `$gp` (acceso `0x....($gp)`), hace falta `-G<n>`
  y que el simbolo sea pequeño (`extern int x;` definido como small data); si usa `lui`+`lw`, -G0.
- El orden de las declaraciones locales, `register`, el uso de `short`/`int`/`unsigned`, tipos de
  retorno, `if/else` vs `?:`, `for` vs `while` y el orden de las expresiones cambian el codigo.
- Los delay slots los rellena el compilador/ensamblador; no hay que escribir `nop`.
- Las constantes de 16.16 (`<< 8`, `>> 16`) conservan el tipo (`short` vs `int`): fijate en `sll/sra 16`.
- Divisiones por constante, `switch` (tablas de saltos), `mult/mflo` (`*`), `div` aparecen tal cual.
- Si una funcion tiene un bucle o llamadas, comprueba que el prologo (`addiu $sp,..`, `sw $ra`) coincide.
- Las llamadas a funciones (`jal`) se enmascaran en la comparacion: declara `extern` los prototipos
  con los tipos de los argumentos que se ven en el ensamblador.

## Reglas
- No reescribas `include/tobj.h`. Si necesitas tipos nuevos crea `include/<nombre8>.h` con nombre propio.
- No intentes forzar un match durante mas de ~10 iteraciones: deja el mejor intento en
  `src/wip/<Nombre>.c` (sin la marca MATCHING) y pasa a la siguiente funcion.
- Cuando un match sea verificado: `git add src/<Nombre>.c; git commit; git pull --rebase origin master; git push origin master`
  (solo `master`, sin ramas ni PRs). Haz commits pequeños y frecuentes.
- Nunca subas el SDK, los binarios del juego ni decompilados crudos.

## Aprendido
- **`li` con constante pequena (`addiu $a2,$zero,N`)**: el ASPSX 2.34 de DOS genera `ori`, el juego usa `addiu`.
  Solucion: `export ASPSX_WINE=/opt/psyq/46/BIN/ASPSX.EXE` (ASPSX 2.86 via wine; matchcheck lo usa en vez del 2.34).
  Con esto coinciden ademas MulCos/MulNegSin/ObjApplyVelocity. Usalo SIEMPRE.
- Llamadas con `extern void f();` sin prototipo o con prototipo dan el mismo codigo; declara los argumentos reales si hay >4 (los extra van a `0x10($sp)`).
- Funciones "FUN_" que empiezan sin prologo y usan `$s0` sin cargarlo son fragmentos mal cortados por Ghidra: no son matcheables, saltalas.
