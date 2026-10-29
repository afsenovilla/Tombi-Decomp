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
  - AREA16/CLUT07 filas 48 y 54: «Female Stonetown Resident» de TCRF (vecina de Stonetown) y el resto de la gente de
    piedra. Plausible, no confirmado.
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
    naranja con remaches, mangas blancas, pantalón verde, pelo negro de punta). Coincide con una captura del juego de
    un vecino del mismo tipo (un vecino de la Ciudad de hierro de lado, ante la puerta del Castillo de hierro: los 15
    colores de la fila aparecen en él; por conjunto de colores, no píxel a píxel).
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
- **Muñecos**: `SYSTEM/A07600` (lista de recarga SYS 76), dibujados con paletas reales: AREA16/CLUT07 fila 53 para «Male Stonetown Resident» (subtipo 2, confirmada con TCRF), filas 54 (subtipo 0) y 48 (subtipos 1 y 3) para el resto (suposiciones).
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
- **Muñecos**: `SYSTEM/A08000` (lista de recarga SYS 80), subtipo 0 dibujado con la paleta real AREA17/CLUT03 fila 1 (según una captura del juego); los subtipos 1-4 están en páginas perdidas.
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
bytes salieron glifos basura, así que caben seis de las 18 frases (`#249 #250 #251 #252 #254 #257`); el script lista las
demás.

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
| Paletas | las filas de los vecinos sobreviven (AREA16/CLUT07 fila 53 confirmada; 48/54, AREA11/CLUT02 4/36/68 confirmadas por los colores de TCRF; AREA16 48/54, AREA17/CLUT03 1 según una captura del juego) | cargar el archivo CLUT del área cuyas filas 48..54 las contienen; el TIM lleva la paleta propia del dibujo |
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

## Catálogo de paletas (todos los discos)

`tools/clut_catalog.py catalog OUT` reúne todas las paletas de todos los discos que tenemos y `tools/clut_catalog.py test OUT ...`
las compara con los colores perdidos. Fuentes: los archivos `CLUTnn` (y `SYSTEM/DSPCLUT.TIM`, que es un CLUT sin cabecera), los
bloques CLUT pequeños de las listas de carga (archivos `.000` de 32..1024 bytes, p. ej. PAL `SYSTEM/A00007` 256 x 1, `A00805`
16 x 16), el CLUT de cada TIM en cualquier desplazamiento múltiplo de 4 (incluye las imágenes 0x8E/0x8F, TIM dentro de bloques
y en el ejecutable/overlays), `CLUT00` (640 x 32) y `SYS001/002/004/006/007` de PrePre 8, y una búsqueda conservadora de
series de 16 colores en el resto de datos que no son páginas (entrada 0 = 0, 15 colores de 15 bits distintos de cero, >= 12
distintos, al menos dos series seguidas). Cada fuente se corta en filas de 16 colores y ventanas de 256 (pasos de 16) y se
calcula su hash. SLPM-80165 no estaba disponible en esta pasada.

| Disco | Filas de 16 colores distintas | Solo en este disco | Dónde están |
|---|---|---|---|
| PAL SCES-01331 | 1981 | 59 | paletas 256 x 1 de título/menú `SYSTEM/A00203`, `A00402`, `A00502`, `A09401`; los dos TIM de `SCES_013.31` (CLUT en (128,480) y (320,480)) |
| USA SCUS-94236 | 1980 | 26 | `SYSTEM/A00013`, `A001901` (paletas del menú en inglés); series de 16 colores en `X14.BIN` y `DSPSUB3.BIN` |
| JP SLPS-01144 | 1851 | 308 | las mismas filas de HUD/sistema en casi todos los `CLUTnn` (filas 2, 4, 5..7, 16..25 en columnas fijas), algunas filas propias de cada área, `DSPCLUT.TIM`, el CLUT de la imagen `D005` de AREA19 |
| PrePre 8 PCPX-96084 | 269 | 62 | `CLUT00` (59 filas), `SYS006`, dos TIM en `TOMBA.EXE` |

1305 filas están en PAL, USA y JP; 171 también en PrePre 8. Ninguna fila de los `CLUTnn` de AREA11/16/17/18 es exclusiva de un
disco, salvo las filas de HUD/sistema de JP, que están en todas las áreas.

**Pruebas, sin coincidencia.**
- Female Stonetown Resident (sprite de TCRF, distancia media ponderada al color más cercano, sin grises, contra todas las filas
  de todos los discos): mejor 24,1 (fila común a PAL/USA/JP, AREA00/CLUT06), mejor fila solo JP 26,9, mejor de PrePre 8 29,8.
  Ninguna paleta exclusiva mejora la suposición de las filas 48 / 54 de AREA16/CLUT07.
- Páginas de decorado, con los renders de la comunidad como referencia índice → color, error medio ponderado por píxel de cada
  ventana de 256 colores: A08000 72,9, A08600 67,4 (una paleta de sistema 256 x 1), A06100 82,4; las mejores ventanas de
  PrePre 8 son peores (85,4 / 72,3 / 94,8). Los renders con las mejores ventanas son ruido. Los CLUT del decorado no están en
  ninguno de estos discos.
