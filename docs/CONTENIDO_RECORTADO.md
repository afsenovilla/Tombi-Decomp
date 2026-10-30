# Contenido recortado: inventario y estado

Versión inglesa: [CUT_CONTENT.md](CUT_CONTENT.md).

Otras versiones revisadas en busca de contenido recortado: [PREPRE8.md](PREPRE8.md) (PrePre Vol. 8, 1997-06),
[JPDEMO.md](JPDEMO.md) (SLPM-80165) y el CD-R de vista previa en inglés del 1998-02-03,
[BETA_980203_ES.md](BETA_980203_ES.md) ([English](BETA_980203.md)): guion inglés temprano, un vídeo 100US subtitulado y
una apertura instrumental, pero ningún dato recortado de Civilización.

Todo lo que el port restaura es **opcional** (`tombi2 --cut-content`, casilla «Activar contenido recortado» del
lanzador) y **solo del port** (reglas de `port/fixes.txt` o código de `port2/runtime`). La compilación que coincide
byte a byte no se toca (`tools/build_full.py`, todos los sha1 OK). No se distribuye nada de ningún disco: el texto sale
del disco del jugador (o de una traducción nuestra incluida) y el arte, de los discos del propio jugador.

Estado: **restaurado** (jugable con `--cut-content`), **posible** (hay datos y un punto de enganche, no hecho; se indica
el coste), **perdido** (los datos necesarios no están en ningún disco que tengamos).

En cada entrada se separa lo que dicen los datos del juego de lo que es decisión nuestra («Nuestro:»).

## Restaurado

| Hallazgo | Evidencia | Cambio en el port | Nuestro |
|---|---|---|---|
| Tres frases más del barquero (AREA00) | MFNT000 #162-164 = (7,3..5); `a00Variant0EventScene` (X000 0x801363E4) pasa del estado 2 al 3 sin abrir cuadro; JP X00 0x80134AD8 igual. Texto JP en Ore! Tomba. | fixes.txt `a00Variant0EventScene` (commit 43855ca) | nada (siguen a su última frase) |
| Carta de Ciudad Baco (AREA03) | MFNT030 #208 = cartel (1,1) = `D_80079EBC[27]`; en AREA03 no hay cartel de subtipo 27 (PAL y JP). | fixes.txt `updateL4Type1AObject` crea un cartel subtipo 27 a 40 px a la izquierda del de guardar (43855ca) | la posición |
| Bayas plateadas / doradas | `initItemObject` filas de CLUT 503/504; `rewardHeart` da 2 PV si `b0c & 0x7f != 0`; ninguna tabla de botín usa b0c 2/3. | fixes.txt `spawnItemFromEntry` + `initItemObject` (dafdbcf) | frecuencias 1/8 plata, 1/32 oro |
| Bocadillo grande «?» | `D_80079D34[1]` (SPR 0x34 f48); `spawnItemPickupObject` solo se llama con 0. | fixes.txt `topdownActorOpenDialogue` (dafdbcf) | cuándo sale |
| **Pantalla de título temprana** (PrePre Vol. 8) | PCPX-96084 `SYSTEM/SYS005.GAM` (8 bpp, página 768x256; x 0..319, y 0..239 = paisaje con roca-torre y río) + `SYS006.000` (su paleta). El final dibuja su paisaje `SYSTEM/A00200` (8 bpp, 320x240) desde VRAM (0,0) con la paleta en (128,511): la imagen antigua cabe en el mismo hueco. | `--early-title DIR` (requiere `--cut-content`), `port2/runtime/titlebg.c`: mientras se ve el título (`titleLogoDraw`, vía `port_title_bg_mark`) se escriben la página y la paleta sobre las de A00200. Lanzador: campo de carpeta bajo la casilla. | Mantenemos el logo TOMBI!, el menú y los textos finales: las piezas del logo antiguo «オレっ！トンバ» están en la misma página (x 512..767, paleta SYS007) pero no se sabe cómo se montaban. Los ficheros se leen de la extracción del usuario (`tools/prepre8.py extract DISCO SALIDA`). |

Comprobación con capturas (port sin ventana, fotograma 400 con `--skip-intro`): paisaje final frente al de PrePre8
bajo el logo final.

## Posible (no hecho)

| Hallazgo | Evidencia | Qué haría falta |
|---|---|---|
| Cartel «DIRT MOTOCROSS RACE ¡A por ella!» | MFNT000 #165 = (8,0); ninguna llamada con k=8 en X000/MAIN0 (PAL ni JP). La tira de títulos de PrePre8 tiene モトクロスコース. | Un cartel que apunte a (8,0) (un subtipo del port; `D_80079EBC` no tiene ninguno) y un sitio: **no hay evidencia de dónde estaba**, la posición sería nuestra. Barato (como la carta). |
| Carteles de AREA00 por configuración de botones | MFNT000 k1 dx 8,10,14,15,21,22 y 24..29: variantes de consejos que PAL ya muestra; `D_80079E8C..EB0[BUTTON_CONFIG_INDEX]` nunca las elige. | Nada nuevo que mostrar. No compensa. |
| Frases del enano leñador (AREA02/19) | MFNT020 (2,54..56) «Los enanos... buenos cortando árboles», «¿quieres que corte un árbol?»; ningún literal/variable WSS llega a dx 54-56. Evento sin usar 18 «La ayuda de los enanos». | Cambiar un script WSS (AREA15/02xx.WSS op 0x85) y un enano que lo ofrezca; medio. |
| Frases de AREA02 grupos 3/4/6 («¡Nos hacen retroceder!», «Hermano... ¡deprisa!») | MFNT020 (3,5..6), (4,0..2), (6,16); X002 solo llama k 2, 3 (dx 0-4), 7, 9. | No hay escena donde colgarlas: medio-difícil. |
| Objetos sin usar (47; 21 con descripción JP) | Lista `unused_items_jp.txt`: Esencia de lluvia (52), mitades de mapa (57/58), guantes de goma (59), hierro (61), carta (90), madera (91), pala (97), anguila eléctrica (108), agua negra (109), cinco caramelos (110-114), piel/árbol/cola de Funga (119/122/123), tambor pequeño (137), lanza del jefe (140), carbón (147). Nombre = MFNT id+3, descripción = D0SPSUB bloque 0 id+4 (inglés PAL «···???»). | Darlos es un `addItemToInventory`, pero nada los usa; casi todos son de las cadenas cortadas de la Civilización / Masakari. Solo un visor sería honesto. Las descripciones JP podrían ir en un parche de texto. |
| Eventos sin usar (68; 26 con información JP) | `unused_events_jp.txt`: 6 «¿Un gran charco?» (la tira de PrePre8 tiene あれま大きな水たまり: la misma idea en dos versiones), 18 ayuda de los enanos, 45 tragaperras de AP, 52 «Ya no puedo cruzar...», 59-61 cadena Masakari, 74 atrapar la anguila eléctrica, 76 agua negra, 77 máquina de la Civilización, 83 coche de hierro. Ningún `setEventStarted/Complete` con esos ids. | Cada uno necesita su disparador y su objetivo; ninguno tiene código. Medio-difícil; solo títulos y descripciones son baratos (parche de texto). |
| Títulos de evento a 0xFF en PAL | Las tablas JP por área (`tools/event_titles.py`, notes/secrets.md) dan título a 25: 76 = PAL #90 "The Black Water", 83 = PAL #126 "The Rusty Steel Car" (los dos textos PAL sin referenciar), los del pueblo recortado 0x61 動かない時計台 «La torre del reloj que no anda», 0x63 材料はどこだ！？ «¿Dónde está el material?», 0x62 «¿Un engranaje irrompible?», y 6, 18, 45, 52, 59-61, 67, 74, 89, 94-96, 98, 100, 138-140, 184, 186. Beta 980203 = tabla PAL. | Parche de tabla: 76/83 reutilizan textos existentes (barato); el resto necesita textos nuevos. |
| Port: títulos de los eventos a 0xFF | Hecho en el port con `--cut-content`: 76->#90, 83->#126 y 22 títulos traducidos del JP (lista en notes/secrets.md; tools/port_event_titles.py). USA y beta 980203 tienen la tabla PAL. | La lista de eventos del sub-screen no cambia. |
| Títulos de evento de PrePre8 少年旅立ち / あれま大きな水たまり | Tira de glifos 16x16 de TIM000 (x 0..224, y 104..232). | Solo texto: una cadena WFM3 de título (el codificador existe). Necesita un evento que la lleve (ver evento 6). |
| Interior 2D de la casa del anciano (PrePre8) | `TIM007` 8 bpp + CLUT00 fila 20, `DATA002` (solo colisión). | Un camino de sala 2D que PAL no tiene para esa casa; mejor como galería. Medio-difícil. |
| Pájaro gris enemigo (PrePre8) | TIM000 (~16 poses); candidato tipo 15, `func_8003D40C` en TOMBA.EXE. | Sin IA equivalente en PAL; solo decoración o galería. Difícil como jugabilidad. |
| 15 sonidos de PrePre8 | JOCHU 1, 5, 11, 12, 19-21, 23-25, 27; S_AREA01 0, 25, 28, 29. | Fácil meterlos en un banco; no hay evidencia de dónde sonaban. |
| OPTION MODE / visor de enemigos (PrePre8) | 0x800854B4 (`OPTION MODE`, pruebas de música/sonido/CD/tarjeta), 0x80083D88 (`enemy=%01d`), código muerto en esa versión. | Reimplementación solo en el port; PAL no tiene ese código. |
| Cactus / cartel del GoGoCart (AREA06) | VRAM de `A001.GAM` (577,256), (516,256), (581,348); sin parte SPR ni polígono; D004.000 tiene 60 modelos y `a06AddModelsBySection` usa los 60. | Partes de sprite nuevas y colocación: nuestras. |
| Página de AREA17 B201 (hombre con mochila, grandullón con correa, anciano calvo, caminante con casco; de lado) | **No es contenido recortado: se retira.** El usuario ha visto al hombre de la mochila en el juego final. La misma página de 192 x 256, idéntica byte a byte, la cargan AREA10 s0 (B002, VRAM x 384) y s4 (B403, x 640), AREA16 s5/s6 (B501, B601), AREA17 s2/s5 (B201, B501) y AREA18 s1 (B101). Nuestras comprobaciones estáticas no encuentran ninguna referencia a sus figuras: ningún fotograma de tabla SPR de ninguna lista de área cuyas partes las encuadren, ningún banco WPP con sus píxeles y ningún registro de parte de 16 bytes que apunte a ellas. Esas comprobaciones no ven las primitivas construidas por código ni otras vías de dibujo, así que aquí «sin referencia estática» no significa sin usar. Todavía no se conoce la sección, el id SPR ni la dirección en el juego. La página B201 se ve con una paleta real: AREA11/CLUT02 fila 36 (la misma fila en AREA17/CLUT03, AREA17/CLUT06 y AREA18/CLUT02; la fila del «Male Lumberjack Town Resident») colorea a su hombre de lado con mochila igual que el sprite en color de TCRF (los 14 colores están en esa fila; la silueta se parece pero no es píxel a píxel, el fotograma de TCRF es distinto). Identificación nuestra. | Encontrar la vía de dibujo (captura dinámica en AREA10 s0/s4 o AREA17 s2) antes de dar ningún fotograma por no usado. |
| Overlay propio de AREA19 `X019.BIN` (y `X119`..`X819`) | Está en el disco (`SYSTEM/` y `AREA19/`, 190.180 B, sha1 `94060c10...`) pero **nunca se carga**: no aparece en ninguna de las nueve tablas de ficheros (notes/cdfiles.md), y las listas de carga de AREA19 nombran `AREA19/X002.BIN`, idéntico byte a byte al `X002.BIN` de AREA02 (sha1 `646aa8b5...`). El JP hace lo mismo: AREA19 trae `X02.BIN` y `X19.BIN` y sus listas solo nombran `X02`. El pueblo purificado ejecuta el código de AREA02; X019 es una versión muerta de AREA19. | Estudiar qué hace X019 distinto de X002 (su parte de área, desde 0x80115EA4); un interruptor en el port para usarlo sería nuestro. Coste desconocido. |
| Bocadillo «?» pequeño | SPR 0x34 f49; sin guion ni puntero. | Solo gráficos. |

## Perdido

| Hallazgo | Por qué |
|---|---|
| Pueblos de la Civilización (secciones cenitales de AREA11/16/17/18) | Los lleva docs/CIVILIZATION.md (`--civ-test`): el código de I0NFO2 está completo, pero las listas de carga están vacías y sus ficheros de mapa / SPR / paletas no están en ningún disco. 63 frases (clase B*). |
| Fotogramas de la flor dorada creciendo (AREA02 SPR 0x3B f27-32) | Los rectángulos a los que apuntan están vacíos en la VRAM PAL: el arte no está en el disco. |
| Texto JP de clase D ligado a la Civilización (MFNT110/170/180: balsero, generador, carbón, Dr. Gantetsu, pueblo nuevo) | Sus escenas son los pueblos cenitales cortados. |
| Guion de diálogos de PrePre8 | La fuente 10x10 está, pero no se encontró texto que la use (abierto, ver docs/PREPRE8.md). |

## Texto solo en JP (SOLO_JAPON, 179 candidatas)

Diálogos JP cuyo hueco PAL está vacío o es relleno, alineados por la estructura de códigos (SLPS-01144 frente a PAL).
Casi todo ya está arriba: pueblos de la Civilización MFNT110/160/170/180 (116, ver Perdido / docs/CIVILIZATION.md),
descripciones de objetos sin usar (DSPSUB#0, 21) y pistas de eventos sin usar (DSPSUB#1, 7) (Posible: objetos/eventos
sin usar), variantes de pistas de AREA00 por configuración de botones (14), frases del barquero (MFNT000 JP #156-158),
cartel de motocross (#159), frases del enano leñador (AREA02/19, 6), frases de grupo de AREA02 (7) y carta de Baccus
(AREA03 #178). Nuevo, menor:

- AREA00 JP #111 `ポスト　あけるな！` ("Buzón: ¡no abrir!"): sin hueco PAL. Sin ubicar.
- AREA03 #174 y AREA10 #171 `ポテンシャルが　たりない` ("Potencial insuficiente"): el hueco PAL es relleno.
  Probablemente un aviso de AP/requisito; no rastreado en el código.

Menor: el disco JP tiene carpetas vacías `AREA20`..`AREA24` (el PAL no las tiene); no contienen nada ni se carga nada de ellas.

## Cómo se usa

```
tombi2 --data data --cut-content [--early-title /ruta/a/extraccion_prepre8]
python3 -I tools/prepre8.py extract PREPRE8.bin /ruta/a/extraccion_prepre8   # una vez, desde tu propio disco
```

## Iconos de eventos (la hoja comunitaria de "iconos de objetos")

La hoja comunitaria corresponde a la **página de la lista de eventos**, no al inventario. Se analiza con `tools/event_icons.py`; los detalles están en CUT_CONTENT.md.
- **Página:** VRAM (256,0), que en PAL es `A00300.000`, palabras 128..191.
- **Icono de cada evento:** `D_800FE974[k]`, que remite a un fotograma de la tabla de sprites kind 0x39.
- **CLUT:** columna 320 del bloque de D0SPSUB (+0x98B8).
- **Inventario:** usa otra página, VRAM (192,0), con `ITEM_ICON_CLUTS` en x=304.

**PAL, USA, JP y beta tienen los mismos 166 fotogramas.** En PAL nunca se muestran 40:
- **Sin evento:** 16 fotogramas.
- **Evento fuera del orden del menú:** 23.
- **Nunca otorgado:** el 202, el engranaje del evento 77 "La máquina de la civilización".

**Relacionados con Civilización:**
- engranajes 202 (ev 77) y 213 (ev 96);
- cerradura de hierro 209 (ev 100);
- coche de acero oxidado 253 (ev 83);
- agua negra 201 (ev 76);
- materiales 216 (ev 99).

La bomba (211, ev 106) sí se usa.

**La fila "Unused" de la comunidad (22 iconos):** todos están entre esos 40, salvo el primero, el anillo rojo. Ese es el fotograma 101, el icono comodín, que usa el evento 0 (Pulsera del abuelo). **Faltan en esa fila:** 107, 115, 139, 140, 159, 172, 173, 183, 192-194, 197, 199, 202, 203, 205, 210, 213, 214, 216, 230.

**Exclusivos de las demos:**
- **SLPM-80165:** fotogramas 79 charco, 87 hacha, 110 enano gorro rojo, 111 tragaperras, 112 "II"+cuenco, 136 cerdo, 144 moneda, 151 tarro.
- **PrePre8:** 59 charco, 67 hacha, 72 enano gorro verde.
- **Retail:** conserva los ids de evento, con otros iconos o sin mostrarlos.

## Eventos sin título PAL (0xFF): dónde se inician

Tabla completa en notes/secrets.md, "Where the 0xFF events are started / completed". Es un escaneo estático de los dos discos (PAL = JP):
- **97 "La torre del reloj parada"** lo inicia una escena de la Ciudad de Piedra recortada (AREA16 s0, I0NFO2 `npcSet0ESceneStep1`). Lo completa el objeto
  de los engranajes de la Torre reloj (X016 `brokenStoneGearsRepair`) cuando se usa la Rueda de hierro (objeto 0x3E).
- **98 "¿Un engranaje irrompible?"** se inicia en esos mismos engranajes (X016 `brokenStoneGearsTalk`, "los piñones de piedra están rotos"). Lo completa el herrero
  de la Ciudad de hierro recortada al dar la Rueda de hierro (AREA17 s0, `npcSet0FSceneStep4`). Ese herrero es el único código que da el objeto 0x3E.
- **76 (Agua negra)** y **99 (¡¿Dónde está el material?!)** solo se inician en la Ciudad de hierro recortada (AREA17 s0) y nada los completa. **61** solo lo lee el diálogo de la Ciudad de Piedra recortada.
- **Los otros 19** (6, 18, 45, 52, 59, 60, 67, 74, 83, 89, 94, 95, 96, 100, 138, 139, 140, 184, 186) no tienen código, datos de sala ni script que los inicie, complete o lea.
  **89 (Adivino verde)** es uno de ellos. Su único contexto es una frase del pueblo de madera recortado: "en esta choza vive un adivino, pero tiene una condición" (AREA11 s0).

## Jungla (AREA10): lo que queda del contenido Masakari / Funga

AREA10 (overlay `X010`, texto `MFNT100*`, JP `X10` + `AREA10/A003.GAM`) es toda la jungla: nombres de guardado "Jungla" y
"Colina del árbol noble"; lista de títulos JP ジャングル / ジャングル奥地. No existe un área "Jungla Masakari" aparte.

- **Verificado: la tabla de títulos JP de X10 (JP `X10.BIN` +0x44068) titula aquí los eventos cortados:** 6 大きな水たまり？,
  59 族長のたのみごと？ (el encargo del jefe), 60 コトバを吸いとろう！Ⅱ, 61 マサカリ族のヒミツ, 74 電気うなぎを捕まえろ！,
  184 おいら、悪い子？ ("¿Soy un niño malo?"), 186 マサカリの集落？, y 138-140 (tres hermanas). La cadena Masakari era de la jungla.
- **Verificado: no hay código para 59/60/61/74/186 en PAL.** Ningún `jal setEventStarted/Complete` (0x8005A8A8 / 0x8005A9A4) con
  esos ids como a0 inmediato en ningún X0nn, overlay de SYSTEM ni el exe; ningún `lui 0x800A` + carga/escritura de `SCRIPT_FLAGS`+E en
  los binarios de idioma 0 salvo la lectura ya conocida de 61 en I0NFO2 (+0xAE00, +0xAF1C, pueblo cortado de AREA16). El resto de
  aciertos caen en datos de las variantes de idioma.
- **Verificado: la escena del juicio del jefe Funga sobrevive solo como texto.** `MFNT100S` grupo 2 (base 211) dx 11-18 = PAL
  #222-#229 (JP AREA10 A003 bloque 0 #188-#195): "¡Eres tú! ¡Nuestro amigo! ¡No has hecho daño a nadie!", "Si tuviera un **tambor
  Funga** me pondría a bailar" (JP añade: es un secreto, ¡olvídalo!), "¡Puedes irte!", "¡Has herido a mis amigos! ... Voy a mirar en
  tu destino; si has sido amable te perdonaré. ¡Fungaaa! ¡Hongaaa!", "Has sido bueno, te perdono", "¡Has sido un malvado! ¡Lo
  pagarás!". dx 14 y 18 son los mismos discursos en galimatías Masakari (glifos ◆★ en JP, `?????` en PAL), lo que se ve antes de
  entender el idioma. Ninguna llamada estática llega a ellos. Las 4 llamadas con argumentos calculados de X010 dan el grupo 4
  (lostHatManTalk) y el grupo 2 dx 22-36 (profesor de natación, tabla de botones `D_8012F3C8` = dx 27-32), y los 9 WSS tampoco los
  usan. La comprobación del destino probablemente iba con el evento 184 "¿Soy un niño malo?" (inferido).
- También sin referencia en el banco: grupo 5 dx 15 ("¿y esa cara de cansancio?"), y los
  avisos de guardado del grupo 1 (van por datos, no son cortes). El JP #171 ポテンシャルが　たりない ("no hay potencial suficiente") está vacío en PAL.
- **Sprites:** las tablas SPR de AREA10 dan 37 fotogramas candidatos, todos poses de Tomba (nadar/pelear, movidas por código: falsos
  positivos). La SPR 0x33 JP de AREA10 tiene 3 fotogramas que no están en PAL (f41, f42, f45: barras de color, sin dibujo). No hay
  sprite del jefe, de la anguila ni del tambor.
- **Sin revisar:** lista completa de funciones muertas de X010 (el escaneo simple de punteros da demasiado ruido) y subtipos colocados por sala.

## Notas de Folkin — verificación (afirmaciones fuera de Civilización)

Las afirmaciones sobre Civilización se comprueban en [CIVILIZACION.md](CIVILIZACION.md) ("Notas de Folkin —
verificación"). Estas son las demás, comprobadas solo cuando era barato. **Sin verificar** significa que no lo
hemos comprobado.

| Afirmación | Veredicto | Pruebas |
|---|---|---|
| Adivino verde, recortada del todo | **en parte** | El evento 89 tiene icono propio (fotograma 227, "adivino verde", ver Iconos de evento), pero no está en `SUBSCREEN_EVENT_ORDER` y ningún código lo inicia. La versión final tiene "The Red Fortune Teller" (CFNT #69) y "The Blue Fortune Teller" (#125). El objeto sin usar 90 es una carta "para las tres hermanas". No hemos mirado sprites. |
| Jefe Funga recortado; eventos de castigo, tambor y lanza | **en parte** | Objetos sin usar: Funga Leather / Tree / Sap (119/122/123, JP "material del tambor"), Small Drum (137) y Chief's Spear (140). Sus listas de comprobación de uso son `subScreenCondFalse` (D0SPSUB `D_800FECE8`) y ningún código los da. No hay sprite del jefe. El **diálogo del juicio del jefe sí existe** (AREA10 `MFNT100S` grupo 2 dx 11-18, sin referencia; ver "Jungla (AREA10)" abajo). |
| Anciano de Lumberjack Town, recortado del todo | **confirmado** | Sus fotogramas cenitales están en `SYSTEM/A06100` (fotogramas extra 30..50 del subtipo 0 de AREA11, AREA11/CLUT02 fila 36). Pertenecen a la AREA11 s0 recortada, "Lumberjack Town" ([CIVILIZACION.md](CIVILIZACION.md)). |
| Pez globo y caracola, recortados del todo | sin verificar | |
| El idioma Masakari se aprende mordiendo | **en parte** | El título JP del evento 60 es コトバを吸いとろう！Ⅱ "¡Absorbe las palabras! II" (tabla JP de AREA10), y las frases cortadas del jefe van en parejas: texto traducido más una copia en galimatías ◆★ (JP #191/#195, PAL `?????`). Sin código: 59-61 no se inician, completan ni leen en X010 (ver "Jungla (AREA10)"). |
| Bolsa del Cerdo Marino que da el jefe | sin verificar | |
| Cactus del GoGoCar y una pieza de pista extra | **en parte** | Los cactus están confirmados (ver "GoGoCart cacti / sign" arriba). La pieza de pista no se ha comprobado. |
| Modelo de cartel metálico de la Mansión Encantada | sin verificar | |
| Texturas/sprites de Doodle Rock en el Bosque Subterráneo | sin verificar | |
| Sprites de flor creciendo, maldita y sin maldición | **en parte** | Los fotogramas 27-32 del SPR 0x3B de AREA02 existen, pero la VRAM a la que apuntan está vacía en PAL (ver Perdido). |
| Puerta de hierro en el Castillo de hierro | **en parte** | El evento 100 "Iron Keyhole" tiene icono (fotograma 209), no está en el orden del menú y nada lo inicia. No hemos mirado el sprite de la puerta. |
| Planta rara en la VRAM de la Jungla Masakari | **no encontrada** | Páginas 4 bpp de AREA10 de sus 10 listas de carga renderizadas (tools/vram.py + find_unused_gfx.py): las zonas no muestreadas son palmeras, juncos, flores y texturas de pared de fondos/modelos, más la página B002 ya retirada. No hay ningún sprite de planta suelto. Sin paleta, una planta pequeña podría escaparse. |
| Texto de estado "Thin" / "Fat" | **no encontrado** | No aparece en el texto WFM inglés/español PAL de AREA03/11/16/17/18 ni en CFNT999. No hemos buscado en los gráficos del submenú. |
| Apertura del reloj de la Mansión y gema | sin verificar | |
| Puerta del Cerdo final | sin verificar | |
| Un objeto Tubería antiguo dibujado como Válvula | **confirmado** | El objeto 64 es "Pipe" (MFNT #67). Su icono (A00300 palabras 64..127, celda 64: x 72, y 72) es un volante redondo, es decir una válvula. No se puede probar que sea arte "antiguo". Su icono del inventario sale gris porque `ITEM_ICON_CLUTS[64]` = 1, la fila gris del submenú, igual en PAL y JP (rareza del original, no fallo del port); en AREA08 el objeto es el fotograma 2 de SPR_DATA[3], una rueda de madera marrón (CLUT de área (304,482)). Ninguna fila del bloque del submenú la colorea como el latón oxidado que es, así que el port la dibuja con una **paleta personalizada, no original**: la fila 0 con los índices de la rueda (13, 7, 12, 5, 14, de luz a sombra) cambiados por una rampa ocre oxidada, redondeada a 15 bits y subida a (304,511) (`port_valve_clut` en `port2/runtime/main.c`; `port/fixes.txt`, subScreenDrawItemIcon). El marco del icono queda exactamente como el de los demás objetos: el índice 15 sigue blanco, y las esquinas / el biselado interior, que comparten los índices 13 / 12 / 5 con la rueda, se re-indexan en una copia corregida de la celda (subida encima) a índices libres con los grises de la fila 0. |
| Demo Pure 8: orugas en la VRAM de AREA00; el evento "Motocross Course" empieza normal | sin verificar | Ese disco no está en nuestro conjunto. La tira de títulos de PrePre 8 sí lleva モトクロスコース (ver la entrada del cartel DIRT MOTOCROSS). |
| Demo Pure 9: un cofre del anciano de 10.000 años donde la versión final tiene el de 1000 | sin verificar | Ese disco no está en nuestro conjunto. |

Imagen (no se sube): `item_icons_63_64_65_62_52_grey.png` (Flower Seeds, Pipe, Wine, Rueda de hierro, Esencia lluvia).
