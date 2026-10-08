# Guía de matching (para trabajar de forma autónoma)

Objetivo: escribir C en `src/<Nombre>.c` que, compilado con la cadena original, produzca **exactamente**
los bytes de la función en el juego. Verificación: `tools/matchcheck.py` (enmascara relocaciones).

## Cadena de compilación (ya configurada en matchcheck)
- **CC1PSX de Psy-Q 4.3** (GCC 2.7.2.SN.1, Win32 vía Wine): `/opt/psyq/cc43/CC1PSX.EXE`, flags por defecto `-O2 -G0`.
- CPPPSX 2.7.2 (DOSBox) para el preprocesado; **ASPSX 2.86** (Wine) para ensamblar.
- El CC1PSX de DOS que usamos al principio tiene otro planificador: no lo uses (`CC1_WINE=0` solo para pruebas).

## Ciclo de trabajo
1. `python3 tools/fn.py <addr>` → ensamblador (capstone) + decompilado de Ghidra.
2. Escribe `src/<Nombre>.c`. Cabecera obligatoria `// FUNC <addr> <size> [MAIN0|X000]`; opcional `// FLAGS -O2 -G0 ...`.
   Objetos: `#include "TOBJ.H"` (include/tobj.h). Globales: `extern T DAT_xxxxxxxx;` (el nombre no importa).
3. `WORK=/opt/psyq/w_<id> WINEPREFIX=/opt/wine python3 tools/matchcheck.py src/<Nombre>.c [--asm]`
   (`--asm` deja el .s en `build/`; `--score` da la distancia 0 = idéntico). Un WORK propio por agente.
4. Con `MATCH`: `... matchcheck.py --mark src/<Nombre>.c` (añade `// MATCHING addr size`); borra `src/wip/<Nombre>.c` si existía.
5. Si no sale en ~6 intentos de verdad distintos: deja el mejor en `src/wip/<Nombre>.c` (sin MATCHING) y sigue.

## Antes de empezar una función
- **Comprueba el tamaño**: Ghidra corta funciones con jump table o con un `j` al epílogo. El final real es el
  `jr $ra` seguido del siguiente `addiu $sp,-N`; pon el tamaño real en `// FUNC` (FUN_80121fe0: 312, no 144).
- Fragmentos (empiezan con epílogo, usan `$sN` sin cargarlo, `j` suelto, <8 B) no son matcheables: sáltalos.
  `notes/todo_match3.csv` ya marca los fragmentos evidentes (`tools/classify.py`).
- Wips antiguos escritos para el compilador DOS (char* y offsets a mano): suele compensar reescribirlos de cero
  con TObj/switch; quita `-fno-delayed-branch` y `-g` de sus FLAGS.

## Recetas por síntoma (GCC 2.7.2 / CC1PSX 4.3)

### Orden de loads de globales frente a stores (lo más frecuente)
gcc 2.7 decide alias por MEM_IN_STRUCT: un **global escalar** de dirección fija nunca choca con un acceso
"in struct" (`o->campo`, array), así que el planificador **sube** su load por encima de esos stores.
- El juego lee el global **después** de los stores → declara el global como array y usa `X[0]`
  (`extern int X[];`), o como miembro de struct (`extern G DAT; DAT.y`), o simbolo+offset. También sirve para
  que un **store** a global conserve su orden. (Si el array se usa 2+ veces en el bloque, su dirección va a un registro.)
- El juego lo **sube** → deja el global escalar y accede al objeto con campos de struct (TObj), no con `*(short*)(p+off)`.
- Al revés: stores con casts `char*`/offsets crudos hacen que gcc recargue globales tras cada store; con campos
  TObj se quedan en registro (ObjSpawn).
- Para lecturas secuenciales de un puntero global usa `*DAT_ptr++` (`p[1]` cuenta como in-struct y se adelanta).

### Accesos con la dirección en registro (`lui r; addiu r,lo; lhu 0(r)`)
- Un único acceso así = **volatile en el punto de uso**: `*(volatile unsigned short *)&DAT` (no basta con el extern volatile),
  o `extern volatile unsigned short DAT[]` + `DAT[0]` (mando en `DAT_8009d670`). Una dirección literal da `lui+ori` y no sirve.
- Dos lecturas así sin fusionar = `volatile unsigned short *k = &DAT;` usado dos veces, asignado dentro de la rama donde se usa.
- Base en un s-reg para varios campos a través de llamadas = puntero local (`unsigned short *g = DAT; g[0]/g[1]`,
  `int *g = &G;` con la primera lectura pronto en el fuente).
- Struct de scratchpad con base en s-reg (`lui/addiu sym+0x30`, resto como `s1-0x30`): `extern S DAT_1f8000c0;` y campos.
- Dos `lw` del mismo campo sin store entre medias: haz volatile SOLO la segunda lectura (`*(T *volatile *)&s->f` o
  `*(volatile int *)&o->f`).

### Orden de operandos (`addu`, `or`)
- La expansión de ASPSX: dirección **literal** `*(int *)(k + 0x801fd80c)` → `addu at,reg,at`; **símbolo extern** → `addu at,at,reg`.
- `x << 2` en vez de `x * 4` pone la base primero; `(char *)o + idx + 0x64` frente a `&arr[idx]`; base como `int`
  y `(T*)(x + code) + 1` conserva el orden del fuente. `a + (b+0x10)` ≠ `(a+b)+0x10`.
- `or` con orden equivocado: usa una variable distinta como destino (`m = btn & 0xff0f; btn = a | m;`).
- Patrón `sll; addiu K; addu D` = `(int)&((struct{char pad[K]; int a[1];}*)D)->a[i]`.
- Con CC1PSX 4.3, `int + (signed char)campo` sale con operandos invertidos y el orden del fuente no lo arregla.

### Parámetros (se ven en el prólogo)
- `move sN,a0` en el delay slot del primer `jal` = parámetro `unsigned char`. Copias duplicadas (`move s6,s1`),
  `move v1,a1 ... move s1,v1`, o `sll rX,aN,16` sin `sra` = parámetro `short` (escribe `x << 16` en el uso).
- Si en una llamada `a0`/`a1` no se carga pero conserva el parámetro, la función **recibe ese argumento**:
  pásalo (`ObjSetAnimFromTable(o)`) y se arregla la asignación de registros.
- `move a3,a0; move t0,a1` al inicio de una hoja = un `static __inline__` expandido (o a0/a1 usados como temporales).
- Parámetro `int` con `(short)` solo en el uso evita la extensión previa. `char *` se lee con `lbu`: usa `signed char *` para `lb`.
- Constante de dirección pasada a función: `extern char DAT_x[]` (no el literal).

### Ramas, switch y colas compartidas
- `slti`/`bltz` sobre un valor = árbol de comparaciones de un **switch**. Casos separados que van al mismo cuerpo
  ≠ `case 1: case 2:`; añade un `case` imposible (`case 99: break;`) o un `case 1: break;` vacío para rehacer el árbol.
  Switch de 4 casos dispersos que en el juego es jump table: añade un `case N:` ficticio para forzar tablejump.
- **Cola compartida** (`j L` con el valor en el delay slot y un `sh/sw` común) = la sentencia completa escrita en
  **cada rama** (cross-jumping): `if (a) o->x = 0xf0; else o->x = 0x50;`, una llamada completa por rama, división en cada rama.
- Un case que salta a mitad de otro (con `sw` en el delay slot del `j`) = `goto common` con una local asignada en ambos.
- Fallthrough sin `j` = case sin `break`. Bloques idénticos consecutivos = código duplicado en el fuente.
- `bgez; negu; j; sra` = `x < 0 ? -x >> 16 : x >> 16` (shift dentro de cada rama).
- `addiu` en el delay slot de un branch antes del if = la expresión completa se calcula antes del if.
- `if (call() && inline_attr(o)) return 1;` para un call==0 que salta directo a la siguiente comprobación.
- Temporales compartidos entre cases acaban en t0/a3: decláralos locales al bloque.

### Bucles
- gcc rota `for(;;){...; if(--n<1) break; ...}`. Si el juego tiene el test arriba y un `j` al inicio: `loop:` + `goto loop`.
- `lh v0; move sN,v0` con test sobre v0 y luego `sll v0,n,16; bnez` = local `short n` con `while (n != 0)`.
- Puntero de bucle cuyo init va tras las constantes izadas = `e = &arr[i];` dentro del bucle; `-fno-strength-reduce` quita givs sobrantes.
- Dos bucles consecutivos con "el mismo" puntero en registros distintos = dos variables distintas.

### CSE, constantes y tipos
- `andi x,0xffff` innecesario antes de test de bits = `t % 32 == 0`; usa `(t & 0x1f) == 0` si no aparece.
- Resta que gcc pliega (`y-(v-8)` → `(y+8)-v`): temporal `unsigned short b = v - 8;` solo en esa rama.
- Campo leído tras un `sh` de constante elegida por condición = dos stores en if/else.
- `x ? 2 : 1` en un campo = `if (c) *p = 2; else *p = 1;`. Resultados 0/1 sin `xori/andi` = inline que devuelve `short`.
- Lee el campo en un `int` temporal antes de una resta truncada para conservar `lhu`; `x %= 10` reutiliza el cociente de `short q = x/10`.
- Constantes `char` negativas: `*(signed char *)&o->campo = -30;`.
- Hexadecimales tipo `0x9e-0x74` forman un solo pp-number por la `e-`: escribe el valor decimal.
- `addPrim` con máscaras `0xff000000/0xffffff` = bitfield `struct {unsigned addr:24; unsigned len:8;}`.
- Mismo global leído con `lhu` y con `lh`: dos externs con distinto tipo y nombre (el nombre no importa).

### Prólogo, epílogo y frame
- Con CC1PSX 4.3 el epílogo `j $31; addu $sp` (delay slot lleno) sí sale cuando solo se guarda `$ra`;
  con s-regs guardados (librería 0x8006xxxx) no se reproduce: déjalas en wip.
- Si el juego carga una global antes de `addiu sp` o adelanta una constante: guárdalas en locales antes del if (`n = g; c = 2;`).
- Frame sin saves: `char pad[16];` (16 B) o `char pad;` (8 B) sin usar. Frames 0x38/0x10 sin saves suelen ser inlines.
- Declara las locales al principio del bloque (a mitad de bloque gcc 2.7 llegó a descartar una sentencia).
- Algunas funciones necesitan `// FLAGS -O1 -G0` (base-reg `sw x,0(v0); sw y,4(v0)`) o `-O2 -G0 -fno-schedule-insns`.

## Reglas de git (hay varios agentes a la vez)
- Solo `master`, sin ramas ni PRs. Commits pequeños cada 3 matches.
- El índice es compartido: **`git commit -m "..." -- <tus rutas>`** (un `git commit` normal se lleva lo que otros tengan en stage).
- `git pull --rebase origin master`; si hay conflicto solo en `src/wip` o `notes`: `git rebase --abort` y
  `git merge -X theirs origin/master`. No uses `git stash -u` (hay ficheros sin trackear de otros agentes).
- Usa un directorio temporal propio (`/tmp/<id>`): el scratchpad de la sesión es compartido.
- No reescribas `include/tobj.h` (crea `include/<nombre8>.h`). Nunca subas el SDK, binarios del juego ni decompilados crudos.

## Historial (resuelto)
- ASPSX 2.34 (DOS) emitía `ori` para `li`; se usa ASPSX 2.86. Versiones 2.56/2.77/2.81 no aportan nada.
- El CC1PSX DOS ponía `addiu sp` después del primer load de global; el CC1PSX 4.3 lo resuelve (con la receta de arrays).
- `tools/permute.py` (mutaciones aleatorias, solo CPU) dio 1 acierto en 32 casos: útil solo para diferencias de tipos.
