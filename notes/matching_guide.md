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

## Aprendido (shard3)
- **Funciones con varias globales y base-reg (`lui v0; addiu v0,..; sw x,0(v0); sw y,4(v0)`)**: usar `// FLAGS -O1 -G0` (con -O2 gcc emite `sw x,D+4`). FUN_8006957c.
- **Stores que el scheduler reordena respecto a loads de globales** (original: todos los stores en orden de fuente y luego los loads): declara el puntero destino `volatile int *o` / `volatile char *` en -O2. ObjFreeDup.
- **Cadena de `beq` por rangos de un campo de bits (`x & 0xc000` con casos 0/4000/8000/c000) = `switch`**. El bloque de cola compartido (`o->t = v&0x3fff; return 0`) que en el binario esta ENTRE los casos se reproduce con `goto tail` desde el caso 0 hacia dentro del caso 0x4000. Las asignaciones de registros se arreglan usando una variable temporal distinta (`w`) en los casos que la comparten. AnimAdvance.
- Frame de 0x10 sin saves en una hoja (`addiu sp,-0x10` incluso en delay slot de la 1a rama): declarar `char pad[16];` sin usar.
- Para ver rapido flags: script tipo `for fl in "-O2 -G0" "-O1 -G0"...` sustituyendo la linea `// FLAGS`.
- NO uses `git stash -u` en el arbol compartido (otros agentes tienen ficheros sin trackear); usa `git pull --rebase --autostash`.
- **`move $a3,$a0; move $t0,$a1` al principio de una hoja** = el original usa una funcion `static __inline__` con esos parametros (gcc 2.7 copia los args a regs nuevos al expandir). Reproducirlo con un `static __inline__ int hit(TO *a, TO *b)` (FUN_8004461c / FUN_8004432c, en wip: esa parte ya coincide, falta el resto del scheduling).
- **Orden del prologo (`addiu sp` ANTES de `lui/lw` global, con el `li`/`sw ra` despues)**: nuestro -O2 mueve el `subu sp` al delay slot del primer `lh/lw` global; en el binario esta primero. NO resuelto (probado: volatile, pad, literal de direccion, -fno-schedule-insns2, if/else invertido). Afecta a FUN_80114890, FUN_800184d8, FUN_80059728 (wip, solo difieren en eso). Si alguien lo resuelve, arregla varias.
- Un `char pad;` sin usar reproduce frames de 8 bytes sin saves (`char pad[4]` da 16).
- Para fijar `lhu` en la lectura y `lh` en el test del bucle sobre la misma global, declara dos externs con el mismo simbolo de distinto tipo (`DAT_x` short y `DAT_x_u` unsigned short): el nombre no importa para los bytes.

## Aprendido (hard.csv, funciones medianas)
- **Declaraciones a mitad de bloque** (`int t = ...;` tras otras sentencias) hicieron que gcc 2.7 descartase la sentencia: declara siempre las locales al principio de la funcion (FUN_80108f60).
- **Orden de stores del scheduler**: si solo difiere la posicion de UN store en una cadena de stores, prueba a moverlo a todas las posiciones (script trivial: mover cada linea a cada hueco y llamar a matchcheck). FUN_80123648: el `sw s2,0x94` iba tras el `0x18`.
- **Arbol `beq 1 / slti 4`** con `x<4 ? x&1 : 3`: escribe `if (t != 1) {ternario} else {...}` (el bloque fallthrough es el primero). FUN_80125984. Retorno `short` de callee => `sll/bltz` (declara `extern short f()`).
- **Parametros `int` con `(short)` solo en el uso** (p.ej. `sll s0,a1,5` sin extension previa): declara el parametro `int` y castea al pasarlo (FUN_801306f0, en wip por asignacion s0/s1).
- Constantes de direccion `lui+addiu` que se pasan a funciones: declara `extern char DAT_x[]` y pasa el array (no el literal 0x800d7e28); si se usa 2 veces gcc la guarda en un s-reg.
- **Epilogo `jr $ra; addiu $sp` (delay slot lleno)** aparece en las funciones de libreria 0x8006xxxx (libcard...), con la logica ya identica (FUN_8006911c, FUN_800693c8, FUN_80069410): no se arregla con flags (-O1/-O3/-fno-delayed-branch/-mips2 probados). Dejar en wip.

## Ensambladores probados (resultado: ninguno mejora)
ASPSX 2.34 (DOS), 2.56, 2.77 (4.3/4.4), 2.81 y 2.86 (4.6). Con 2.81/2.86 salen las mismas 205 exactas;
2.56/2.77 son algo peores. Los casos que fallan (store en el delay slot de `jr`, `addiu sp` tras el primer
load de global, `jr; addiu $sp`) no dependen de la versión del ensamblador. `tools/permute.py` tampoco
los arregla (12 funciones a distancia 2-6: 0 aciertos): el hueco es sistemático, no de tipos.

## Hallazgo: orden del prologo (ThreadWaitFrames y similares)
El juego emite `addiu sp` ANTES de `lui/lw` del primer acceso a global; nuestro CC1PSX los pone
despues, con cualquier flag (-O1/-O3/-fno-schedule-*, etc.) y con simbolo o direccion absoluta.
Es la causa de casi todos los wip a distancia 2-6. Hipotesis: el juego usa otra build de CC1PSX 2.7.2.SN.1
(las de Psy-Q 4.0-4.4 difieren en el planificador). Pendiente: CC1PSX.EXE de otras versiones del SDK.

## Aprendido (CC1PSX 4.3, tanda wip)
- Quitar `-fno-delayed-branch`/`-g` de wip antiguos: con el CC1PSX 4.3 varios coinciden solos (FUN_80102a08).
- Argumentos puntero consecutivos `(c, a+1, a+2, b)` donde el original hace `addiu a1,a0,4; addiu a2,a1,4`: incrementa el parametro (`a++; f(c, a, a+1, b)`), no uses variable `p` aparte (FUN_80024e58).
- Un `lw` repetido del mismo campo (leer `o->d` antes y despues del store) se reproduce escribiendo `o->dur = o->d[n].v & ..; o->d += n;` en ese orden, sin temporales (AnimJump).
- `x ? 2 : 1` guardado en un campo: escribe `if (c) *p = 2; else *p = 1;` (dos stores) para que gcc no comparta la constante (FUN_80019a08).
- Orden de un `sll` respecto a `lui/addiu` global: asigna el puntero global antes de `n <<= 2;` y escribe `q + n + 4` (FUN_8003509c). `a + (b+0x10)` vs `(a+b)+0x10` cambia el orden de addiu/addu (FUN_80057368).
- Parametros `char *` que se leen con `lb` deben ser `signed char *` (char es unsigned en este gcc: lbu).
- Muchas entradas de los shards (m1/m2/m3) son fragmentos de Ghidra (switch con jump table cortado, `jr $ra` en medio): comprobar que el ultimo `jr $ra` y que no hay `jr $reg` antes de intentar.
