# Los pueblos recortados de la Tierra de la Civilización (vista desde arriba)

*English version: [CIVILIZATION.md](CIVILIZATION.md). Este documento es su traducción completa y se mantiene a la par.*

La sección 0 de AREA11, AREA16, AREA17 y AREA18 iba a ser un pueblo visto desde arriba, con el mismo motor que las
vistas generales de Ciudad Baco y del Lago Baco (`SYSTEM/I0NFO2.BIN`). El código de estos pueblos está en el disco PAL
y está decompilado al 100 % en `src/i0nfo2/`. Las posiciones de los vecinos y de los objetos también están en el
disco, como datos dentro de I0NFO2. Lo que falta son los datos de la sección: las listas de carga están vacías, así
que no hay dibujo de fondo, mapa de celdas ni tablas de sprites. Las paletas de los vecinos, en cambio, sobreviven en los archivos CLUT de las áreas (ver Paletas). Esta página trata dos cosas:

1. Todos los formatos de datos que lee el motor de vista desde arriba. Cada uno está comprobado contra una sección que
   funciona con `tools/topdown.py`.
2. Qué esperaba cada sección recortada: sus personajes, comportamientos, tablas de sprites, banderas, eventos,
   salidas y diálogos japoneses.

Como con `game/`, no se sube nada sacado del disco: ni imágenes, ni volcados de texto, ni bloques extraídos. Ejecuta
las herramientas sobre tu propia copia del disco.

Direcciones: MAIN0 es el `SCES_013.31` PAL. Las funciones y datos de I0NFO2 están en su dirección de ejecución (base
`0x800E8028`, archivo `SYSTEM/I0NFO2.BIN`, 0x218DC bytes, 196 funciones, 100 % decompiladas). Los textos se citan como
`(k, dx)`: el banco y el índice que reciben las funciones de mensajes. El número de diálogo que se ve es
`base[k] + dx`, donde `base[k]` es la tabla u16 en `WFM + 0x10`. Para los WFM de los pueblos estas bases son:

| Archivo | k=1 | k=2 | k=3 | k=4 | k=5 |
|---|---|---|---|---|---|
| `AREA11/MFNT110S.WFM` | 207 | 208 | 227 | 249 | - |
| `AREA16/MFNT160S.WFM` | 207 | 208 | 230 | 248 | - |
| `AREA17/MFNT170S.WFM` | 207 | 208 | 257 | 269 | 270 |
| `AREA18/MFNT180S.WFM` | 207 | 209 | 223 | - | - |

## Qué lugares son estas áreas

El README llama a las áreas 11/16/17/18 "Village of Civilization (…)". El texto del juego da nombres mejores (los
rótulos de los puntos de guardado son el diálogo 207 del `MFNT*S.WFM` de cada área):

| Área | Lugar en el juego (texto PAL en español) | Pruebas |
|---|---|---|
| AREA18 | **Ciudad de civilización**, el centro (Civilization Town) | rótulo #207 "Ciudad de civilización"; cartel #208 "↑ Castillo de hierro → Torre reloj ↓ Serrería"; la bruja Mizuno #223-#237; las salidas de s1 llevan a AREA11 s1, AREA17 s1, AREA16 s4 y AREA10 |
| AREA16 | **Ciudad de Piedra**, el barrio de la gente de piedra, con la **Torre reloj** | #207 "Ciudad de Piedra"; #213-#223 la torre del reloj parada / arreglada por el Minero; #248 "Los piñones de piedra están rotos" |
| AREA17 | **Ciudad de hierro**, el barrio de los Hombres de hierro, con el **Castillo de hierro** | #207 "Ciudad de hierro"; #220 "la cámara de piñones"; #240-#247 la bolsa Cerdiablo, el Anciano de 10.000 años, la balsa "en la fábrica" |
| AREA11 | la **Fábrica** y la central eléctrica, del lado de la gente de madera | #220 "Estoy al mando de esta fábrica"; #211 carbón; #212 "la central eléctrica funciona de nuevo"; #218 el coche GoGo; #221-#226 la bomba para la puerta del Castillo de hierro; #229-#233 el tambor Funga |

Los lugares que se preguntaban:

- La **Torre reloj** está en AREA16 (#213/#214/#220/#221/#223). AREA10 #258 la llama "la Torre del reloj" y el cartel
  de AREA18 #208 señala hacia ella.
- La **Torre de vigilancia** no es de este grupo. Es un punto de guardado de AREA01/AREA07 (`MFNT010S` #211, Bosque de
  las 100 flores / Bosque de Enanos) y aquí sólo se menciona (AREA11 #213 "un ascensor cerca de la Torre de vigilancia
  en el Bosque de Enanos", AREA18 #225).
- Las **campanas de 10.000 años y de un millón de años** son nombres de objetos. Están en #154/#155 del MFNT de todas
  las áreas (la lista común de objetos), así que no sitúan nada. El Anciano de 10.000 años vive "cerca de la piscina
  encima de la cascada" (AREA17 #243).
- **Máquina de civilización** es un título de evento (`CFNT999S.WFM` #95, la lista común de eventos), junto a
  "¡Rompe la puerta oxidada!" #93, "Necesito una bomba" #94 y "¡Vamos en la balsa!" #97. Son las misiones de la
  Fábrica, el Castillo de hierro y la balsa de AREA11/17.

El texto japonés dice qué eran los pueblos recortados. AREA11 k4 es el pueblo de la **gente de madera** (木人) ("la
madera es lo mejor", la choza del constructor de balsas, un laberinto bajo el suelo). AREA16 k3 es el pueblo de la
**gente de piedra** (石人) (el ascensor de la pared, el engranaje de la torre del reloj). AREA17 k5 es el pueblo de los
**Hombres de hierro** (鉄人) ("Bienvenido... éste es el pueblo de los Hombres de hierro", el ascensor, el doctor
Gantetsu, el Castillo de hierro). AREA18 k2 es el **pueblo reunificado**, después de que cae el muro del Cerdiablo
("hemos vuelto a ser un solo país... gracias a Tombi", "más de cuatro veces el tamaño del antiguo pueblo de piedra").
Así que las cuatro secciones desde arriba son los tres pueblos enfrentados más el pueblo del final. El juego final los
sustituyó por los barrios de vista lateral.

## Paso 1: los formatos de datos

### Dónde funciona el motor

El motor de vista desde arriba sustituye al overlay del área: la lista de la sección carga `I0NFO2.BIN` (tipo 0xF0)
en la base de overlays en lugar de `X0nn.BIN`. Secciones del juego que lo usan, y las secciones para las que se
escribió:

| Sección | Lista de carga (`SYS/LD0ARnn` lista s+1) | Conjunto de vecinos (variante) | Controlador de sección (`D_80079AD0[area+7]`) | Manejador de personajes / decorado |
|---|---|---|---|---|
| AREA05 s0, s2 (Ciudad Baco) | completa | 0x07 `updateNpcSet07` | `topdownSectionStepA` 0x800E8B28 | `topdownActorHandler` 0x800F05B0 / `topdownA05SceneryHandler` 0x800F07C0 |
| AREA08 s0, s2 (Lago Baco) | completa | 0x0C `updateNpcSet0C` | `topdownSectionStepB` 0x800E8C98 | `topdownA08ActorHandler` 0x800F18C0 / `topdownA08SceneryHandler` 0x800F1A60 |
| AREA19 s2 (Ciudad oculta) | completa | 0x11 `updateNpcSet11` | `topdownSectionStepBySection` 0x800E9174 | `topdownA19ActorHandler` 0x800F6B4C / `topdownA19SceneryHandler` 0x800F1590 |
| **AREA11 s0** | **vacía** | 0x0D `updateNpcSet0D` | `topdownSectionStepC` 0x800E8D98 | `topdownA11ActorHandler` 0x800F2CA8 / `topdownA11SceneryHandler` 0x800F6D5C |
| **AREA16 s0** | **vacía** | 0x0E `updateNpcSet0E` | `topdownSectionStepD` 0x800E8ED4 | `topdownA16ActorHandler` 0x800F3D84 / `topdownA16SceneryHandler` 0x800F0C0C |
| **AREA17 s0** | **vacía** | 0x0F `updateNpcSet0F` | `topdownSectionStepE` 0x800E9040 | `topdownA17ActorHandler` 0x800F528C / `topdownA17SceneryHandler` 0x800F126C |
| **AREA18 s0** | **vacía** | 0x10 `updateNpcSet10` | `0x80116234`: no es una función de I0NFO2 (ver AREA18) | `topdownA18ActorHandler` 0x800F548C / ninguno |

Cómo se conectan las piezas, todo en MAIN0:

- **Objetos de sección.** `spawnSectionUnlayeredObjects` (0x8004BFE4) recorre `PTR_8007BEE4[area][sección]`, una
  lista de entradas de 4 bytes `{tipo, variante, ?, init}`. Toda sección desde arriba lista el tipo 2 (generador), el
  tipo 1 (controlador de sección / fundidos) y el tipo 4 con la variante del conjunto de vecinos. El tipo 4 es
  `dispatchUnlayeredByVariant` (0x8004D3B8), que llama a `D_8007C1C8[variante]`: la entrada 7 es `updateNpcSet07` y
  las entradas 0x0C..0x11 son `updateNpcSet0C..11` (I0NFO2).
- **Controlador de sección.** El subtipo del controlador indexa `D_80079AD0`. Los despachadores están en las entradas
  9 (AREA02), 12, 15, 18, 23, 24, 25 y 26, es decir `area + 7` para cada área que tiene uno. Lo deducimos de la
  disposición de la tabla, y cuadra con lo que llama cada despachador. Las entradas 12..26 son los despachadores por
  área que en `src/` se llaman `dispatchSectionUpdate/Draw/Input/Scroll/Select/Close/Confirm`. Esos nombres no
  describen lo que hacen: cada uno elige la función de paso de I0NFO2 para la sección 0 (y 2) y la función de X0nn
  para las demás. La función de paso comprueba los rectángulos de salida y llama a `tryStartItemInteraction`
  (0x800270A0, que prepara la salida n). Después `cameraEventScrollHandler` (0x8002A4D0) lee el registro de salida de
  `D_80079A48`.
- **Personajes.** La capa 2 tipo 0x30 es `updateL2Type30Object` (0x8003BCD0), que según el área salta al
  `topdownAnnActorHandler` de la tabla de arriba.
- **Decorado y objetos.** La capa 3 tipo 0x1D es la función de MAIN0 llamada `dispatchAreaDialogInit` (0x8002E414).
  Ese nombre también engaña: es el selector por área del manejador de decorado.
- **Mapa, jugador y sombra.** El objeto del mapa es la capa 7 tipo 0x0A (`topdownMapObjHandler` 0x800EDA90). La
  sombra del jugador es la capa 3 tipo 0x3A (`topdownShadowHandler` 0x800E94AC). Los puntos de objeto son capa 2 tipo
  0x2D y los puntos de conversación capa 2 tipo 0x2E.

### Lista de carga de la sección

Lo que carga una sección desde arriba que funciona (se muestra AREA08 s0; las demás siguen el mismo patrón).
`tools/topdown.py lists` las muestra todas:

| Tipo | Puntero de ranura | Tamaño (AREA05 / AREA08 / AREA19) | Contenido |
|---|---|---|---|
| 0x10 GRPX | VRAM | B000.., CLUTnn | páginas de textura en x = 0x40.. y el bloque de CLUT de 256 x 32 en (128, 480) |
| 0x8E | `APD_DATA[14]` = scratch `0x1F800390` | 0x4008 cada uno | mapa de celdas |
| 0x8F | `APD_DATA[15]` = scratch `0x1F800394` | 0x80620 / 0x3BA20 / 0x2EA20 | el dibujo del mapa (TIM) |
| 0x62 | `WPP_DATA[2]` | 0xCA3C, los mismos bytes en todas las secciones | los fotogramas de Tombi visto desde arriba |
| 0x3A | `SPR_DATA[10]` | 0x11DC, igual en todas las secciones | la tabla de sprites de Tombi visto desde arriba |
| 0x3B..0x3E | `SPR_DATA[11..14]` | depende del área | tablas de sprites de personajes y decorado (las elige el campo `spad` de las definiciones) |
| 0x80+n | `APD_DATA[n]` | | registros de colocación (cofres, las farolas de AREA08) |
| 0xF0 | | 0x218DC | `I0NFO2.BIN` |

En AREA08/19, los bloques 8E/8F/62 son subregistros (`tipo 0xFFFFFFFF`) de un único archivo RAW_4 `.000`. En AREA05
son archivos `.000` separados. En los dos casos el cargador los guarda por tipo, así que da igual cómo estén
empaquetados.

### Mapa de celdas (tipo 0x8E): colisión y disparadores

`topdownLoadMapTiles` (0x800ECA74) lee `*(u16 **)0x1F800390`:

```
u16 w, h          tamaño del mapa en celdas de 8 x 8 px (AREA05 110 x 77, AREA08 68 x 56, AREA19 48 x 62)
u8  cell[0x4000]  se copia a TOPDOWN_MAP_TILES, por filas, w celdas por fila (w*h <= 0x4000)
```

También rellena `D_800A4668` (`{tp0, clut0, tp1, clut1, tp2, clut2, w, h, w>>2, h>>2, tim, tim+0x220}`):
`D_800A4674` = w, `D_800A4678` = w>>2 (el ancho del dibujo en bloques de 32 píxeles), `D_800A4680` = los píxeles del
TIM.

El paso de mundo a celda (`topdownCollideMapTile` 0x800EE18C, `topdownCheckTileTrigger` 0x800EE440):

```
cell = TOPDOWN_MAP_TILES[((x + 0x80) >> 3) + ((y - 0x28) >> 3) * w]   colisión (pies)
trig = TOPDOWN_MAP_TILES[((x + 0x80) >> 3) + ((y - 0x30) >> 3) * w]   prueba de disparador, 8 px más arriba
```

El dibujo usa el mismo origen: la posición del mundo (x, y) es el píxel (x + 128, y - 40) del dibujo. Los dibujos
generados confirman que las vallas y los bordes del muelle coinciden con las paredes.

Los 7 bits bajos de una celda dan su forma. El bit 7 marca una celda disparadora. En el código, `x`, `y` son `& 7`
dentro de la celda y `a`/`y` son la x/y del objeto.

| Valor | Forma | Empuje (`f & 2` = moviéndose en horizontal) |
|---|---|---|
| 0 | libre | ninguno (FORMATS.md decía "0 = no transitable": es un error, 0 es suelo libre) |
| 1 | diagonal, sólida por debajo de la línea "/" `y >= 7-x` | se le hace retroceder en el eje del movimiento |
| 2 | diagonal, sólida por encima `y <= 7-x` | igual |
| 3 | diagonal "\", sólida por encima `y <= x` | igual |
| 4 | diagonal "\", sólida por debajo `y >= x` | igual |
| 5 | pared: empuja a la izquierda `x` | `x -= x&7` |
| 6 | pared: empuja hacia arriba `y` | `y -= y&7` |
| 7 | pared: empuja a la derecha hasta la celda siguiente | `x += 7 - x&7` |
| 8 | pared: empuja hacia abajo hasta la celda siguiente | `y += 7 - y&7` |
| 9 | esquina / bloque sólido: `topdownResolveCornerTile` (0x800EE05C) lleva la sonda al borde más cercano del cuadrante y decide la celda que hay ahí | |
| bit 7 | celda disparadora: pone `o->b68 = 1` al jugador | |

**Disparadores y habitaciones.** Cuando el jugador pisa una celda disparadora, `topdownPlayerMainStep` compara su
posición con `TOPDOWN_TRIGGER_RECT_LISTS` (0x800FEC9C, una lista por área: AREA05, 11, 16, 17, 19; rectángulos del
mundo `{x0, y0, x1, y1}` terminados en 0xFF). Dentro del rectángulo i, la habitación actual pasa a ser i + 1
(`D_800A60A3`). Cada personaje, objeto y decorado compara entonces su habitación (`wb6`, el campo `room` del registro)
con `D_800A60A3`. Los de la habitación actual se dibujan (`b6b = 0x80`) y los demás se ocultan. Con
`D_800A60A3 = 0` sólo se dibujan los de la habitación 0. Así funcionan los interiores de las casas: el tejado es un
objeto de la habitación 0 y los muebles y los habitantes son objetos de la habitación n. La capa del mapa se oculta
mientras hay una habitación activa (`topdownRefreshMap` pone `b6b = 0` si `D_800A60A3 != 0`). AREA19 s2 lo muestra:
sus tres manchas de disparadores cubren las tres puertas de las casas.

### Dibujo del mapa (tipo 0x8F)

El dibujo es un TIM normal de 8 bpp: `10 00 00 00 09 00 00 00`, después el bloque de CLUT (`0x20C` bytes, x=0 y=0
w=256 h=1) y después el bloque de imagen (`len, x, y, w palabras, h`). El juego sólo usa desplazamientos fijos: la CLUT
en `+0x14` (`LoadClut` a la VRAM (0x80, 0x1FE)) y los píxeles en `+0x220`. El dibujo mide `(w>>2)*32` x `(h>>2)*32`
px: AREA05 864 x 608, AREA08 544 x 448. AREA19 mide 384 x 496, 16 filas más de las que necesita el mapa de celdas.
Su propia CLUT de 256 colores lo hace independiente del CLUTnn del área.

El dibujo se queda en la RAM y se va pasando a la VRAM. `topdownBuildVisibleTileList` (0x800ED7EC) enumera los
12 x 10 bloques de 32 x 32 px alrededor de la cámara. `topdownBuildMapStrip` / `topdownUploadMapStrip`
(0x800ECC18 / 0x800ED380) copian cinco tiras a `TOPDOWN_MAP_STRIP_BUFFER` y las suben con `LoadImage`:

| Tira | Origen | VRAM (8 bpp, en palabras de 16 bits) |
|---|---|---|
| 0 | 256 filas x 64 palabras | (0x300, 0) |
| 1 | 256 filas x 32 palabras | (0x380, 0) |
| 2..4 | 32 filas x 32 palabras cada una | AREA05: (0x40, 0x180+y); `GAME == 0x20013` (AREA19 s2): (0x40, y); las demás: (0, 0x100+y) |

`topdownDrawMapLayers` (0x800EC710) las dibuja como cinco `SPRT` (código 0x65) con `TOPDOWN_MAP_LAYER_RECTS`
(0x800FECB8), cada una con su página de textura y clut0..2. Esta disposición de la VRAM es lo que una sección nueva
debe dejar libre: las páginas propias del pueblo no pueden pisar (0x300..0x3FF, 0..255) ni la zona de las tiras 2..4.

### Personajes (capa 2 tipo 0x30) y objetos (capa 3 tipo 0x1D)

Los pueblos no usan registros APD para su gente. `spawnNpcSet0D/0E/0F/10/11` (0x800FB054...) leen dos listas de
I0NFO2. `tools/topdown.py npcs` las vuelca.

`SpawnA`, 28 bytes, personajes, terminada en el tipo 0xFF. Cada uno se convierte en un objeto `allocObjectLayer2` de
tipo 0x30 con `b0a = 0xD`, guardado en `SCRIPT_OBJECTS[i]`:

| Desplazamiento | Campo | Objeto |
|---|---|---|
| 0 | tipo (0..2) | `_pad0e[0]` |
| 2 | habitación | `wb6` (0 = en la calle, n = dentro de la casa n) |
| 4 | fotograma | `animFrame` (orientación: 0..3) |
| 6 | juego de animaciones | `wb4` (índice en la lista de animaciones de la definición al iniciar) |
| 8..E | caja de patrulla x0, y0, x1, y1 | `w74..w7a` (absoluta en los conjuntos 0D/0F/10/11, relativa a x, y en 0E) |
| 10 | caso de diálogo | `wba` (elige el `(k, dx)` en `topdownAnnActorOpenDialogue`) |
| 12 | comportamiento | `wb8`: 0 quieto, 1 patrulla vertical, 2 patrulla horizontal, 3 / 4 patrullas diagonales, 5 / 6 con guion (nada) |
| 14, 16 | x, y | posición en el mundo |
| 18 | subtipo | índice en `TOPDOWN_Ann_ACTOR_DEFS` |
| 1A | b0c | condición de aparición (conjunto 0E) / fila de la tabla de diálogos |

`SpawnB`, 16 bytes, objetos y decorado (`ObjAlloc` capa 3, tipo 0x1D): tipo, habitación, -, animación, x, y,
subtipo, b0c. El objeto 16 (índice 15) suma `D_8009CFA1` a su animación: es un objeto que depende del avance.

Definiciones de personaje, 12 bytes cada una, `{u8 box[4]; u16 w1e; s16 spad; void **anims}`
(`topdownAnnActorInit`). `box` es la caja de contacto, `w1e` la palabra de desplazamiento de CLUT/página, `spad` la
ranura de `SPR_DATA` y `anims` una tabla de guiones de animación. Los guiones usan el índice `animFrame + k`: +0..3
quieto según la orientación, +4..7 andando, +8..11 hablando (`TalkStep` usa `b69 + 7`):

| Tabla | Subtipos | `spad` | `w1e` por subtipo |
|---|---|---|---|
| `TOPDOWN_A11_ACTOR_DEFS` 0x800FEEB8 | 5 | 11 (tipo 0x3B) | 0x01, 0x0A, 0x0B, 0x10, 0x11 |
| `TOPDOWN_A16_ACTOR_DEFS` 0x800FEEF4 | 4 | 11 | 0x01, 0x01, 0x01, 0x11 (cajas 8/16 y 16/32) |
| `TOPDOWN_A17_ACTOR_DEFS` 0x800FEF64 | 5 | 11 | 0x01, 0x0A, 0x0B, 0x10, 0x08 (el subtipo 4 no tiene tabla de animaciones) |
| `TOPDOWN_A18_ACTOR_DEFS` 0x800FEFA0 | 7 | 11 | todos 0x01 |
| `TOPDOWN_A11/16/17_SCENERY_DEFS` 0x800FF054 / 0x800FEDD0 / 0x800FEE04 | 2 / 3 / 3 | 13 (tipo 0x3D) | 0x02.. 0x08 |

`tools/topdown.py defs` imprime la tabla completa, con las direcciones de las tablas de animación y cuántos guiones
tiene cada una.

### Tablas de sprites (tipo 0x3B..0x3E, SPR)

Las tablas usan el formato SPR de siempre (FORMATS.md, "SPR"): `{u16 partes, u16 desplazamiento}` por fotograma y
después partes de 16 bytes con u/v, CLUT y página. La tabla de Tombi (0x3A) y el WPP (0x62) son idénticos en todas las
secciones desde arriba y se pueden reutilizar tal cual. La tabla de personajes (0x3B, `SPR_DATA[11]`) y la de
decorado (0x3D, `SPR_DATA[13]`) son propias de cada pueblo. Los guiones de animación de I0NFO2 indexan fotogramas de
estas tablas, así que la numeración de fotogramas la fija el código aunque las tablas se hayan perdido.

### Paletas

- El dibujo trae su propia CLUT de 256 colores dentro del TIM.
- Los sprites usan el bloque de CLUT del área (`CLUTnn.GAM`, 256 x 32 palabras en (128, 480)). La palabra de CLUT de
  cada parte SPR elige una fila, y `w1e` le suma un desplazamiento.
- **Las paletas de los vecinos sobreviven.** Aquí una fila de CLUT son 16 entradas u16 (desplazamiento fila * 32 en
  `CLUTnn.GAM`; la fila r está en la VRAM en (128 + (r % 16) * 16, 480 + r / 16)). Las filas 48..54 de estos archivos
  tienen las paletas de los aldeanos de los barrios de vista lateral (44..47 y 60..63 son variantes de destello o tinte
  del mismo bloque), y esas mismas filas colorean las páginas de la vista desde arriba:
  - **AREA16/CLUT07 fila 53** (también en AREA16/CLUT06, AREA17/CLUT03, AREA17/CLUT06 y AREA18/CLUT02, con el mismo
    número de fila): «Male Stonetown Resident» de TCRF (vecino de Stonetown, el forzudo). Con esta fila, la página de 4 bpp de A07600 en (75, 202),
    50 x 52, coincide píxel a píxel con el sprite "Male Stonetown Resident" de TCRF (el 100 % de los píxeles en color de
    5 bits, sin diferencias de silueta); ninguna otra fila del archivo lo hace. **Confirmado.**
  - AREA16/CLUT07 filas 48 y 54 como la «Female Stonetown Resident» de TCRF y el resto de la gente de piedra:
    **refutado**. La fila 54 es el vecino de camisa roja del FMV (figura de A08600, ver abajo), y la «Female Stonetown
    Resident» usa la fila 21.
  - Los nombres de TCRF, y leer AREA11 como la «Lumberjack Town» de TCRF (el trabajador del casco es de la Fábrica y el
    anciano, del pueblo), son identificación nuestra.
  - **AREA11/CLUT02 filas 4, 36 y 68** (las mismas filas están en AREA17/CLUT03, AREA17/CLUT06 y AREA18/CLUT02):
    fila 4 = «Lumberjack Factory Worker» de TCRF (trabajador de la fábrica, el del casco de SYSTEM/A06100), fila 36 =
    «Male Lumberjack Town Resident» de TCRF (vecino de Lumberjack Town, el anciano), fila 68 = el mismo anciano
    «maldito» (más oscuro). **Confirmadas por los colores de TCRF**: los 14 colores de cada sprite de TCRF están en esa
    única fila (por conjunto de colores; los fotogramas de TCRF son algo distintos, así que no es píxel a píxel). Las
    filas separadas 32 forman un mismo juego de paletas de vecinos; AREA16/CLUT07 tiene la misma disposición (16/48/80,
    21/53).
  - **AREA17/CLUT03 fila 1** (= AREA17/CLUT06 filas 1 y 129, AREA18/CLUT02 fila 6): los Hombres de hierro (traje
    naranja con remaches, mangas blancas, pantalón verde, pelo negro de punta). **Confirmada: es la fila que usa el vecino
    de la Ciudad de hierro en la vista lateral del juego final**, probado con los datos de la sala: la lista 3 de AREA17
    (sección 2) carga `AREA17/B202` en la VRAM (0, 256) y `AREA17/CLUT03` en (128, 480); su tabla SPR `AREA17/D205`
    (tipo 0x33, ranura 3) dibuja los Hombres de hierro de pelo de punta de B202 (fotogramas 94-112) con la palabra CLUT
    0x7809 = (144, 480) = fila 1 (los fotogramas 113-130, los del turbante, usan 0x7849 = fila 17). No está localizado
    el manejador que los dibuja (debe dejar `b0d` en 0 para que se use la palabra de la pieza); `updateScriptActor`
    subtipo 10 de los overlays de área también pone `w08 = 0x7809`. B202 es el mismo personaje que las figuras de
    A08000, pero dibujado otra vez y más grande; ningún fotograma es idéntico píxel a píxel.
  - **Recortes del FMV de la comunidad con tiras de paleta (imágenes 138-140 del proyecto).** Cada tira es una fila
    real del disco, con los 15 colores exactos (mostrados como 5 bits << 3), así que no son paletas sacadas del FMV:
    - vecino de la Ciudad de hierro de camisa roja y pelo rizado (BUNMEI.STR, fotograma sin identificar):
      `0000 0421 7FFF 56B5 318C 4AFF 3639 2174 21BC 10D6 0010 01B6 0174 0132 6358 4AF0` = **fila 54** de AREA16/CLUT06/07 y AREA18/CLUT02,
      **fila 49** de AREA17/CLUT03/06 (todos los discos; fila = bloque de 16 entradas del archivo);
    - vecino vestido de morado (BUNMEI.STR, fotograma sin identificar):
      `0000 2A17 7FFF 4210 318C 2108 52DD 3A59 6593 5571 454F 011B 00B3 006B 0218 0173` = **fila 38** de AREA16/CLUT06/07 y AREA18/CLUT02, **fila 33** de AREA17/CLUT03/06;
    - hombre calvo y moreno dentro de un edificio: `0000 0421 4210 294A 6B5A 1E39 012F 1193 56B5 4A52 2D6B 5EF7 35AD 6B5A 0000 7FFF`
      = fila 255 (antes "fila 15": hueco 15 de la línea 495 de VRAM) de los CLUT de AREA00, 01, 02 y otras áreas (una fila común a todas las áreas, no de Civilización).
    - figura de sombrero azul y capa roja vista de espaldas en el césped de la Ciudad de hierro (imagen 143; BUNMEI.STR,
      fotograma sin identificar).
    Estos colores encajan con la **página de figuras de AREA18, `SYSTEM/A08600`** (4 bpp en sus primeras 64 palabras; probada
    con todas las filas de vecinos de AREA18/CLUT02, a ojo; el FMV es 3D, así que no es una prueba por silueta):
    - 139 = la figura grande de pelo rizado de A08600 (filas y 144-191) con la fila 54: pelo rojo, ropa roja y salmón;
    - 140 = la misma figura con la fila 38: pelo naranja, vestido morado;
    - 143 = la figura de sombrero ancho y capa (y 96-141, que tiene vistas de espaldas); la fila más cercana es la 22 de
      AREA18/CLUT02 (sombrero gris azulado, ribetes naranja), pero ninguna fila da una capa roja: queda sin resolver.
    Estos vecinos sólo están dibujados en A08600 (la página de AREA18), no en las páginas que sobreviven de la Ciudad de hierro ni
    de la de piedra. La fila 54 de AREA16/CLUT07, la antigua suposición para la «Female Stonetown Resident», es por tanto la
    paleta del vecino de camisa roja: la suposición era errónea (ese subtipo usa la fila 21).
- Qué fila usa cada vecino de la vista desde arriba no está en el código que sobrevive: la CLUT de un vecino viene de las
  partes de su tabla SPR (las inicializaciones de personaje ponen `b0d` a 0, así que no se usa `w08`), y esas tablas se
  perdieron. `tools/civ_sheets.py` indica con qué archivo y fila dibuja cada grupo de vecinos (`sub_pal`, `PAL_NOTE`).

### Las páginas de textura que sobreviven (listas SYS 61 / 76 / 80 / 86)

`inventoryScreenHandler` (MAIN0) cierra la subpantalla con `queueSystemLoadList(D_80077A8C[area][sección])`, que
recarga la VRAM que pisó el menú. En una sección que funciona, esa lista es una copia exacta de la primera página (o
páginas) de textura de la sección, recortada a x = 0x40..0x200. Lo comprobamos byte a byte: `SYSTEM/A03900` es igual
a las columnas 0..447 de `AREA05/B000`, `SYSTEM/A04700` es igual a `AREA08/B000` y `SYSTEM/A09100` es igual a las
columnas 0..127 de `AREA19/B200`.

Las secciones recortadas conservan sus entradas, así que sus primeras páginas sobrevivieron:

| Sección | Lista SYS | Archivos | VRAM |
|---|---|---|---|
| AREA11 s0 | 61 | `SYSTEM/A06100` | 448 x 256 palabras en (0x40, 0) |
| AREA16 s0 | 76 | `SYSTEM/A07600`, `A07601` | 320 x 256 en (0x40, 0) y 128 x 256 en (0x180, 0) |
| AREA17 s0 | 80 | `SYSTEM/A08000` | 448 x 256 en (0x40, 0) |
| AREA18 s0 | 86 | `SYSTEM/A08600`, `A08601` | 320 x 256 en (0x40, 0) y 128 x 256 en (0x180, 0) |

`A06100` tiene en sus primeras 64 palabras una página de 4 bpp con sprites de aldeanos: un trabajador con gorra en
cuatro orientaciones, fotogramas de andar, un martillo y una postura sentada, que encaja con el constructor de balsas.
El resto son recortes de decorado de 8 bpp: una plaza octogonal, paredes y tejados de casas, vallas, árboles. Es decir,
son los sprites de personajes y de habitaciones/tejados a los que apuntaban las tablas SPR. El **dibujo del mapa no
está entre ellos**, porque vive en la RAM y se pasa a (0x300..) cuando hace falta. Sus paletas son las filas de CLUT de
la sección Paletas.

## Paso 2: qué esperaba cada sección recortada

`tools/topdown.py plan AREA OUT.png` dibuja el conjunto sobre una cuadrícula de 32 px. Muestra los personajes (rojo en
la calle, magenta dentro), las cajas de patrulla (verde), los objetos (amarillo) y los rectángulos de las casas
(azul). Todos los registros de interior caen dentro de un rectángulo de casa de la misma área, lo que respalda la
decodificación.

Los bytes de bandera de abajo son bytes de `GAME`/`SCRIPT_FLAGS` (la tabla de eventos es `SCRIPT_FLAGS` = 0x8009CDA4).
`setEventStarted` es 0x8005A8A8. Un manejador de conversación que pone `o->wac = E` y vuelve por `state 2` inicia el
evento E al cerrarse el cuadro.

Los generadores ajustan tres variables de avance:

- `D_8009CFA5` (conjuntos 0D/0F) vale 0 al principio, 1 en cuanto `D_8009CE0A != 0` y 2 en cuanto `D_8009CDF1 == 0xFF`.
- `D_8009CF60` (conjunto 0E) vale 0..3, según `D_8009CDF1 != 0`, `D_8009CE0A` y `D_8009CDF1 == 0xFF`.
- `D_8009CFA1` elige la animación del objeto 15.

### AREA11 s0: el pueblo de la gente de madera (conjunto 0D)

- **Tamaño y salidas** (`topdownSectionStepC`, `D_80079A48[11][0]` = 0x800798B4):
  - salida 0 con x > 0x22B, y 0x169..0x1CC, va a AREA18 s1 punto 1;
  - salida 1 con y < 0x4F, x 0xC7..0x107, va a AREA11 s1 punto 0 (la Fábrica en vista lateral).
  - Los personajes ocupan x -21..496 e y 204..581, así que el pueblo medía unos 640 x 560 px (80 x 70 celdas).
- **Casas**: tres, habitaciones 1..3 en (-10,191)-(154,283), (-60,278)-(51,379) y (361,419)-(500,520).
- **Personajes** (5 subtipos, `SPR_DATA[11]`):
  - habitación 1: dos aldeanos.
  - habitación 2: un aldeano.
  - habitación 3: un aldeano (subtipo 4, `dlg 11`). `dlg 11` no tiene caso de conversación, así que `OpenDialogue`
    devuelve 0 y por este camino no dice nada. Probablemente es el constructor de balsas de k3 dx10-14, al que no
    llega ninguna llamada de I0NFO2.
  - en la calle: ocho aldeanos, seis de ellos patrullando (dos en vertical, tres en horizontal, uno en diagonal).
- **Objetos**: 14. Tres objetos de tipo 2 están en las habitaciones 1..3, en los rectángulos de las casas. Los otros
  11 son objetos de la calle con animaciones 3..15.
- **Diálogo** (`topdownA11ActorOpenDialogue` 0x800F1C88, por `dlg`):
  - 0 → (3,0)
  - 2/5 → (4,2) "esta máquina es de hierro..."
  - 3 → (4,0) la choza de la adivina
  - 4 → (4,1) "la madera es lo mejor"
  - 6 → (4,3) el laberinto bajo el suelo
  - 7 → (4,4) "a esa choza sólo se llega por debajo del suelo"
  - 8 → (4,5 / 10 / 16) según el avance: el engranaje, "iré a por un coco", "el Pueblo Karakuri se hundió"
  - 9 → (4,6) la entrada del taller
  - 10 → (4,7) el cartel viejo y peligroso, o (4,11) "siempre estás lleno de energía"
  - 1 pone `D_8009CFA9` (1, o 2 si `D_8009CDF3`)
  - `topdownA11ActorDialogueBank2` (0x800F1BAC) muestra (2,4), luego (2,5) e inicia el **evento 0x4F**.
  - Son MFNT110 #249-#265. PAL los sustituyó por "···??". Las frases japonesas están en AREA11 A002 bloque 0
    #220-#249; el constructor de balsas (#237-#241 "¿quieres una balsa? tráeme madera de debajo del Árbol Sabio...
    pum, clan... ¡ay!") es k3 dx10-14.

### La adivina/adivino de la casa H3 (AREA11 subtipo 4)

- **Dibujo encontrado.** El hombre del turbante (hoja de la comunidad) es `AREA02/B300.GAM` palabras 64..127 (página 4 bpp
  en VRAM (192, 0) de AREA02 s3, los túneles; también la copia `SYSTEM/A02100`), píxel a píxel desde u 1. La versión
  final lo dibuja en vista cenital con `SPR_DATA[3]` (`AREA02/D305.GAM`, tabla en 0x143C), fotogramas 10..46, para
  `TUNNEL_ACTOR_DEFS[1]` (w1e 3), con el CLUT 0x780D = **AREA02/CLUT04 fila 5 = el adivino azul**. **La fila 21 del
  mismo fichero es la verde**: sus 15 colores son exactamente los de la hoja; la fila 37 es la sombra. El adivino rojo
  no usa esta hoja con esta piel (ninguna fila del disco la tiene con ropa roja).
- **Vecino.** AREA11 subtipo 4 (w1e 0x11 = página (64, 256), perdida en AREA11) es el vecino 12, el único de H3, junto al
  vecino 4 («en esta choza vive una adivina»). Sus guiones (fotogramas 204..240) tienen exactamente la estructura y los
  tiempos de los 10..46 del adivino de los túneles (f = f_final + 194): es el Adivino Verde, no el constructor de
  balsas. Su `dlg 11` no tiene caso en PAL y nada inicia el evento 89.
- **Port (`--civ-town 11`, sólo port, `port_civ_teller`).** Copia esa página a (64, 256), las filas 21 / 37 a
  (0, 510) / (0, 511) y rehace su tabla con las piezas reales de D305; la sustitución por el aldeano de AREA08 ya no
  toca el subtipo 4.

### AREA16 s0: el pueblo de la gente de piedra (conjunto 0E)

- **Salidas** (`topdownSectionStepD`):
  - salida 0 con x < -0x74, y 0x208..0x211, va a AREA18 s1 punto 3;
  - salida 1 con x 0x6B..0x72, y 0x13F..0x140 (una puerta), va a **AREA11 s2** punto 0;
  - salida 2 con x > 0x93, y < 0x4A, va a AREA16 s1 punto 0 (también recortada).
- **Casas**: cuatro (habitaciones 1..4).
- **Personajes**: 13. `b0c` decide si aparecen: b0c 8 sólo tras el primer avance, b0c 11/12 sólo antes, b0c 15 sólo
  al final.
- **Diálogo** (`topdownA16ActorOpenDialogue` 0x800F2E84):
  - Los casos 0..7, 13 y 14 muestran `(3, TOPDOWN_A16_ACTOR_DIALOGUE_IDS[b0c][D_8009CF60])` (0x800FEF24,
    `u8[16][4]`). Filas usadas: b0c 1 → 0, 2 → 1, 3 → 2 (el ascensor que no se mueve), 4 → 3/7/10/14,
    5 → 4/8/11/15, 6 → 5/5/12/16, 7 → 6/6/6/17, 8 → 0/9/13/13. Entre todas cubren MFNT160 #230-#247: "¡aúpa!", el
    ascensor, "los otros pueblos", el engranaje, el generador, "hacer las paces con todos", "vencer al Cerdiablo".
  - Caso 8 → (2,2) o (2,0).
  - Casos 12/15 → (2,0x15) = #229 (Mizuno). El caso 12 además inicia el **evento 0x67** si `D_8009CE0B == 0`.
  - Los casos 9/10 ponen `D_8009CFA7`.
- **Escenas**: `npcSet0ESceneStep1` inicia el **evento 0x61**. `npcSet0ESceneStep3` muestra (2,1)/(2,2) e inicia el
  **evento 0x3E**.
- **Objetos**: la lista de objetos pasa a `NPC_SET_0E_EXTRA_RECORDS_ALT` cuando `D_8009CFA8 == 4`. Esa lista
  alternativa sólo cambia la animación del objeto 5 (12 en vez de 10), seguramente el reloj arreglado.
  `topdownA16SceneryUpdate` mueve el objeto `wb8 == 5` a (-4/-3, 0x193) con la animación 0xB/0xC cuando
  `D_8009CFA8` pasa a 2/3.

### AREA17 s0: el pueblo de los Hombres de hierro (conjunto 0F)

- **Salidas** (`topdownSectionStepE`):
  - salida 0 con y > 0x23A, x 0xA7..0x117, va a AREA18 s1 punto 2;
  - salida 1 con y < 0x50, x 0xB0..0x10F, va a AREA17 s1 punto 0.
- **Casas**: cinco (habitaciones 1..5).
- **Personajes**: 16, siete de ellos patrullando. **Objetos**: 35, entre ellos dos filas de ocho objetos iguales
  (animación 23) en y = 302 y en y = 84. Seguramente es el muro del Cerdiablo, o los raíles del ascensor.
- **Diálogo** (`topdownA17ActorOpenDialogue` 0x800F4110):
  - Caso 0 → (2,0xA); caso 1 → (2,7).
  - Caso 2 → `topdownA17ActorDialogueBank2`, el herrero: (2,5) "¿necesitas algo?", (2,6) "¿un engranaje de hierro?"
    y el **evento 0x63**, (2,9), (2,10).
  - Caso 3 → `topdownA17ActorDialogueBank3`, el fabricante de bombas: (3,0) "no he visto tu cara", (3,2), (3,3) y el
    **evento 0x69**, (3,5) "primero necesito electricidad", (3,8) "ahora la mezcla de la bomba", (3,0xB) "¿más
    bombas?".
  - Casos 5..15 → (5,0..19) según el avance: los aldeanos de hierro, el ascensor, Gantetsu, el Castillo de hierro,
    "madera, piedra y hierro: lo importante es el equilibrio".
- **Escenas**:
  - `npcSet0FSceneStep1` inicia el **evento 0x4C**.
  - `npcSet0FSceneStep2` muestra (2,6) e inicia el **evento 0x63**.
  - `npcSet0FSceneStep4` muestra (2,7) "buen material... espera" y (2,8) "toma", y después da el **objeto 0x3E** (el
    engranaje de hierro) con `func_80026C50(0x3E, 1, 1)`.
  - `npcSet0FCheckTriggerSpots` y `topdownA17SceneryUpdate` mueven dos plataformas (objetos `wb8` 0xD/0xE entre
    `TOPDOWN_A17_SCENERY_LIMITS`, que llevan al jugador cuando está el bit 0x40). Son los ascensores.
- **Texto**: MFNT170 #213-#218, #257-#268 y #270-#289. En PAL son "···??"; el texto japonés los tiene.

### AREA18 s0: el pueblo reunificado (conjunto 0x10), nunca terminado

- El conjunto tiene 10 personajes sin patrulla, sin objetos y sólo con los casos de diálogo 0/1.
- `topdownA18ActorHandler` (0x800F548C) sólo cambia la animación (`wb4 = (wb4+1) & 7`) mientras se mantiene pulsado el
  bit 0x1000 del mando. Es un visor de sprites, sin nada de conversación.
- La entrada del controlador de sección `D_80079AD0[25]` (`dispatchSectionClose`) llama a 0x80116234 para la sección
  0. Eso es espacio de X0nn, no de I0NFO2 (en X018 ni siquiera es el principio de una función). Una AREA18 s0 desde
  arriba saltaría al overlay equivocado.
- Su tabla de salidas (0x800799D0) apunta a AREA01 s5 y AREA03 s1, que son de relleno.
- Su texto japonés (MFNT180 k2 dx0-12 = #209-#221, "hemos vuelto a ser un solo país gracias a Tombi") no lo llama
  nadie.
- Este pueblo se planeó pero nunca pasó de datos de prueba.

### Cómo se habría entrado en la sección 0 (y por qué no se puede)

Los registros de salida (`D_80079A48[área][sección]`, 8 bytes `{s16 x, s16 y, u8 tipo, u8 área, u8 sección, u8
punto}`, los imprime `tools/topdown.py warps`) todavía describen las rutas planeadas. La sección 0 de cada pueblo iba
a ser la puerta entre el centro y las secciones de vista lateral del pueblo:

- AREA11 s0 sale a 18:1 y 11:1. AREA16 s0 sale a 18:1, 11:2 y 16:1. AREA17 s0 sale a 18:1 y 17:1.
- **Ningún registro de salida de ninguna área apunta a 11:0, 16:0..3, 17:0 ni 18:0.** La búsqueda cubrió las tablas
  de MAIN0 de las áreas 2/5/8/11/16..19 y las de los overlays X000/X001/X003/X004/X006/X009/X010/X013/X014.
- El centro del juego final se los salta: las salidas 0/1/2 de AREA18 s1 van a **AREA11 s1** punto 0, **AREA17 s1**
  punto 0 y **AREA16 s4** punto 0.
- AREA16 s4..s6 son copias de las s1..s3 recortadas, con las mismas tablas de salida (0x80079908.. compartidas).
  AREA17 s6..s10 comparten igualmente las tablas de s1..s5.
- AREA10 s0/s4 llevan a 18:1 punto 4, la salida 2 de AREA00 s0 lleva a 18:2, y AREA14/15 llevan a 16:5.

Así que a los pueblos recortados no se puede llegar por tres motivos: ninguna puerta lleva a ellos, las listas de
carga están vacías y (en AREA18) no hay controlador. Recuperar uno supone:

1. apuntar una salida del juego hacia él, por ejemplo la salida 0 de AREA18 s1 → 11:0 punto 0, cuya salida 1 ya sigue
   a 11:1;
2. rellenar la lista de carga;
3. dejar todo lo demás como está.

Los puntos de entrada del pueblo son los valores de `GAME_SELECTED_SPAWN_POINT` que usan sus propios registros de
salida al volver (salida 0 de AREA11 s1 → 18:1 punto 1, etc.).

### Colores del decorado, el FMV e imágenes de referencia de la comunidad

- **Los recortes de decorado de 8 bpp no tienen paleta que sobreviva.** Son las páginas con interiores de casas, muros,
  puertas, vallas y el molino. Comparamos los renders en color de la comunidad de SYSTEM/A06100, A08000 y A08600
  (índice → color, con coincidencia exacta de página) con cada ventana de 256 colores, en pasos de 16 entradas, de cada
  archivo de los discos PAL y USA. Nada se acercó: el mejor error medio pasa de 60 por color. Las CLUT reales del
  decorado se perdieron con los datos de las secciones.
- **La única referencia de color es el FMV.** `MOVIE/BUNMEI.STR` (文明, «civilización»), idéntico en PAL y USA, es el
  vuelo sobre la Civilización purificada. Muestra Lumberjack Town con el molino, el trabajador del casco y el anciano,
  Stonetown con sus tejados de tejas verdes, e Iron Town con la puerta redonda de hierro y el cartel «Iron the No.1».
  Le sigue `BUNMEI2.STR`, con la sala de máquinas y los engranajes.
  `python3 tools/str.py frames game/disc/Tombi.bin BUNMEI SALIDA 12 25` saca fotogramas (nunca se suben).
- **Solo se reproduce BUNMEI2.** El id de película es `MOVIE_ID` (0x1F8001CD); `moviePlayerTask` reproduce el archivo
  `MOVIE_FILE_IDS[D_80077F3C[id]]`, así que el id 11 es BUNMEI.STR (archivo 323) y el 12 es BUNMEI2.STR (324). Nada
  guarda 11: en PAL se escriben 0-9, 0x14 y 0x15, y la tabla de fin de sección de x014 `D_80125684` tiene 10 y 13-19.
  BUNMEI2 es el id 12, que solo pone `waterfallElderHintScene` (x017, `sb` en 0x80119204), la escena del anciano de
  10.000 años que termina con el evento 0x49. Resto: esa escena llama antes a `cdSeekStream(0xB)` (0x801191A0), una
  búsqueda previa de BUNMEI, y luego reproduce el 12. JP (SLPS-01144 x17) también usa 12. BUNMEI.STR está en el disco
  pero no se usa.
- **Imágenes de referencia de la comunidad.** Un colaborador de TCRF coloreó las páginas de recarga y montó vistas del
  decorado (siete imágenes, más dos «capturas» de Iron Town y Lumberjack Town). Son reconstrucciones hechas con
  fotogramas de BUNMEI.STR y una CLUT deducida a mano, no capturas previas al lanzamiento ni paletas del juego. Las usamos
  sólo como referencia visual y no están en el repositorio.
- **Los archivos VRAM de la comunidad son las páginas PAL.** Los siete archivos `.000` (nombres USA) son, byte a byte:

| Nombre USA | Nombre PAL | Contenido |
|---|---|---|
| SYSTEM/A00147 | SYSTEM/A06100 (lista SYS 61) | página de Lumberjack Town |
| SYSTEM/A00153 | SYSTEM/A07600 (lista SYS 76) | página de Stonetown |
| SYSTEM/A00154 | SYSTEM/A07601 (lista SYS 76) | página de Stonetown, segunda parte |
| SYSTEM/A00157 | SYSTEM/A07702 (lista SYS 77) | página de 128 x 256 en (0x180, 0) |
| SYSTEM/A00163 | SYSTEM/A08000 (lista SYS 80) | página de Iron Town |
| SYSTEM/A00175 | SYSTEM/A08600 (lista SYS 86) | página de AREA18 (interiores de los tres pueblos) |
| SYSTEM/A00176 | SYSTEM/A08601 (lista SYS 86) | página de AREA18, segunda parte |

  Son las mismas páginas de recarga SYS con la numeración de archivos del disco USA, no datos sólo de USA.

### Qué dibujo es cada subtipo (pruebas)

Los guiones de animación sobreviven en I0NFO2 (`TOPDOWN_Ann_ACTOR_DEFS[sub].anims`, pasos de 8 bytes `{u16 fotograma, -, -, u16
control}`). El código de quieto/andar/hablar lee los guiones `animFrame + 0/4/8` de la tabla 0. Por subtipo:

| Pueblo | Sub | w1e (página) | Fotogramas de sus guiones | Dibujo | Estado |
|---|---|---|---|---|---|
| AREA11 | 0 | 0x01 | 0..29 (quieto, andar, hablar), más 30..50 en los guiones extra 12..23 | el trabajador del casco, Lumberjack Factory Worker (el anciano está en la misma página, fotogramas 30..50) | página probada; figura coherente |
| AREA11 | 1, 2, 3, 4 | 0x0A, 0x0B, 0x10, 0x11 | 36..89, 75..128, 108..196, 204..240 | en páginas que no rellena ningún archivo que sobreviva | perdido |
| AREA16 | 0 | 0x01 | 30..80 | primer grupo de figuras de A07600 (Female Stonetown Resident) | página probada; figura coherente |
| AREA16 | 1 | 0x01 | 60..110 | segundo grupo (el chico) | página probada; figura coherente |
| AREA16 | 2 | 0x01 | 90..129 | tercer grupo (el forzudo, Male Stonetown Resident) | página probada; figura coherente; paleta píxel a píxel |
| AREA16 | 3 | 0x11 | 0..29 | página en VRAM y 256 | perdido |
| AREA17 | 0 | 0x01 | 0..55 | el vecino de pelo de punta de A08000 | página probada; figura coherente |
| AREA17 | 1, 2, 3, 4 | 0x0A, 0x0B, 0x10, 0x08 | 41..96, 82..135, 115..147, ninguno | páginas perdidas | perdido |

- **Página (probado).** Una parte se dibuja de la página `part.tpage + w1e`. En las tablas de vista desde arriba del juego
  (AREA05/08/19) las partes de personaje usan la página relativa 0. Las páginas de recarga SYS que sobreviven empiezan
  en VRAM x 0x40, que es la página 1, y la zona de sprites de 4 bpp son sus primeras 64 palabras. Así que sólo los
  subtipos con w1e 1 pueden dibujarse de ellas; la página de los demás no la rellena ningún archivo que sobreviva.
- **Figura (coherente).** En AREA05/08 los fotogramas de SPR_DATA[11] están colocados en la página por orden de número,
  fila a fila, y los fotogramas en espejo reutilizan los texeles en u - 1. Los subtipos 0/1/2 de AREA16 empiezan en los
  fotogramas 30/60/90, en el mismo orden que los tres grupos de figuras de A07600 desde arriba: mujeres con capucha, el
  chico, el forzudo. Los colores del forzudo coinciden píxel a píxel con TCRF. El subtipo 0 de AREA11 usa primero los
  fotogramas 0..29, y el trabajador del casco es el primer grupo de A06100.
- **Lo que no se muestra.** El fotograma de orientación dibujado en las láminas es elección nuestra (el campo +4 del
  registro elige el de verdad). Los vecinos cuya página se perdió se dibujan como cápsulas grises con interrogación.

### Música de las secciones recortadas

En resumen: no queda ningún tema propio de los pueblos, y tampoco falta ninguno. Cada sección 0 recortada todavía
tiene su entrada de música, y apunta a la misma pista que el resto de su área. `tools/music_refs.py` imprime la tabla
entera a partir de tu MAIN0.

- **Cómo se elige la música (probado, código de MAIN0).** La música no está en las listas `SYS/LDnARxx`. Esas listas
  solo llevan los subtrozos pequeños `0xB2`/`0xB3` dentro del paquete del área. `loadAreaSectionResources`
  (0x8001D1BC) lee un número de juego de sonido de `PTR_DAT_80077c4c[área + AREA_VARIANT][sección]`. Después
  `loadSoundSet` (0x8004FB68) ejecuta `SOUND_SET_LOAD_LISTS[juego]` (0x8007D1B4). Son 0x34 listas de carga guardadas
  dentro de MAIN0: un WVD (tipo 0x93) más un SEQ (tipo 0xB0). `openAreaBgm` (0x8001EC80) toca el SEQ que se haya
  cargado. Solo cambia el volumen (por área, con ajustes por sección en AREA06/09) y la reverberación. Ningún código
  elige la música por sección.
- **I0NFO2 no toca la música (probado).** Ninguno de los manejadores `topdown*` pone una BGM. La única llamada de
  control de sonido es `stopBgm(1)` en `topdownPlayerDropExitStep` (0x800EAAA0), que hace un fundido de salida cuando
  una salida lleva a otra área. Las secciones cenitales que funcionan usan la tabla normal del área: AREA05 s0 juego
  0x0F `S_AR050` (igual que s1..s3), AREA08 s0 juego 0x13 `S_AR080` (igual que s1..s3), AREA19 s0 juego 0x2B `S_AR192`.
- **Qué sonaría al entrar en una sección recortada (probado por la tabla, sin ejecutarlo):**

| Sección | Fila (byte) | Juego | Ficheros | Compartido con |
|---|---|---|---|---|
| AREA11 s0 | 0x80077C10 | 0x1B | `SND0110.WVD` + `S_AR110.SEQ` | AREA11 s1..s3 (Fábrica) |
| AREA16 s0 | 0x80077C34 | 0x27 | `SND0160.WVD` + `S_AR160.SEQ` | AREA16 s1..s7 (Ciudad de Piedra) |
| AREA17 s0 | 0x80077C3C | 0x28 | `SND0170.WVD` + `S_AR170.SEQ` | AREA17 s1..s7 (Ciudad de hierro) |
| AREA18 s0 | 0x80077C44 | 0x29 | `SND0181.WVD` + `S_AR181.SEQ` | AREA18 s1 (centro); s2/s3 usan 0x2A `S_AR1802` |

- **No hay pistas sin usar (probado).** Se llega a los 44 juegos de sonido desde la tabla de áreas, desde las tablas
  de bancos de efectos extra en 0x80077C9C/CA4/CB0 o desde el código de arranque y del título. Cada
  `SOUND/*.SEQ`/`*.WVD` pertenece a un juego al que se llega. No hay audio XA en el disco. El único audio en streaming
  está dentro de `MOVIE/*.STR`.
- **Nombres (deducción).** Los ficheros llevan el nombre de un área y de la primera sección que los usa (`S_AR003`,
  `S_AR023`, `S_AR147`...). Así, `S_AR110`/`S_AR160`/`S_AR170` llevan el nombre de la sección 0 de los pueblos
  recortados. Pero la pista por defecto de todas las áreas sigue la misma regla (`S_AR050`, `S_AR080`), así que esto no
  demuestra un tema exclusivo de los pueblos. AREA18 es la excepción: su primera pista es `S_AR181`, no `S_AR180`. Puede
  que se pensara en un `S_AR180` para el pueblo reunido. Ninguna tabla, fichero ni cadena lo nombra.
- **Disco JP (SLPS-01144, `game_jp/disc/tomba_jp.bin`).** Los bancos están en las carpetas de área:
  `AREA11/SND0110.WVD`, `AREA16/SND0160`, `AREA17/SND0170`, `AREA18/SND0181` y `SND0182`. Sus tamaños coinciden byte a
  byte con los PAL, y no hay `SND0180`. No hemos rastreado la tabla de juegos JP. Los ejecutables JP y beta 980203 no
  tienen una copia byte a byte de las filas PAL.

## Los pueblos recortados uno a uno (láminas)

`tools/civ_sheets.py` dibuja tres láminas explicativas por pueblo, en español o en inglés, a partir de tu propio
disco extraído (`game/disc/fs` o `TOMBI_FS`). Las láminas contienen arte del disco (el dibujo de AREA08 y los sprites
de vecinos que sobreviven), así que nunca se suben al repositorio: genéralas en tu equipo.

```
python3 tools/civ_sheets.py                                   # todos los pueblos, los dos idiomas, todas las láminas -> build/civ/
python3 tools/civ_sheets.py --area 16 --lang es --sheet esquema --out SALIDA   # una lámina
```

| Lámina | Archivo (ES / EN) | Qué muestra |
|---|---|---|
| esquema | `civ_esquema_aNN.png` / `civ_esquema_aNN_en.png` | sólo datos. El panel A es el dibujo real de AREA08 s0 con las marcas que pone su propio código. El panel B es el pueblo recortado a la misma escala sobre gris, con los sprites reales de los vecinos en paletas reales del juego, las casas, las patrullas, los objetos y las salidas en las posiciones del código. Debajo: qué significa cada marca, adónde lleva cada salida, qué dice cada vecino y de dónde sale cada dato |
| explicado | `civ_explicado_aNN.png` / `civ_explicado_aNN_en.png` | un plano con casas dibujadas, sprites, flechas de salida y una leyenda al lado, junto a la referencia de AREA08 |
| perspectiva | `civ_perspectiva_aNN.png` / `civ_perspective_aNN_en.png` | los mismos datos en perspectiva; lo único inventado es la altura de las casas |

Marcas: las casas son los rectángulos de `TOPDOWN_TRIGGER_RECT_LISTS`, donde al entrar Tombi desaparece el tejado y se
ve el interior; los vecinos rojos están en la calle y los morados dentro de una casa; el verde es la zona por la que
camina un vecino; los cuadraditos amarillos son objetos. Las frases de los vecinos son resúmenes cortos del texto
japonés (en PAL esos huecos tienen "···??"). Qué subtipos usan la página que sobrevive, y qué figura es cada uno, está en «Qué dibujo es cada subtipo»; los vecinos de páginas perdidas son cápsulas grises. El tamaño del dibujo de cada pueblo es una estimación a partir de las posiciones (AREA11 736 x 608,
AREA16 544 x 608, AREA17 832 x 704 px con márgenes).

### AREA11 s0, el pueblo de la gente de madera

- **Casas** (`TOPDOWN_TRIGGER_RECT_LISTS` 0x800FEC9C, mundo x0,y0-x1,y1): C1 (-10,191)-(154,283); C2 (-60,278)-(51,379); C3 (361,419)-(500,520).
- **Vecinos** (NPC_SET_0D, lista en I0NFO2 0x80100080; frases vía topdownA11ActorOpenDialogue 0x800F1C88 → AREA11/MFNT110S.WFM #249-#265): 12, con la misma numeración que las láminas.

| # | Dónde (x, y) | Casa | Quién / qué dice (resumen de la frase japonesa) |
|---|---|---|---|
| 1 | (88, 240) | C1 | «Lumberjack Factory Worker» (trabajador de la fábrica) — Casa 1: «...Ahora puedo hacer algo muy interesante...» (texto PAL #227) |
| 2 | (12, 256) | C1 | «Lumberjack Factory Worker» (trabajador de la fábrica) — Casa 1: no habla, activa una bandera |
| 3 | (16, 352) | C2 | «Lumberjack Factory Worker» (trabajador de la fábrica) — Casa 2: «No funciona, pero esta máquina es de hierro... me vendría bien» |
| 4 | (400, 520) | calle | «Lumberjack Factory Worker» (trabajador de la fábrica) — Patrulla diagonal: «En esta choza vive una adivina, pero pone una condición» |
| 5 | (300, 470) | calle | «Lumberjack Factory Worker» (trabajador de la fábrica) — Patrulla horizontal: «La madera es lo mejor; la piedra y el hierro son fríos» |
| 6 | (170, 528) | calle | «Lumberjack Factory Worker» (trabajador de la fábrica) — Patrulla horizontal: «No funciona, pero esta máquina es de hierro...» |
| 7 | (172, 424) | calle | «Male Lumberjack Town Resident» (vecino de Lumberjack Town) — Vecino: «Bajo el suelo hay un laberinto; no te metas sin cuidado» |
| 8 | (50, 320) | calle | «Lumberjack Factory Worker» (trabajador de la fábrica) — Patrulla vertical: «A esa choza sólo se llega por debajo del suelo» |
| 9 | (62, 294) | calle | «Lumberjack Factory Worker» (trabajador de la fábrica) — Patrulla horizontal: «¿Un engranaje? De madera se rompería» → «Tengo sed, iré a por un coco» → «El Pueblo Karakuri se hundió» |
| 10 | (280, 236) | calle | «Lumberjack Factory Worker» (trabajador de la fábrica) — Patrulla vertical: «Al taller se entra por arriba; ve a echar un vistazo» |
| 11 | (496, 344) | calle | «Male Lumberjack Town Resident» (vecino de Lumberjack Town) — Vecino: «Este cartel es viejo y peligroso» → «Siempre tan lleno de energía» |
| 12 | (432, 448) | C3 | «Lumberjack Factory Worker» (trabajador de la fábrica) — Casa 3, sentado: probablemente el constructor de balsas («¿Quieres una balsa? Tráeme madera del Árbol Sabio»); el código no le da diálogo |

- **Salidas** (`topdownSectionStepC`, I0NFO2 0x800E8D98; destinos MAIN0 0x80079A48): Ciudad de civilización (AREA18); Fábrica (AREA11 s1).
- **Objetos**: 14 (I0NFO2 0x800FFE24); el código no dice qué es cada uno.
- **Muñecos**: `SYSTEM/A06100` (lista de recarga SYS 61), dibujado sólo el subtipo 0 («Lumberjack Factory Worker», AREA11/CLUT02 fila 4, confirmada por los colores de TCRF); los subtipos 1-4 están en páginas perdidas.
- **Falta**: dibujo de fondo, mapa de celdas (colisión), tablas de sprites.
- **Regenerar**: `python3 tools/civ_sheets.py --area 11 --lang all --out build/civ`

### AREA16 s0, el pueblo de la gente de piedra

- **Casas** (`TOPDOWN_TRIGGER_RECT_LISTS` 0x800FEC9C, mundo x0,y0-x1,y1): C1 (78,351)-(202,486); C2 (160,264)-(235,343); C3 (219,176)-(299,258); C4 (-25,146)-(51,244).
- **Vecinos** (NPC_SET_0E, lista en I0NFO2 0x80100364; frases vía topdownA16ActorOpenDialogue 0x800F2E84 → AREA16/MFNT160S.WFM #230-#247): 13, con la misma numeración que las láminas.

| # | Dónde (x, y) | Casa | Quién / qué dice (resumen de la frase japonesa) |
|---|---|---|---|
| 1 | (86, 460) | calle | Vecino que levanta algo: «¡Aúpa... YA!» |
| 2 | (144, 332) | calle | Vecino que levanta algo: «¡Hop... AAAH!» |
| 3 | (244, 427) | calle | «Female Stonetown Resident» (vecina de Stonetown) — Vecino junto al ascensor: «Esto pegado a la pared es un ascensor... pero no se mueve» |
| 4 | (176, 384) | C1 | «Male Stonetown Resident» (vecino de Stonetown) — Casa 1: «¿No puedes entrar en los otros pueblos? Ni lo sé ni me importa» → luego habla del engranaje y del generador |
| 5 | (-72, 362) | calle | «Female Stonetown Resident» (vecina de Stonetown) — Patrulla vertical: «La gente de los otros pueblos...» → «¿Un engranaje que no se rompa? Si fuera de hierro...» |
| 6 | (0, 245) | calle | Patrulla diagonal: «¿Primera vez en este pueblo?» → «Hace mucho intenté meterme en el río...» |
| 7 | (86, 315) | calle | Vecino: «¿Debería entrar...?» → «Hermano, vencerás al Cerdiablo que hechizó esta tierra, ¿verdad?» |
| 8 | (239, 106) | calle | Patrulla diagonal (sale tras el 1er avance): «Abrí en secreto la puerta entre el pueblo de hierro y el de madera» |
| 9 | (26, 174) | C4 | Casa 4, artesano: «Mmm... quiero fabricar algo» / «Ahora que lo pienso...» |
| 10 | (140, 192) | calle | Vecino (antes del 1er avance): no habla, activa una bandera |
| 11 | (169, 210) | calle | Vecino (antes del 1er avance): no habla, activa una bandera |
| 12 | (205, 290) | C2 | Casa 2: sin diálogo en el código |
| 13 | (37, 528) | calle | Al final: el chico que vio a la bruja Mizuno (texto PAL #229), inicia el evento 0x67 |

- **Salidas** (`topdownSectionStepD`, I0NFO2 0x800E8ED4; destinos MAIN0 0x80079A48): Ciudad de civilización (AREA18); Fábrica (AREA11 s2), puerta; AREA16 s1 (también recortada).
- **Objetos**: 7 (I0NFO2 0x801001D4 / 0x801001D8); el código no dice qué es cada uno.
- **Muñecos**: `SYSTEM/A07600` (lista de recarga SYS 76), dibujados con paletas reales: AREA16/CLUT07 fila 53 para «Male Stonetown Resident» (subtipo 2, confirmada con TCRF), fila 37 para el chico (subtipo 1, confirmada por los colores de TCRF: los 14 colores de la hoja de TCRF están en esa fila, también en AREA16/CLUT06, AREA17/CLUT03/06 y AREA18/CLUT02), y para la «Female Stonetown Resident» (subtipo 0) la **fila 21** (`0000 0421 7FFF 4AFD 3238 29B2 6EB6 5E33 4DD0 3D6E 22FC 1E56 1DD1 4210 2108 19FB`, también en AREA16/CLUT06, AREA17/CLUT03/06 y AREA18/CLUT02): fila real del disco, encontrada comparando el aspecto de la hoja de TCRF (mujer rubia, vestido lila); los colores de TCRF están desplazados (hasta 9 niveles). Sustituido: un CLUT propio derivado píxel a píxel de la hoja de TCRF con `tools/clut_from_sprite.py` (24 fotogramas, 0 diferencias de silueta, los 15 índices con un solo color); no está en ningún disco (la mejor fila del disco: error medio 30): `0000 0000 7FFF 3A78 25D2 194F 5233 41B0 314D 1D0C 1677 0DD1 096E 1D4C 10EA 21D2`.
- **Falta**: dibujo de fondo, mapa de celdas (colisión), tablas de sprites.
- **Regenerar**: `python3 tools/civ_sheets.py --area 16 --lang all --out build/civ`

### AREA17 s0, el pueblo de los Hombres de hierro

- **Casas** (`TOPDOWN_TRIGGER_RECT_LISTS` 0x800FEC9C, mundo x0,y0-x1,y1): C1 (177,203)-(270,336); C2 (454,280)-(580,420); C3 (373,100)-(488,195); C4 (-62,75)-(81,197); C5 (-151,257)-(0,422).
- **Vecinos** (NPC_SET_0F, lista en I0NFO2 0x80100784; frases vía topdownA17ActorOpenDialogue 0x800F4110 (+ DialogueBank2/3) → AREA17/MFNT170S.WFM #213-#218, #257-#268, #270-#289): 16, con la misma numeración que las láminas.

| # | Dónde (x, y) | Casa | Quién / qué dice (resumen de la frase japonesa) |
|---|---|---|---|
| 1 | (7, 152) | C4 | Casa 4: «He oído que arreglaste la torre del reloj de piedra» |
| 2 | (27, 140) | C4 | Casa 4: «¡Buen material! Espera un momento...» |
| 3 | (-75, 371) | C5 | Casa 5, el herrero: «¿Necesitas algo? ¿Un engranaje de hierro?» → te da el engranaje (objeto 0x3E) |
| 4 | (223, 123) | calle | El fabricante de bombas: «No he visto tu cara...» → «Para hacer bombas necesito electricidad» |
| 5 | (223, 0) | C1 | Casa 1: sin diálogo en el código |
| 6 | (175, 424) | calle | Vecino: «¿Quieres subir? Entra en esta choza» |
| 7 | (256, 450) | calle | Patrulla horizontal: «Bienvenido, éste es el pueblo de los Hombres de hierro» |
| 8 | (500, 363) | C2 | Casa 2: «El hierro es resistente y duradero» → «¿Arreglaste hasta el generador?» |
| 9 | (370, 310) | calle | Patrulla diagonal: «¿Por qué siempre andamos peleados?» |
| 10 | (300, 313) | calle | Patrulla horizontal: «Todos con su madera y su piedra...» → «Madera, piedra y hierro: lo importante es el equilibrio» |
| 11 | (290, 191) | calle | Vecino: «Tómame como referencia y baja recto para salir» |
| 12 | (440, 152) | C3 | Casa 3: «Antes el ascensor funcionaba con máquinas» |
| 13 | (300, 108) | calle | Patrulla horizontal: «Sube al ascensor y por el camino del medio llegas al Castillo de hierro» |
| 14 | (90, 120) | calle | Patrulla diagonal: «Ese viejo Gantetsu no me deja ir al Castillo» → «Perdona que me burlara de ti» |
| 15 | (128, 232) | calle | Patrulla diagonal: «Esta tierra también está hechizada por los Cerdiablos» |
| 16 | (70, 290) | calle | Patrulla diagonal: «Si buscas algo de hierro, ve a la choza del ascensor» → «¿Sabes nadar bien?» |

- **Salidas** (`topdownSectionStepE`, I0NFO2 0x800E9040; destinos MAIN0 0x80079A48): Ciudad de civilización (AREA18); interior de la Ciudad de hierro (AREA17 s1).
- **Objetos**: 35 (I0NFO2 0x80100368); el código no dice qué es cada uno.
- **Muñecos**: `SYSTEM/A08000` (lista de recarga SYS 80), subtipo 0 dibujado con la paleta real AREA17/CLUT03 fila 1 (confirmada: la fila que usan los Hombres de hierro de lado de AREA17/D205); los subtipos 1-4 están en páginas perdidas.
- **Falta**: dibujo de fondo, mapa de celdas (colisión), tablas de sprites.
- **Regenerar**: `python3 tools/civ_sheets.py --area 17 --lang all --out build/civ`

## Prueba en la versión para PC (`--civ-test`)

Es una opción de desarrollo de la versión nativa (`port2/`, sólo PC; la compilación de consola no cambia) que ejecuta
AREA11 s0 con su propio código (conjunto 0D, `topdownSectionStepC`, los manejadores de personajes y diálogos de AREA11)
sobre un mapa prestado: el dibujo, el mapa de celdas, las tablas de sprites y el I0NFO2 de AREA08 s0 (reglas en
`port/fixes.txt`, condicionadas a `port_civ_test`). Los vecinos recorren sus patrullas y hablan (Cuadrado, mirándolos).
Llevan el aldeano de AREA08, porque las tablas de sprites del pueblo se perdieron. Sus huecos de texto PAL tienen
"···??", así que los datos de prueba ponen ahí resúmenes en español de las frases japonesas.

```
python3 tools/civ_test_data.py SALIDA/civdata                   # copia con enlaces de data/ + MFNT110S / LD5AR11 reconstruidos
PORT_STALE_FULL=warn cmake -S port2 -B SALIDA/bp && cmake --build SALIDA/bp -j 8
SALIDA/bp/tombi2 --headless --skip-movies --data SALIDA/civdata --lang 5 --civ-test --civ-spawn 100,294 --frames 1760 \
  --poke 900:1F8001B4:1 --poke 1250:8009C960:B:2 --poke 1250:8009C962:0:2 \
  --pad-script 1000:start,1300:circle:3,1600:left:1,1610:square:2x120/8 --dump-frames SALIDA/capturas --dump-every 20
```

Los pokes abren el selector de áreas de depuración en AREA11 s0. `--civ-spawn 100,294` pone a Tombi en la línea de
patrulla del vecino 9, y Cuadrado habla con él (el fotograma 1740 muestra su frase). `tools/civ_test_data.py` guarda las
traducciones como texto y las escribe por orden de prioridad mientras el WFM no pase de 26540 bytes (`--cap`). Con más
bytes salieron glifos basura, así que caben seis de las 18 frases (`#249 #250 #251 #252 #254 #257`) en el archivo del disco. El script escribe además
`civ_MFNT110S.WFM` con las 18 frases; `--civ-wfm SALIDA/civdata/civ_MFNT110S.WFM` hace que el port cargue ese banco
directamente en memoria antes de que hable un vecino, así se oyen todas (comprobado: con los datos originales y esta
opción, el vecino 9 dice su frase traducida).

## Pueblos jugables (`--civ-town`, sólo en la versión para PC)

Los tres pueblos reconstruidos funcionan en su propia ranura (la sección 0 de AREA11, AREA16 y AREA17, las secciones
recortadas de verdad), con el código del propio pueblo (juegos de NPC 0D/0E/0F, salidas StepC/D/E, manejadores de
vecinos y de diálogo). Todo es sólo de la versión para PC y opcional; la versión de consola no cambia (reglas en
`port/fixes.txt`, condicionadas a `port_civ_test`).

```
python3 tools/civ_rebuild.py --out OUT/maps [--refdir IMAGENES] [--community-cluts CLUTGEN]   # dibujos, celdas, salas
python3 tools/civ_test_data.py OUT/civdata              # civ_MFNT110?/160?/170?.WFM, uno por letra de idioma
OUT/bp/tombi2 --lang 5 --civ-town 17 --civ-maps OUT/maps --civ-wfm OUT/civdata [--civ-collision]
OUT/bp/tombi2 --lang 5 --civ-route --civ-maps OUT/maps --civ-wfm OUT/civdata      # por el cruce, jugando normal
```

- `--civ-town 11|16|17` equivale a `--civ-test` más el selector de áreas de depuración abierto en la sección 0 de ese
  pueblo (Start y luego Círculo, como hacían los pokes del comando anterior). Tomba empieza justo dentro de la salida
  que viene de la Ciudad de civilización (AREA11 mundo (530, 410), AREA16 (-95, 524), AREA17 (223, 540);
  `--civ-spawn X,Y` lo cambia). Los límites de la cámara siguen al dibujo reconstruido (x máx = An - 288,
  y máx = Al - 112).
- `--civ-maps DIR` carga `DIR/AREAnn/map8F.tim` y `map8E.bin`, y la vista de sala `map8F_in.tim`.
- `--civ-collision` pone en su lugar los dibujos de depuración `map8F_cells.tim` / `map8F_in_cells.tim`: el mapa de
  celdas teñido sobre el dibujo (rojo macizo 9, naranja diagonal 1-4, magenta muro de un lado 5-8, cian disparador,
  cada celda con su borde), para comprobar el encaje mientras se camina.
- `--civ-wfm DIR` da a cada pueblo su banco traducido para el `--lang` actual: `civ_MFNTnn0<L>.WFM` con L =
  `BFGIHSPWD` para `--lang 0..8` (si falta, B y luego S). Los textos se traducen del JP: castellano (S) e inglés (B);
  los demás idiomas PAL llevan el texto inglés con su nombre de fichero (sin traducción propia aún). El lanzador genera
  todos en civ_cache/wfm. Un fichero en vez de carpeta sigue valiendo para AREA11.
- `--civ-route` (opcional): el cruce AREA18 s1 manda sus salidas 0/1/2 a 11:0, 17:0 y 16:0 en vez de 11:1, 17:1 y
  16:4, y la salida de arriba de la Ciudad de piedra (hacia la 16:1 recortada) sigue hasta la 16:4. Las salidas de
  los pueblos ya llevan a 11:1 / 17:1 y de vuelta a 18:1. Comprobado: andando a la derecha desde el cruce se entra en
  la Ciudad de piedra reconstruida. Necesita `--civ-maps`.

**Vecinos.** El subtipo 0 de AREA11 (el trabajador del casco, AREA11/CLUT02 fila 4), los subtipos 0/1/2 de AREA16
(filas 21/37/53 de AREA16/CLUT07) y el subtipo 0 de AREA17 (AREA17/CLUT03 fila 1) se dibujan de su página que
sobrevive con su fila real. Las tablas que ordenan los fotogramas se perdieron; el trabajador (fotogramas 0..29) y la mujer (30..56) se animan con una tabla reconstruida: el orden sale de los GIF de la comunidad (Tomba Unused Animations), cuyos cuadros coinciden píxel a píxel con la página (trabajador: todos; mujer: reposo y hablar; andar deducido de la página y de los cuadros espejados del trabajador). El chico, el forzudo y el vecino de la Ciudad de hierro siguen siendo una figura de pie. Los
demás subtipos llevan el aldeano de AREA08 (su página cargada en la VRAM x 512) con una fila real de vecino del pueblo
(AREA11 36, AREA16 54, AREA17 49), porque las filas propias del aldeano no están en el bloque CLUT del pueblo. El
decorado y los objetos con tablas perdidas (tejados, carteles, el ascensor, las plataformas) no se dibujan; su lógica
sí funciona.

**Salas.** El motor oculta la capa del mapa mientras hay una sala activa (Tomba en una celda disparadora dentro de la
casa n) y dibuja los muebles como objetos, cuyas tablas se perdieron. El port mantiene la capa y la construye con
`map8F_in.tim`: negro, y cada rectángulo de casa con su interior emparejado (`interiors` del layout). Al salir vuelve
el dibujo del pueblo. Los vecinos de la sala n aparecen como los muestra el motor.

**Títulos de evento y texto de escenas.** El título del evento de una escena (p. ej. AREA17 H4, evento 0x4C)
sigue oculto: su tabla por área y sus glifos están en los datos perdidos del pueblo. Los "bancos" de
`showMessageBoxDirect(k, dx)` son grupos del mismo WFM principal (línea = inicio del grupo k en +0x10 + dx; AREA17: 0,
207, 208, 257, 269, 270; AREA16: 0, 207, 208, 230, 248), así que ya están en `civ_MFNT170<L>.WFM`. Como las escenas
abren cuadros sin paso de diálogo, `--civ-wfm` cambia también el banco en `npcSet0ESceneStep1` / `npcSet0FSceneStep1`.
Las líneas de H4 (2,0..4) = #208-#212 son texto PAL propio de AREA17 (la corriente del Masakari y la cascada). Los
demás huecos "···??" ya están traducidos del disco JP (n.º PAL = n.º del bloque JP + 17 en AREA11/17, + 23 en AREA16,
casando las líneas PAL vecinas): AREA16 #208-#210 (el artesano de H4, JP #185-#187: quiere fabricar algo, los
Masakari, el tambor Funga), AREA17 #219, #221-#239 (la escena JP del arranque del generador: combustible, carbón,
sobrecalentamiento, el ventilador apañado; JP #202-#222), #253 (la puerta oxidada del castillo), #254 («¡Has encendido
el interruptor!»), #261 (Gantetsu acepta hacer una bomba) y AREA11 #237/#238/#241 (el artesano de balsas, JP
#220/#221/#224). Ningún código de los pueblos reconstruidos abre AREA17 #219-#239 (el X017 PAL usa su propia escena
reescrita con #220/#240+), así que se comprobaron poniéndolas en el hueco del vecino 6 (#271): todas las páginas se ven
bien con `--lang 5` y `0`. Solo queda el aviso de guardado de AREA11 #207 en "..........???" (el JP llama al pueblo
バッカスの村).

**Colisión** (`tools/civ_rebuild.py`, `towns.N.collision` del layout): formas editables en px del dibujo, calcadas
sobre los dibujos v2: `{"t": "wall", "poly": [...]}`, `{"t": "line", "pts": [...], "w": px}` (vallas, barandillas),
`{"t": "rect", "r": [x, y, an, al]}`, `{"t": "water"}` (bloquea como un muro: el motor de vista desde arriba no tiene
forma de agua ni de caída) y `{"t": "floor"}` (abre, en orden). Cada una lleva su `note`. La rasterización da celdas
al estilo de los mapas reales, porque el 9 del motor es una esquina y no un relleno (`topdownResolveCornerTile` saca
la sonda por el cuadrante y se rinde si la celda siguiente también está bloqueada, así que Tomba atraviesa un bloque
si entra por su borde): una celda maciza con un solo lado libre pasa a ser el muro de un lado que empuja hacia él
(5 izquierda, 6 arriba, 7 derecha, 8 abajo), las celdas de esquina medio cubiertas pasan a diagonales 1-4, y las
esquinas y el interior quedan en 9, como en AREA05/08/19. Después van las casas (muros en el rectángulo disparador del
código, o en `body` cuando un vecino de la sala 0 está en su patio delantero, un hueco de puerta y el interior
disparador), el borde del dibujo (cerrado salvo en los rectángulos de salida), los vecinos (cada aparición y cada caja
de patrulla se abre a suelo) y una comprobación de alcance (relleno desde la aparición de entrada, `spawn` del layout;
se avisa de cada vecino o salida a los que no se puede llegar andando). Lo calcado: en la Ciudad de hierro la roca,
las vallas de pilares, los muros de la fortaleza, las farolas, la torre de la llave, los bloques del edificio de
delante con un paso de la puerta redonda a la cúpula, la muralla con la puerta redonda, los faroles y el cubo; en la
Ciudad de madera los setos y las vallas de troncos, la aguja, la casa grande, la base del molino, la casa del muelle,
los arbustos y el caballete; en la Ciudad de piedra la muralla, las alas de las casas, el pilar roto, la cúpula de
roca, los arcos y el pozo. Es nuestra lectura de los dibujos reconstruidos, no datos originales.

**Comprobado sin ventana** (capturas con `--dump-frames`): en cada pueblo, andar con la capa de colisión y sin ella,
entrar en una casa (AREA17 H5, AREA16 H1, AREA11 H3: aparecen la vista de sala y su ocupante) y un diálogo (AREA17
vecino 6 «¿Quieres subir?», AREA16 vecino 7 «¿Entro o no entro...?», AREA11 vecino 9).

## Comprobación (tools/topdown.py)

La herramienta reconstruye las secciones que funcionan sólo con datos del disco:

- `tools/topdown.py render 8:0 out.png --grid` dibuja la vista general del Lago Baco. El TIM de 544 x 448 usa su
  propia CLUT, las formas de las celdas se tiñen encima (el bit 7 con contorno), y se marcan los registros APD y los
  personajes del conjunto 0C. Las paredes siguen exactamente los bordes del muelle.
- `render 19:2` da el mismo dibujo para AREA19 s2. Sus celdas disparadoras cubren las tres puertas de las casas, y los
  rectángulos azules de `TOPDOWN_TRIGGER_RECT_LISTS` las encierran.
- `render 5:0` (Ciudad Baco) y `grid AREA:SEC` (sólo el mapa de celdas, más un histograma de valores) cubren el resto.
- `info`, `lists`, `npcs`, `defs` y `warps` imprimen las tablas citadas en esta página.

## Paso 3 (crear los pueblos): viabilidad

| Pieza | Estado | Trabajo |
|---|---|---|
| Código (todos los manejadores, colocación de vecinos y objetos, diálogo, eventos, salidas) | completo en I0NFO2 y decompilado | ninguno para AREA11/16/17. AREA18 necesita un controlador (la lógica de cualquier StepA..E con sus propios rectángulos) |
| Lista de carga | vacía | escribir una lista como la de AREA08 s0: páginas GRPX + CLUT, 8E, 8F, 62 + 3A (se copian de cualquier sección desde arriba), 3B..3E, APD, I0NFO2 |
| Mapa de celdas 8E | falta | crearlo. Es una rejilla de bytes con 10 formas más un bit de disparador, fácil de pintar encima de un dibujo. Las posiciones fijan las condiciones: casas en los rectángulos disparadores, salidas en los rectángulos de StepC/D/E y cajas de patrulla sin paredes |
| Dibujo del mapa 8F | falta | la parte difícil. Un dibujo prerrenderizado de 8 bpp por pueblo, de unos 640 x 576 (AREA11), 448 x 544 (AREA16) y 704 x 480 (AREA17). Se puede pintar, o montar con AREA05/08/19 y los recortes de decorado de las páginas de recarga SYS |
| Sprites de personajes 3B | falta la tabla, las páginas sobreviven en parte | la página de aldeanos de 4 bpp de A06100 (y las partes equivalentes de A07600/A08000) tiene los fotogramas. Una tabla SPR nueva debe ordenar sus fotogramas como esperan los guiones de animación de I0NFO2, o hay que aportar guiones nuevos |
| Decorado 3D / tejados | falta la tabla, los recortes sobreviven en la parte de 8 bpp de las páginas de recarga | el mismo trabajo que 3B. Los tejados deben cubrir los rectángulos de las casas |
| Paletas | las filas de los vecinos sobreviven (AREA16/CLUT07 fila 53 confirmada, fila 37 (el chico) confirmada por los colores de TCRF; 48/54, AREA11/CLUT02 4/36/68 confirmadas por los colores de TCRF; AREA16 48/54, AREA17/CLUT03 1 confirmada con la tabla SPR lateral AREA17/D205) | cargar el archivo CLUT del área cuyas filas 48..54 las contienen; el TIM lleva la paleta propia del dibujo |
| Texto | PAL tiene "···??", JP lo tiene todo | traducir las frases japonesas a los huecos de PAL (ya están en los índices que usa el código) |

En la versión para PC esto es una tarea de datos, no de código. El motor ya ejecuta AREA05/08/19 s0/s2, y cargar
I0NFO2 en la base de overlays funciona (docs/PORT.md). La prueba más barata es cargar el dibujo, el mapa de celdas y
las tablas de sprites de AREA08 s0 bajo AREA11 s0 con el conjunto 0D. Los vecinos, las casas y los diálogos aparecerían
entonces sobre el muelle del lago (`plan 11 --from 8:0` muestra cómo se solapan) antes de crear ningún arte.

## USA disc (SCUS-94236)

Comparado con el disco USA (*Tomba!*) con `tools/usa_compare.py IMAGEN_USA FS_USA`. **Resultado: el disco USA tampoco tiene
los datos cenitales recortados.** Son los mismos recursos que en PAL con otros nombres; no recupera nada de lo que PAL perdió.

- **Estructura.** Un solo juego de listas de carga (`SYS/LDARnn.BIN`, `LDSYS.BIN`; PAL tiene LD0..LD8, uno por idioma) y la
  tabla de ficheros dentro de `SCUS_942.36` en el offset 0x699A0 (1054 entradas, mismo formato que MAINn). Los ficheros se
  renumeraron (PAL `SYSTEM/A07600.000` = USA `SYSTEM/A00153.000`, A07601 = A00154...), así que comparar nombres no sirve: los
  367 nombres "solo USA" y 1392 "solo PAL" de AREAnn + SYSTEM son casi todos los mismos bytes con otro nombre. Se compara por
  contenido: cada bloque de lista de carga y cada fichero entero (GAM descomprimido) contra todo PAL.
- **Los ficheros solo USA de AREA11/16/17/18** (`A003.GAM`, `B404.GAM`, `D303.000`, `D305.GAM`, `D401.000`, `D402.GAM`,
  `B103.GAM`, `Xnn.BIN`, `INFO2.BIN`) son de las secciones de scroll lateral o de la lista del área: tipos 0x10 GRPX, 0x30/0x34
  SPR, 0x54, 0x60 WPP y 0xF0 código. Sus bloques son idénticos a bloques PAL o son texto/código (abajo).
- **Las listas de la sección 0 también están vacías en USA:** registros por lista AR11 `[12, 0, 21, 12, 0, 0]`, AR16
  `[11, 0, 0, 0, 12, 13, 15, 13]`, AR17 `[12, 0, 10, 13, 11, 11, 13, 0, ...]`, AR18 `[11, 0, 16, 12]` (lista 1 = sección 0).
- **Ningún bloque 0x8E / 0x8F / 0x3B..0x3E nuevo.** Los 302 bloques USA sin gemelo PAL son de tipo 0xF0 (código: `Xnn.BIN`,
  `INFO2.BIN`), 0x54 / 0xD0 / 0xD1 / 0x34 / 0x39 (texto en inglés, fuentes, sprites de palabras del HUD), 0x70, algunos bloques
  APD 0x80..0x86 de secciones laterales y 17 páginas GRPX de SYSTEM (título y menús en inglés). Los únicos TIM de 8 bpp del
  disco son los fondos cenitales que funcionan (AREA05 D003/D203, AREA08 D002/D202, AREA19 D005/D206, AREA02 D005) y el de
  X13.BIN; los huecos entre ficheros son sectores de directorio y el final del disco son ceros.
- **Ficheros huérfanos.** Fuera de la tabla de SCUS: AREA11 CLUT01/04/05, AREA16 CLUT01..03, AREA17 CLUT01/07..11, AREA18 CLUT01
  y las copias de `INFO2.BIN` por área. Los CLUT son idénticos a los de PAL (CLUT01 es el mismo fichero en las cuatro áreas).
- **`INFO2.BIN`** (nombre USA de `I0NFO2.BIN`; una copia idéntica en SYSTEM y en cada área cenital, 136672 bytes frente a 137436
  en PAL) es otra compilación del mismo overlay: los registros NPC_SET_0D/0E/0F/10 de los pueblos recortados están igual,
  0x340 bytes más abajo (p. ej. PAL 0x8010009C = USA 0x800FFD3C); solo cambian los punteros.
- **Paleta de la Mujer de Stonetown.** Ningún bloque USA mejora el parecido con el sprite de TCRF (con colores retocados): las
  mejores filas son las mismas que en PAL (distancia media ponderada al color más cercano unos 24, sin negro/blanco/gris) y la
  mejor fila de datos solo USA es peor (unos 35). La página (`A07600` = USA `A00153`) y AREA16/CLUT07 no cambian, así que la
  suposición de las filas 48 / 54 se mantiene.

**CD-R de vista previa en inglés del 1998-02-03** ([BETA_980203_ES.md](BETA_980203_ES.md)): revisado igual, tampoco trae
nada nuevo. Las listas de la sección 0 de AREA11/16/17/18 están vacías, no hay bloques 0x8E/0x8F/0x3B..0x3E ni imágenes de
8 bpp nuevas, las únicas filas de CLUT propias de la beta son del HUD y los diálogos de los pueblos ya son «···??».

**Demo japonesa SLPM-80165** ([JPDEMO.md](JPDEMO.md), "Civilization check"): tampoco trae nada. No hay carpetas AREA16/17/18
ni listas de carga de AREA11/16/17/18 (la tabla de archivos con nombre del ejecutable solo lista SYSTEM, MOVIE, AREA00 y
`AREA15/0000.WSS`); `AREA11` solo tiene `CA11.BIN` (294 bytes, sin referencias), una tabla de datos cuyos primeros 204 bytes
coinciden con una tabla en el desplazamiento 0x30D58 de `X016.BIN` de PAL (código de AREA16 lateral, no de vista desde arriba).
La única imagen de 8 bpp es `AREA08/D002.000`, idéntica en píxeles a `AREA08/D002` de PAL (el mapa ya conocido). Ningún tramo
de 32 bytes de las páginas A06100 / A07600 / A07601 / A08000 / A08600 / A08601 aparece en el disco; las páginas de SYSTEM
propias de la demo son sprites del pueblo, cerdos, el anciano y menús. Ningún texto de la Civilización (solo los objetos
Bumerán de piedra/hierro).

## Catálogo de paletas (todos los discos)

`tools/clut_catalog.py catalog OUT` reúne todas las paletas de todos los discos que tenemos y `tools/clut_catalog.py test OUT ...`
las compara con los colores perdidos. Fuentes: los archivos `CLUTnn` (y `SYSTEM/DSPCLUT.TIM`, que es un CLUT sin cabecera), los
bloques CLUT pequeños de las listas de carga (archivos `.000` de 32..1024 bytes, p. ej. PAL `SYSTEM/A00007` 256 x 1, `A00805`
16 x 16), el CLUT de cada TIM en cualquier desplazamiento múltiplo de 4 (incluye las imágenes 0x8E/0x8F, TIM dentro de bloques
y en el ejecutable/overlays), `CLUT00` (640 x 32) y `SYS001/002/004/006/007` de PrePre 8, y una búsqueda conservadora de
series de 16 colores en el resto de datos que no son páginas (entrada 0 = 0, 15 colores de 15 bits distintos de cero, >= 12
distintos, al menos dos series seguidas). Cada fuente se corta en filas de 16 colores y ventanas de 256 (pasos de 16) y se
calcula su hash. SLPM-80165 se añadió después (8 archivos CLUT, 40 filas solo de la demo: filas de HUD/pueblo de los `CLUTnn` de AREA00 y series de 16 colores en `AREA03/CA03.BIN`).

| Disco | Filas de 16 colores distintas | Solo en este disco | Dónde están |
|---|---|---|---|
| PAL SCES-01331 | 1981 | 59 | paletas 256 x 1 de título/menú `SYSTEM/A00203`, `A00402`, `A00502`, `A09401`; los dos TIM de `SCES_013.31` (CLUT en (128,480) y (320,480)) |
| USA SCUS-94236 | 1980 | 26 | `SYSTEM/A00013`, `A001901` (paletas del menú en inglés); series de 16 colores en `X14.BIN` y `DSPSUB3.BIN` |
| JP SLPS-01144 | 1851 | 308 | las mismas filas de HUD/sistema en casi todos los `CLUTnn` (filas 2, 4, 5..7, 16..25 en columnas fijas), algunas filas propias de cada área, `DSPCLUT.TIM`, el CLUT de la imagen `D005` de AREA19 |
| PrePre 8 PCPX-96084 | 269 | 62 | `CLUT00` (59 filas), `SYS006`, dos TIM en `TOMBA.EXE` |
| Demo JP SLPM-80165 | 431 | 40 | `CLUT01..06` de AREA00 (filas de HUD/pueblo), series de 16 colores en `AREA03/CA03.BIN` |

1305 filas están en PAL, USA y JP; 171 también en PrePre 8. Ninguna fila de los `CLUTnn` de AREA11/16/17/18 es exclusiva de un
disco, salvo las filas de HUD/sistema de JP, que están en todas las áreas.

**Pruebas, sin coincidencia.**
- Female Stonetown Resident (sprite de TCRF, distancia media ponderada al color más cercano, sin grises, contra todas las filas
  de todos los discos): mejor 24,1 (fila común a PAL/USA/JP, AREA00/CLUT06), mejor fila solo JP 26,9, mejor de PrePre 8 29,8.
  Ninguna paleta exclusiva mejora la suposición de las filas 48 / 54 de AREA16/CLUT07.
- Páginas de decorado, con los renders de la comunidad como referencia índice → color, error medio ponderado por píxel de cada
  ventana de 256 colores: A08000 72,9, A08600 67,4 (una paleta de sistema 256 x 1), A06100 82,4; las mejores ventanas de
  PrePre 8 son peores (85,4 / 72,3 / 94,8), igual que las de SLPM-80165 (A08000 83,6, A08600 67,5 con `SYSTEM/A00009` = PAL `A00201`,
  A06100 95,7); la mejor fila de la demo para el sprite (24,1) es la fila común de AREA00/CLUT06. Los renders con las mejores ventanas son ruido. Los CLUT del decorado no están en
  ninguno de estos discos.

### Búsqueda de paletas con tolerancia amplia (CLUT de escenario)

La fila real de la Mujer de Stonetown resultó diferir hasta 9 niveles de los colores de TCRF. Por eso se repitió la
búsqueda del escenario con una coincidencia mucho más laxa (`tools/clut_wide/`, sólo scripts).
- **Corpus:** todos los archivos de PAL, USA, JP, la beta, SLPM-80165 y PrePre 8 (ambos volcados), salvo `.STR`. Los
  GAM se leen en bruto y desempaquetados; sin duplicados (3283 archivos). Las dos fases de byte dan 272 M palabras.
- **Disposiciones:** una ventana de 256 entradas en **cada** desplazamiento de palabra, con pasos 1, 2, 4 y 16. Esto
  cubre imágenes VRAM de 256 x N / 512 x 1, bloques de 16 x 16 y filas o columnas intercaladas. Aparte, el mejor bloque
  de 16 entradas en cualquier lugar para cada fila de la CLUT (ensamblado no contiguo).
- **Objetivos:** las CLUT propias cuantizadas de `civ_rebuild.py`. Sólo cuentan los índices medidos por clutgen o por
  los anclajes FMV (los interpolados no), ponderados por los píxeles de la página que los usan.
- **Métricas:**
  - en bruto: por índice, la diferencia máxima de canal en niveles de 5 bits, y la fracción ponderada dentro de 6 / 9 / 12;
  - corregida: un ajuste afín ponderado por canal (ganancia 0,5..2, calculado en cada desplazamiento por correlación
    FFT), y después las mismas fracciones;
  - se descartan las ventanas con menos de 80 colores distintos o con menos de 2 niveles de variación por canal.
- **Control positivo:** la CLUT real de 256 colores de AREA08/D002, alterada como una CLUT de FMV (ganancia 0,85, +2,
  ruido sigma 3, 10 % de entradas al azar), sale primera con 83 / 94 / 96 % dentro de 6 / 9 / 12. La mejor ventana
  ajena llega a 24 / 52 / 64 %.
- **Resultado (mejor ventana por página, bruto w6/9/12 -> corregido):**

| Página | Mejor ventana | Bruto | Corregido |
|--------|---------------|-------|-----------|
| A06100 | AREA02/CLUT03 desempaquetado 0x3DAE (todos los discos) | 31/52/66 % | 50/74/86 % |
| A07600/01 | JP AREA09/CLUT04 0x1294 paso 2 | 28/44/58 % | 48/70/81 % |
| A08000 | AREA11/CLUT03 desempaquetado 0x3E1A | 43/69/79 % | 67/84/88 % |
| A08600/01 | USA AREA05/INFO2.BIN 0x130 paso 2; USA X03.BIN 0x2E510 paso 4 | 46/66/79, 42/78/94 % | 67/88/97, 65/94/98 % |

  Todos los ajustes tocan el suelo de ganancia (0,5). Eso aplana la ventana hacia la media del objetivo, así que las
  fracciones corregidas premian el bajo contraste, no una coincidencia. Con estas ventanas todas las páginas salen como
  ruido. El ensamblado por bloques de 16 toma cada fila de un archivo distinto y sin relación (CLUT de otras áreas, una
  fuente, overlays), lo que es azar.
- **Conclusión:** ninguna paleta real de escenario existe en los discos que tenemos, ni con 12 niveles de tolerancia y
  corrección de ganancia/desplazamiento. Se mantienen las CLUT propias derivadas del FMV.

## Reconstrucción (no original)

`tools/civ_rebuild.py` construye una imagen de fondo (tipo 0x8F) y un mapa de celdas (tipo 0x8E) **aproximados**
para AREA11, AREA16 y AREA17 s0. Parte de tu propio disco y del fichero `tools/civ_rebuild_layout.json`, que solo
contiene coordenadas y motivos, sin píxeles. Nada del resultado es original.

```
python3 tools/civ_rebuild.py [--area 17] [--both | --no-fmv] [--refdir DIR] [--community-cluts DIR] [--preview DIR]
```

La salida va a `build/civ_rebuild/AREAnn/` (`AREAnn_nofmv/` sin la capa del FMV):

- `map8F.tim`: el TIM de 8 bpp tal como lo lee el motor (CLUT en +0x14, píxeles en +0x220).
- `map8E.bin`: el mapa de celdas (`u16 w, u16 h` y 0x4000 bytes).
- Las vistas previas: `picture.png`, `cells.png`, `preview.png` (con los vecinos, sus recorridos, las casas, los
  objetos y las salidas) y `source.png`.
- `manifest.json`: de dónde sale cada color y los avisos de las comprobaciones del mapa de celdas.

**Lo real**
- Los píxeles e índices de los recortes vienen de las páginas SYS. A06100 es Lumberjack Town, A08000 es Iron Town,
  A07600 y A07601 son Stonetown, y A08600 y A08601 son AREA18. Cada pareja comparte un CLUT.
- Las casas, los vecinos, sus recorridos y las salidas se leen de I0NFO2 y del código en cada ejecución.

**Lo nuestro**
- **Colores: derivados del FMV, no originales.** Para cada índice se usa la primera fuente disponible:
  1. `--community-cluts`: los pares *clutgen* de la comunidad ("community FMV-derived CLUT (clutgen)"),
     recalculados con su propio método.
  2. `--refdir`: los renders 125/127/128 de la comunidad.
  3. Nuestras anclas en BUNMEI.STR.
  4. Interpolación armónica sobre la vecindad de índices de la página.

  El resultado se **cuantiza al color de 15 bits de PS1**: cada canal pasa a `round(c*31/255)` y se muestra como
  `round(v*255/31)`, la misma regla que `tools/clut_from_sprite.py`. Se aplica a todos los CLUT de decorado, al CLUT
  de la imagen y a `map8F.tim`, cuyo CLUT guarda esas palabras de 15 bits con la misma regla STP que antes. El cambio
  es como máximo 4 y de unos 2 de media por canal. Algunos índices coinciden en el mismo color de 15 bits: 26
  (A08000), 25 (A06100) y 33 (A07600/01). El manifest lista cada CLUT en hexadecimal de 15 bits (`clut.clut15`),
  junto con el cambio máximo y medio y el número de coincidencias.
  Los ficheros `1..6.bmp` solo traen la paleta por defecto (`GGDdefault.act`), así que solo sirven para identificar
  las páginas.
- **Capa del FMV.** Es la mediana del barrido de BUNMEI.STR, a escala 1:1 y reindexada al CLUT del pueblo.
  - En Iron Town la posición la fija el código: la cúpula ocupa la casa H1, el eje coincide con las dos salidas y
    las personas del FMV están sobre los vecinos #15, #6 y #7.
  - En los otros dos pueblos la posición es una estimación nuestra.

  Las personas del FMV se borran. `--no-fmv` quita esta capa.
- **Colocación.** Los recortes reales van donde coinciden con el FMV. El resto es diseño nuestro, con el motivo de
  cada decisión anotado en el fichero de layout.
- **Colisión.**
  - Bloques de forma 9 para el borde, las casas y nuestros rectángulos.
  - Los interiores y las puertas de las casas son suelo de disparo (0x80).
  - Las salidas del código quedan abiertas.
  - Cada vecino y cada recorrido quedan sobre suelo, y se informa de cada celda abierta para ello.

### Interiores de las casas y layout de la comunidad (v2, estimaciones nuestras)

**Fuente del layout: los montajes de la comunidad.** Usamos las imágenes 129-132 de la comunidad (que se citan aquí
como fuente del layout) para ver cómo se unen los fragmentos de las páginas.
- Cómo se hizo la correspondencia: cada ventana de 8x8 se emparejó exactamente con nuestras páginas, por
  correspondencia color↔índice. El desplazamiento más votado da el rectángulo de cada fragmento y su posición en el
  objeto.
- Lo que se guarda: `assemblies` en el layout lista los fragmentos de cada objeto. Todos los píxeles salen de tu
  disco.

Hay 19 habitaciones en total:
- 5 de Iron Town (4 casetas de tambor con cadena y la de la tubería);
- 4 de Stonestown;
- 3 de Lumberjack;
- 7 de la página A08600 de AREA18.

`--rooms OUT` las dibuja sueltas, ensambladas y con el relleno en magenta. El relleno solo cierra juntas, un 0,2-1,4 %
de cada habitación. Cada habitación se recorta al rectángulo de su casa solo después de ensamblarla.

**Pueblos.**
- Iron Town usa la muralla sur completa con la puerta redonda y el tejado de la cúpula de la imagen 131.
- Lumberjack Town usa el molino, la valla y las tres fachadas de la imagen 132.
- Stonestown lleva la losa "Stone the No.1". La salida 1, hacia el Pozo de los Deseos Seco, queda unos 40 px al
  este del pozo del FMV; manda el código.
- El suelo son teselas rotas por parches aleatorios.
- Las personas del FMV se rellenan por difusión.
- En los mosaicos se quitan los bordes negros de cada fotograma y las bandas oscuras.

Los motivos de cada fila están en inglés, igual que en el fichero de layout.

**AREA17 (Iron Town)**

| Casa | Habitación ensamblada | Confianza | Por qué |
|---|---|---|---|
| H1 | `room_iron_pipe` (A08000, 1 fragmentos, relleno 0.4 %) | plausible | H1, the dome (93x133): the tall pipe room (75x138), the only iron room as tall as the dome |
| H2 | `room_iron_hut_c` (A08000, 5 fragmentos, relleno 0.8 %) | plausible | Iron Town has four chain-drum crank huts (Folkin) and four houses besides the dome, so every other house gets one. Which hut goes where is a guess, by size: 117x135 for the 126x140 house |
| H3 | `room_iron_hut_d` (A08000, 2 fragmentos, relleno 0.7 %) | plausible | H3 ('the lift used to run on machinery'): a chain-drum hut with a doorway, the smallest (104x117) for the smallest house (115x95) |
| H4 | `room_iron_hut_b` (A08000, 4 fragmentos, relleno 1.3 %) | guess | H4 ('good material! wait a moment'): the chain-drum hut with a plant shelf |
| H5 | `room_iron_hut_a` (A08000, 2 fragmentos, relleno 0.6 %) | guess | H5, the smith with the iron gear: the largest chain-drum hut (118x143), whose coal basket reads as a forge |

**AREA11 (Lumberjack Town)**

| Casa | Habitación ensamblada | Confianza | Por qué |
|---|---|---|---|
| H1 | `room_wood_logs` (A06100, 1 fragmentos, relleno 1.2 %) | plausible | H1, the two workers ('now I can make something'): the log workshop with tools and a pot |
| H2 | `room_wood_table` (A06100, 4 fragmentos, relleno 0.2 %) | guess | H2: the remaining wooden room (round table on a green rug) |
| H3 | `room_wood_zzz` (A06100, 1 fragmentos, relleno 0.5 %) | plausible | H3, next to resident #4 who says 'a fortune-teller lives in this hut': the ornate octagonal room with the 'ZZZ' sign; same width as the house |

**AREA16 (Stonestown)**

| Casa | Habitación ensamblada | Confianza | Por qué |
|---|---|---|---|
| H1 | `room_stone_chair` (A07600, 3 fragmentos, relleno 0.6 %) | plausible | H1, the strongman, largest house: the largest stone room |
| H2 | `room_stone_stool` (A07600, 5 fragmentos, relleno 0.8 %) | guess | H2 (#12, no dialogue): the remaining room, drawn clipped |
| H3 | `room_stone_closet` (A07600, 7 fragmentos, relleno 0.9 %) | plausible | H3, top right: the teal room with the inner wooden door = Folkin's locked closet |
| H4 | `room_stone_shelves` (A07600, 4 fragmentos, relleno 1.1 %) | plausible | H4, the craftsman: shelves, jugs and a book |

No se sube nada derivado del disco, del FMV ni de los ficheros de la comunidad.

## Notas de Folkin — verificación

Folkin (TCRF) dejó unas notas sobre los pueblos de Civilización. Hemos comprobado cada afirmación con el código
descompilado y el disco PAL (USA/JP donde se indica). Veredictos: **confirmado**, **refutado**, **en parte** (una
parte se sostiene y otra no), **no verificable** (los datos no están en el disco, o la afirmación es visual y no se
puede fijar).

**Nuevo punto de apoyo: la tabla de nombres de sala por sección.** D0SPSUB `0x80108E5C` (20 punteros, uno por área)
apunta a listas de nombres en inglés (cadenas en el código de fuente de 16 bits de D0SPSUB: `0xED`+n = a..z,
`0xCD`+n = A..Z, `0xAC` = espacio). Leída por sección da:
- AREA03: s0/s1 Stormy Mountain, **s2 Lava Caves**, s3 Phoenix's Nest;
- AREA11: **s0 Lumberjack Town**, s1 Lumberjack Factory, **s2 Dried Wishing Well**, s3 Factory, s4 Dried Wishing Well;
- AREA16: **s0 Stonestown**, s1..s6 Clock Tower;
- AREA17: **s0 Iron Town**, s1..s10 Iron Castle.

Así que las secciones recortadas tenían nombre en la versión final: Lumberjack Town, Stonestown e Iron Town.

| # | Afirmación | Veredicto | Pruebas |
|---|---|---|---|
| 1a | El FMV de purificación de las Cuevas de Lava es distinto | **confirmado** | `MOVIE/HONOO2.STR` (炎 "llama", 230 fotogramas) muestra nubes de lluvia y la lluvia apagando la lava. HANA, KARAKURI y los demás (BAKKASU, ARASHI, JUNGLE, MANTION: 232 fotogramas) muestran un barrido de destellos. No hay HONOO1 en el disco. |
| 1b | El vaquero de las Cuevas de Lava dice que la lluvia apagaría las llamas | **confirmado** | `flameBlockedNpcTalk` (X003 0x80132D80) actúa en las secciones >= 2 de AREA03. Muestra (7,0..3) = MFNT030 #249-#253 (inglés: "If we weren't inside a cave, maybe some rain could help put out these flames"). El mismo PNJ inicia el evento **0xAC "The Fire Pig Bag"** (`setEventStarted(0xAC)`). |
| 1c | La Esencia lluvia solo se puede usar en las Cuevas de Lava | **confirmado** | Comprobación de uso en D0SPSUB `subScreenItemListStep`: `D_800FECE8[objeto]` elige una lista de condiciones de `D_800FFAF8`, evaluada con `D_800FFBB0`. Para el objeto 52 (lista 16, 0x800FF290): `área != 3 → no`, `sección != 2 → no`, si no sí. AREA03 s2 es "Lava Caves" (tabla de arriba). Ningún código de X003 lee `useItemId == 0x34`, así que usarla allí no hace nada. Descripción JP: 雨雲を呼んで雨をふらせる奇跡のアイテム ("llama a las nubes y hace llover"). |
| 1d | El icono del evento "Fire Pig Bag" es el de las Cuevas de Lava | **refutado** | Los iconos son `ICONS[evento-1]` (D0SPSUB 0x800FE974). El evento 0xAC (Fire Pig Bag, CFNT #54) usa el fotograma 0xA6 (uv 0x98,0x68). El 0x22 (Lava Caves, CFNT #55) usa el 0x82 (uv 0xE0,0x28). Las dos celdas tienen píxeles distintos (A00300 palabras 128..191). |
| 1e | La Bolsa del Cerdo Verde está en la VRAM de la Serrería, así que se encontraba allí | **en parte** | Hay una bolsa con cara de cerdo en `AREA11/B101` (fábrica s1, VRAM x 904, v 0..24). Pero los mismos píxeles están en páginas de AREA02, 03, 05, 08, 10, 18 y 19 (p. ej. `AREA08/B302`, `AREA19/B003`). Es parte de una tira de objetos común, así que no prueba nada sobre la fábrica. X011 no tiene copia del código del portal de la bolsa (`updateEvilPigBagPortal`: solo X002, X010, X016). Ninguna parte SPR que hayamos encontrado fija su color. |
| — | En conjunto: el Cerdo de Fuego, y no las Cuevas de Lava, maldijo Civilización | **no verificable** | 1a-1c muestran que la lluvia en las Cuevas de Lava estaba prevista. En la versión final, la Fire Pig Bag ya es la bolsa de las Cuevas de Lava (0xAC se inicia allí). Nada del código la liga a la maldición de Civilización. |
| 2 | La Rueda de hierro solo se usa en la Torre reloj | **confirmado (versión final)** | Objeto 62 (lista 20, 0x800FF3A0): `área != 16 → no`; `sección 3 o 6 → sí`. AREA16 s1..s6 son "Clock Tower". En X016 la consumen `checkStoneGearsTrigger` / `updateBrokenStoneGearsNpc` (`useItemId == 0x3E`) y `brokenStoneGearsRepair` la retira. Se usa en la versión final, no está recortada. |
| 3a | A Iron Town se entra por abajo; el camino de arriba lleva al Castillo de hierro | **confirmado** | `topdownSectionStepE`: la salida 0 con y > 0x23A va a AREA18 (el centro); la salida 1 con y < 0x50 va a AREA17 s1, llamada "Iron Castle". El vecino #11: "para salir, ve todo recto hacia abajo". |
| 3b | Una manivela a la izquierda del cartel "Iron the No.1" | **en parte** | A08000 tiene una rueda de manivela con mango en 6 fotogramas de giro (px de página 817..896, 152..202 = palabras de VRAM 472..512). Su posición junto al cartel necesita el mapa perdido. |
| 3c | Sprites del cartel cayéndose | **no verificable** | A08000 tiene dos recortes de cartel sobre un poste (px 690..728, 128..143), pero ninguna secuencia de inclinación legible. |
| 3d | Ascensores de cadena con 4 casetas de manivela, con sprites de interior | **confirmado** | A08000 tiene cuatro interiores, cada uno con un tambor de cadena sobre base hexagonal y la cadena subiendo por la pared (px 128..390). Código: dos plataformas móviles (props `wb8` 0xD/0xE, `TOPDOWN_A17_SCENERY_LIMITS`) y las frases "¿quieres subir? entra en esta caseta", "el ascensor funcionaba con una máquina", "prueba en la caseta del ascensor". Hay cinco casas, cuatro de ellas con caseta. |
| 4a | Un render distinto (maldito) de la Torre reloj | **no verificable** | No hemos identificado arte de la torre en A07600/A07601. Las páginas de recarga de AREA16 s1..s6 (A077xx/A078xx/A079xx) son idénticas byte a byte a las B300..B600 de la versión final. |
| 4b | Un sprite del cartel "Stone the No.1" | **en parte** | A07601 tiene una losa de piedra con letras en relieve (px 137..176, 200..240); solo se distingue "...ton...". Es la pareja del cartel de hierro, pero no se lee lo bastante para estar seguros. |
| 4c | El pozo lleva a la zona "Dried up wishing well" | **confirmado** | La salida 1 de `topdownSectionStepD` es un agujero de 7 x 1 px (x 0x6B..0x72, y 0x13F..0x140), no una puerta, y lleva a AREA11 s2 punto 0. El nombre de AREA11 s2 en la versión final es "Dried Wishing Well". |
| 4d | Un armario cerrado en la casa de arriba a la derecha | **no verificable** | H3 (219,176)-(299,258) es la casa de arriba a la derecha: un prop de tipo 2 (sala 3), ningún vecino y ningún código de cerradura. A07600 tiene una sala verde azulada con una puerta interior de madera (px 280,154), la que la reconstrucción de arriba usa para H3. |
| 5a | Lumberjack Town tiene una parte subterránea bajo el suelo | **confirmado (texto)** | Frases JP (4,3) "hay un laberinto bajo el suelo" y (4,4) "a esa choza solo se llega por debajo del suelo" (`topdownA11ActorOpenDialogue` casos 6/7). |
| 5b | A la fábrica se llega por el camino de arriba | **confirmado** | La salida 1 de `topdownSectionStepC` con y < 0x4F (x 0xC7..0x107) va a AREA11 s1, "Lumberjack Factory". El vecino #10: "la entrada del taller está ahí arriba". |
| 5c | Una roca en la Serrería | **no verificable** | No hay objeto de roca/voladura en X011 ni en las páginas de la fábrica. |
| 6 | Dos vecinas de Stonetown (#1, #2), un niño y un hombre; el hombre tiene sprites cenitales y laterales | **en parte** | El conjunto 0E usa cuatro subtipos. **0** = la mujer con capucha (#3, #5, frases femeninas: #241 あたしも). **1** = el niño (#7, #8, frases infantiles, la puerta secreta). **2** = el forzudo (#4; los scripts 16-23 son animaciones de levantar, fotogramas 120-129). **3** = 8 vecinos (#1, #2, #6, #9-#13) en una página perdida (caja 16x32, habla masculina おれ). A07600 tiene un único juego de fotogramas de mujer (30-59). El color de un actor sale de sus partes SPR, así que un subtipo da una paleta: el código tiene sitio para **un** tipo femenino. Una segunda mujer solo puede ser el mismo dibujo con otra paleta (filas 48/54 de AREA16/CLUT07, o la CLUT propia de TCRF, ver imágenes), y ningún código que se conserve la usa. Los fotogramas cenitales del forzudo (frente, espalda, lado) están en A07600; los de vista lateral, en la página B201 de la versión final (AREA10/16/17/18). |
| 7 | La frase de la puerta secreta («Abrí en secreto la puerta…») es del vecino 8 de AREA16 | **confirmado** | El actor 7 (#8 en las láminas: subtipo 1 niño, b0c 8, patrulla diagonal, aparece tras el primer avance) usa la fila b0c 8 = (3, 0/9/13/13). (3,9) = MFNT160 #239 = bloque JP #216: へへ・・・こっそり、鉄人の町と木人の町のトビラ・・・あけてきちゃった・・・ないしょだよ ("jeje... abrí a escondidas la puerta entre el pueblo de los Hombres de Hierro y el de la Gente de Madera... es un secreto"). En los avances 2/3 pasa a #243 (JP #220): "me han pillado la travesura... pero es un rollo que todos se peleen". |

Además: el objeto 109 (Agua negra) solo pasa la comprobación de uso en AREA18 s2 con la H del jugador entre 128 y 192
(lista 22, 0x800FF3E8). Ningún código reacciona a ella.

**Imágenes** (no se suben; regenéralas con tu disco). Las páginas de Stonetown e Iron Town son de 8 bpp y no se
conserva su CLUT, así que se colorean con la CLUT derivada del FMV de `tools/civ_rebuild.py`, que **no es la
original**. Los sprites de vecinos usan filas CLUT reales donde se conocen:
- `stone_sign_A07601.png`
- `crank_huts_A08000.png` (las cuatro casetas de tambor)
- `cranks_signs_A08000.png` (fotogramas de la manivela, postes, carteles)
- `rails_A08000.png`
- `pig_bag_factory_B101.png` (filas de CLUT02 como candidatas; la fila real se desconoce)
- `stonetown_residents_palettes.png` (A07600 con las filas 48, 54, 37, 53 de AREA16/CLUT07 y la CLUT derivada de TCRF)
- `icons_lava_vs_firebag.png`
- `fmv/sheet.png` (HONOO2 frente a HANA y KARAKURI)
