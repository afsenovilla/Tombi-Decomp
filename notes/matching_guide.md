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

## Aprendido
- `li` con constante positiva: ASPSX 2.34 (DOSBox) emite `ori`, pero el juego tiene `addiu $x,$zero,N`. matchcheck.py ahora
  ensambla con ASPSX 2.86 (`/opt/psyq/46/BIN/ASPSX.EXE`, via wine) que si emite `addiu`; cc1/cpp siguen siendo los de 2.7.2.
  `ASPSX_WINE=0` vuelve al ensamblador DOS. Los .C se compilan con el mismo cc1, solo cambia el ensamblado.
- Muchos "FUN_" pequeños en Ghidra son fragmentos (epilogos `lw $ra; addiu $sp; jr $ra`) o `j` a otra funcion: no hay C que los genere.
- Algunas funciones solo coinciden con `// FLAGS -O1 -G0` (p.ej. ObjListPush_1F800228: con -O2 el scheduler sube los lhu/lw).
  Si ves loads de globales (lui+lw) tras los stores en el original, prueba -O1.
- Para elegir el sentido de un `beqz` prueba a invertir `if/else` (el bloque fallthrough es el del `if`).
- Funciones sin frame que acaban con `jr $ra; addiu $sp` (delay slot relleno) tras varios `lw`: no he conseguido reproducirlo
  (el epilogo sale `addu sp; j; nop`); ejemplo en src/wip/FUN_80069410.c.
- El orden de las sentencias en C cambia la asignacion de registros s0..s3 (prioridad por orden de uso), aunque el scheduler luego
  reordene los stores: si los s-regs salen permutados, mueve el store del parametro "perdido" al principio (FUN_80020d20).
- Constantes `char` negativas (p.ej. `li $v0,-30` para un `sb`): escribe `*(signed char *)&o->campo = -30;` (con campo unsigned sale `li 0xe2`).
- Cuando el original lee `byte b = TABLA[idx]` ANTES de `o->state++`, escribelo asi (variable temporal) para que no cambie el orden de loads.

## Aprendido (shard0, segunda tanda)
- `slti` + `bltz` sobre un `lhu` (rangos tipo `x>=0 && x<3`) = **switch** de GCC (arbol de comparaciones), no `if`. Escribe
  `switch (x) { case 0: case 1: case 2: ... }`; con `unsigned short` los `>= 0` se pliegan si los escribes a mano.
  Dos `case` separados que van al mismo cuerpo generan cadena `beq/slti` distinta a `case 1: case 2:` (que se funde en rango).
- Un switch con casos 0 y 1 que sale como `beq v1,1; slti v1,2; bnez v1 -> fin` necesita un `case` extra imposible (p.ej. `case 99: break;` antes de `case 0:`)
  para que el arbol quede igual (FUN_800313c8).
- Si dos bucles consecutivos usan "el mismo" puntero, el original usa **variables distintas** (una en $s1, otra en $a0): declara `p` y `q` separados (FUN_8001801c).
- Parametro `unsigned char op` + `switch(op)` con 4 casos consecutivos y cuerpo por caso con `idx*4 + base + 0x1090` repetido por caso reproduce el codigo (FUN_800387e0, casi).
- Una direccion de global en registro (`lui r; addiu r,r,lo; lhu x,(r)`) aparece cuando la direccion se usa 2+ veces en el bloque (load+store con `*p`); con un solo uso no he logrado reproducirla.
- Si el original recarga un global en cada rama (dos `lhu` seguidos del mismo sitio) y gcc lo funde, declara el global con dos nombres distintos (`DAT_x` y `DAT_xb`): gcc 2.7 no los trata como alias (FUN_8001f1c0).
- Hay un script util: comparar tu .o con el juego instrucción a instrucción (diff unificado de capstone) acelera mucho mas que mirar el `DIFF` de palabras.
