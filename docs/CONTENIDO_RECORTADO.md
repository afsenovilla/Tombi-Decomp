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
| Tablón de anuncios de Baco (AREA05 s1) | `spawnNpcSet09` (X005, tabla `D_80117AC8[GCD.bceed]`) sí crea el cartel del tablón: subtipo 19 = MFNT050 (1,5) RATÓN DESAPARECIDO (al leerlo empieza el evento 46 «¿Dónde está el ratoncito?»), 20 = (1,7) ¡SE NECESITA AVENTURERO! cuando el ratón ha vuelto (`bceed` 2; empieza el evento 49 «Una bebida para adultos»), 35 tras el evento 50. Pero está en (28,-144) a profundidad 90, junto al vecino que lo menciona, y Tombi no puede pisar nada a su profundidad de contacto (la cornisa sobre el muro está a 144, \|dz\| 54 > 45; el suelo, 88 px más abajo): nunca se podía leer y los eventos 46/49 nunca empezaban desde él. (1,6) ¡ENHORABUENA! no tiene fila en la tabla de carteles. `tools/dialogue_refs.py` no veía el cartel creado por código (solo lee las listas de sala). | fixes.txt `updateL4Type1AObject` + runtime/main.c `port_cc_sign`: el cartel baja al suelo bajo el tablón pintado (28,-60, profundidad 0); se lee con Arriba + Cuadrado. `port_cc_sign_text` / `port_cc_sign_next`: con el ratón ya devuelto y el evento 49 sin empezar, sale primero (1,6) y a continuación (1,7) en la misma lectura (y empieza el evento 49 como en el código). | la posición en el suelo; (1,6) antes del primer (1,7) |
| Guardado de La roca de la sirena (AREA06 s1) | MFNT060 (1,0) «La roca de la sirena / ¿Guardar DATOS?» no está en ninguna fila ni sala; el hombre de la carrera (1,1): «la meta... está cerca de la roca de la sirena». AREA06 s1 (playa de la meta) solo tiene el tablón de récords (subtipo 36). | `port_cc_sign` crea un cartel del port (subtipo 0x40+i, texto de `port_cc_saves`) 48 px a la izquierda del tablón de récords; guarda como cualquier cartel de guardado (bloque «La roca de la sirena»). | la posición |
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
| Títulos de evento de PrePre8 少年旅立ち / あれま大きな水たまり | Tira de glifos 16x16 de TIM000 (x 0..224, y 104..232). | Solo texto: una cadena WFM3 de título (el codificador existe). Necesita un evento que la lleve (ver evento 6). En TOMBA.EXE solo los 25 primeros títulos tienen puntero (huecos de lista 0..23); el resto (島のヒミツ, 大停電, 人魚の王を救え, 大団円...) son cadenas sueltas sin datos de área/icono/AP/flag (PREPRE8.md "Event data behind the titles"). |
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
| Hoja de glifos de los túneles: numerales de estilo maya 1-9 (puntos y barras), concha/ojo 0, "=" 10, diez símbolos enmarcados de 24x24 (sol, pájaro, luna, estrella enmarcada, arco, flechas cruzadas, engranaje, anillo de aritos, rejilla, montículo) | `AREA02/B301.000` (GRPX 256x256 palabras en VRAM (768,0), sección 3 de AREA02 = túneles cenitales), 4 bpp en (832,72) 240x72 (página 13, u 0..239, v 72..143), CLUT `AREA02/CLUT04` fila 22 col 7 = (240,502), palabra 0x7D8F. Mismos píxeles en USA y PAL (archivo idéntico) y JP (archivo distinto, mismo bloque); no está en ninguna copia de recarga de `SYSTEM/` ni en los datos de PrePre8; el disco beta no se comprobó (no está local). Sin referencia estática: la palabra del CLUT no aparece en el código ni datos de AREA02 y ninguna parte de los bancos SPR de los túneles (D305) muestrea v 72..143. **Comprobado en el port: nunca se dibuja.** `--dump-draws` en 13 ejecuciones sin ventana desde el selector de depuración a la sección 3 de AREA02 (recorridos por x 982..1677, y -80..-729 de la red de túneles, la salida a la sección 3 de AREA01, y `D_800A60A3` forzado a 1/3/4 para los fondos alternativos de `tunnelSelectBackdropPieces`): 5508 primitivas texturizadas distintas en la sección 3, ninguna muestrea la página 13 en v 72..143 (allí la página 13 solo se usa en v 0..56 y 146..158) y ninguna usa el CLUT (240,502). `tunnelSelectBackdropPieces` (I0NFO3) solo elige punteros a piezas del fondo (entradas 0..20 según la x de la cámara, o la entrada 0x5C/0x60 según `D_800A60A3`), así que por sí sola no llega al bloque. **Barrido completo con `--noclip`** (opción de depuración del port, docs/PORT.md): cinco recorridos en zigzag sin ventana por toda la sección (x del jugador 90..1750, el límite de la sección, es decir más allá de las salidas en x<0x99; y -157..-1567, más allá del -0x540 del mapa y de la salida más baja en -0x4FE; filas a 118 px, x de cámara 199..1400+ así que las 21 tiras del fondo, y de cámara desde su tope -52), uno por valor de `D_800A60A3` = 0..4 forzado dentro de `tunnelSelectBackdropPieces` (un `--poke-every` de 0x800A60A3 no se mantiene: el gancho leía 0 en cada frame, así que las ejecuciones anteriores con 1/3/4 probablemente dibujaron el fondo normal): 2318 primitivas texturizadas distintas con el fondo normal (0 y 4), 202 con 1/2 (entrada 0x5C), 212 con 3 (entrada 0x60); la página 13 de nuevo solo en v 0..55 y 146..157, ningún CLUT (240,502). Con `--noclip` los tiles de disparo y las salidas están desactivados, así que las escenas de evento de los túneles no entran en este barrido (las 13 ejecuciones anteriores cubrieron las alcanzables). **Datos del fondo decodificados:** el bloque de RAM que lee `tunnelSelectBackdropPieces` (0x1DC40 bytes, cabecera de 24 offsets de piezas 0x64..0x1DC40) es una lista de polígonos texturizados (palabra CLUT, palabra tpage, color+código GP0 0x24/0x2C, luego x,y,z,u,v por vértice): 882 polígonos, tpages 0x0C/0x0D/0x0E/0x4D, 16 CLUTs (0x784E..0x7DCF), ninguno 0x7D8F; página 13 solo en v 0..31 y 144..159. **Nueva comprobación estática:** 0x7D8F como palabra de 16 bits no está en MAIN0 ni en ningún overlay salvo un falso positivo (X010 0x37608 es `j 0x8011F63C`); el patrón tpage 13 4 bpp + UV (alto 0x000D/2D/4D/6D, v 72..143, u<240) y las palabras GP0 0xE1 de página 13 solo coinciden con instrucciones MIPS en MAIN0/X001/X006; las coincidencias en ficheros de AREA02 (píxeles GRPX de B001/B101/B200, flujos comprimidos de D004/D304) no son referencias. Ninguna referencia encontrada. Arte sin usar de un puzle de símbolos/números recortado; disposición, código y el texto japonés de la pista (un mecanismo recortado de "suelo de garabatos" con numerales de puntos y barras en base 20): ver "Hoja de glifos de los túneles (AREA02/B301): disposición, código y texto" más abajo. |
| Texto JP de clase D ligado a la Civilización (MFNT110/170/180: balsero, generador, carbón, Dr. Gantetsu, pueblo nuevo) | Sus escenas son los pueblos cenitales cortados. |
| Guion de diálogos de PrePre8 | Encontrado el 2026-10-30 en TOMBA.EXE (índices de glifo u16, `tools/prepre8_text.py`): guion del área 0 (tutorial, Yan, anciano de 100 años) y guion del bosque/enanos (niño enano perdido, Baron bautizado por una enfermera enana, ladrón atrapado). Falta la página de kanji del bosque. Ver docs/PREPRE8.md. |

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
  sprite de un tambor aparte. **Corrección: el jefe Funga sí tiene sprite.** Las listas de carga 1 y 5 de AREA10 llevan la tabla SPR 0x37 (`SPR_DATA[7]`, de `SYS/LD0SYS.BIN`); sus fotogramas 31 y 47-58 (el 53 es un bloque mal recortado) son el jefe: gran tocado como llamas, capa, poses sentado / levantándose / hablando / andando, 56x64 px. Los píxeles están en la página de recarga `SYSTEM/A05700.000` (lista SYS 57, cargada junto a `X010.BIN`; la comparten los Funga de los fotogramas 59+). El extractor los pinta con la CLUT 0x780A (marrón/amarillo); la paleta real se pone en ejecución (un ripeo de la comunidad lo muestra azul, con capa roja y túnica naranja). Ningún código de X010 lee `SPR_DATA[7]` (no hay `lw 0x2E4`, ningún manejador de AREA10 en `tools/animobj.py` usa la tabla 0x37) y ningún guion de animación de X010 recorre solo esos fotogramas: el dibujo se carga pero nunca se pinta (PAL; JP/beta sin comprobar). **Corrección: lo de la anguila era demasiado amplio.** Sobrevive un guion
  de 3 fotogramas sin referencias: JP X10 celda 0x8012DEB8 → 0x8012E0E0 = fotogramas 33, 34, 35 de la SPR 3, 8 tics
  cada uno, el último fijo (0x8008), sin bucle; PAL X010 tiene los mismos 24 bytes en 0x80131F1C (celda 0x80131CE8).
  SiergiejW decodificó los fotogramas como una anguila de 32x16 con su propia paleta (textura en `AREA10/B003.GAM`);
  nosotros comprobamos el guion y que nada lo apunta en las dos versiones, no los píxeles (SiergiejW, TombaEdit docs/jp-retail-unused-research.md).
- **Objetos frente a la anguila (SLPS JP, igual en PAL donde se ha mirado):** el objeto 108 (Anguila eléctrica) usa el
  descriptor de mundo 0 (tabla 0x80077BE4 → punteros 0x80077CAC; el 0 es la baya genérica), como 59 Guantes de goma,
  109 Agua negra y 119 Cuero Funga; ninguno de los 97 descriptores apunta a la celda de la anguila. Su condición de
  menú es la lista 22 (D0SPSUB PAL `D_800FECE8[108]` = 22 → 0x800FF3E8: AREA18 s2, H del jugador 128..192), la misma
  que el Agua negra: un sitio donde usarla, no donde aparece. El Tambor Funga 125 (descriptor 94) y el Tambor pequeño
  137 (61) comparten la celda 0x80011E8C (un solo sprite) y sólo cambian la caja (`08 10 08 10` frente a
  `0C 0C 0C 0C`); la condición del tambor es la lista 41. Madera 91, Árbol Funga 122, Savia Funga 123 y Lanza del
  jefe 140 tienen descriptor propio (75, 73, 72, 74) (SiergiejW, TombaEdit docs/jp-retail-unused-research.md).
- **Las líneas JP de uso nombran dos huecos de relleno:** en el bloque JP de uso (AREA10 A003 #n = objeto n) el #133 es
  １ＵＰをつかった y el #134 ゲージＭＡＸ　＋１をつかった (los dos «Chick»: 1UP y medidor máx. +1), #131 地下迷宮の地図
  (mapa del laberinto subterráneo), #147 石炭 (carbón); #77-#86 (Math Bead 11.., Number Balls) dicen エラーです. Todos
  tienen la lista 0 (nunca se pueden usar); sólo el 77 tiene descriptor de mundo (89). Sólo texto, no hay código del
  efecto (SiergiejW, TombaEdit docs/jp-retail-unused-research.md).
- **La reacción de calambre es sólo PAL:** `playerStepHurtKnockback` de X010 PAL (0x800FD358) pone la animación 0x50
  con el motivo 3 (+0xD1); la rutina JP (X10 0x800FCE48, 1.492 bytes) nunca lee +0xD1 y carga siempre la animación
  0x39 (`li a2, 0x39` en 0x800FD01C) (SiergiejW, TombaEdit docs/jp-retail-unused-research.md).
- **Sin revisar:** lista completa de funciones muertas de X010 (el escaneo simple de punteros da demasiado ruido) y subtipos colocados por sala.

## Notas de Folkin — verificación (afirmaciones fuera de Civilización)

Las afirmaciones sobre Civilización se comprueban en [CIVILIZACION.md](CIVILIZACION.md) ("Notas de Folkin —
verificación"). Estas son las demás, comprobadas solo cuando era barato. **Sin verificar** significa que no lo
hemos comprobado.

| Afirmación | Veredicto | Pruebas |
|---|---|---|
| Adivino verde, recortada del todo | **en parte** | El evento 89 tiene icono propio (fotograma 227, "adivino verde", ver Iconos de evento), pero no está en `SUBSCREEN_EVENT_ORDER` y ningún código lo inicia. La versión final tiene "The Red Fortune Teller" (CFNT #69) y "The Blue Fortune Teller" (#125). El objeto sin usar 90 es una carta "para las tres hermanas". No hemos mirado sprites. |
| Jefe Funga recortado; eventos de castigo, tambor y lanza | **en parte** | Objetos sin usar: Funga Leather / Tree / Sap (119/122/123, JP "material del tambor"), Small Drum (137) y Chief's Spear (140). Sus listas de comprobación de uso son `subScreenCondFalse` (D0SPSUB `D_800FECE8`) y ningún código los da. El **sprite del jefe sobrevive** sin uso (SPR 0x37 de AREA10, fotogramas 31 y 47-58, página `SYSTEM/A05700.000`; ver "Jungla (AREA10)"). El **diálogo del juicio del jefe sí existe** (AREA10 `MFNT100S` grupo 2 dx 11-18, sin referencia; ver "Jungla (AREA10)" abajo). |
| Anciano de Lumberjack Town, recortado del todo | **confirmado** | Sus fotogramas cenitales están en `SYSTEM/A06100` (fotogramas extra 30..50 del subtipo 0 de AREA11, AREA11/CLUT02 fila 36). Pertenecen a la AREA11 s0 recortada, "Lumberjack Town" ([CIVILIZACION.md](CIVILIZACION.md)). |
| Pez globo y caracola, recortados del todo | sin verificar | |
| El idioma Masakari se aprende mordiendo | **en parte** | El título JP del evento 60 es コトバを吸いとろう！Ⅱ "¡Absorbe las palabras! II" (tabla JP de AREA10), y las frases cortadas del jefe van en parejas: texto traducido más una copia en galimatías ◆★ (JP #191/#195, PAL `?????`). Sin código: 59-61 no se inician, completan ni leen en X010 (ver "Jungla (AREA10)"). |
| Bolsa del Cerdo Marino que da el jefe | sin verificar | |
| Cactus del GoGoCar y una pieza de pista extra | **en parte** | Los cactus están confirmados (ver "GoGoCart cacti / sign" arriba). La pieza de pista no se ha comprobado. |
| Modelo de cartel metálico de la Mansión Encantada | sin verificar | |
| Texturas/sprites de Doodle Rock en el Bosque Subterráneo | **confirmado (corregido)** | Las rocas de garabatos existen: cinco placas distintas de puntos y barras (2, 5, 18, 139, 59) en páginas de textura de salas, dos de ellas colocadas en el juego final como modelos 3D decorativos (Bosque de los Enanos AREA01 s1/s3 = 2, Mansión Encantada AREA04 s0/s1 = 5). Ver "Rocas de garabatos (らくがき岩): las placas existen". |
| Sprites de flor creciendo, maldita y sin maldición | **en parte** | Los fotogramas 27-32 del SPR 0x3B de AREA02 existen, pero la VRAM a la que apuntan está vacía en PAL (ver Perdido). |
| Puerta de hierro en el Castillo de hierro | **en parte** | El evento 100 "Iron Keyhole" tiene icono (fotograma 209), no está en el orden del menú y nada lo inicia. No hemos mirado el sprite de la puerta. |
| Planta rara en la VRAM de la Jungla Masakari | **no encontrada** | Páginas 4 bpp de AREA10 de sus 10 listas de carga renderizadas (tools/vram.py + find_unused_gfx.py): las zonas no muestreadas son palmeras, juncos, flores y texturas de pared de fondos/modelos, más la página B002 ya retirada. No hay ningún sprite de planta suelto. Sin paleta, una planta pequeña podría escaparse. |
| Texto de estado "Thin" / "Fat" | **no encontrado** | No aparece en el texto WFM inglés/español PAL de AREA03/11/16/17/18 ni en CFNT999. No hemos buscado en los gráficos del submenú. |
| Apertura del reloj de la Mansión y gema | sin verificar | |
| Puerta del Cerdo final | sin verificar | |
| Un objeto Tubería antiguo dibujado como Válvula | **confirmado** | El objeto 64 es "Pipe" (MFNT #67). Su icono (A00300 palabras 64..127, celda 64: x 72, y 72) es un volante redondo, es decir una válvula. No se puede probar que sea arte "antiguo". Su icono del inventario sale gris porque `ITEM_ICON_CLUTS[64]` = 1, la fila gris del submenú, igual en PAL y JP (rareza del original, no fallo del port); en AREA08 el objeto es el fotograma 2 de SPR_DATA[3], una rueda de madera marrón (CLUT de área (304,482)). Ninguna fila del bloque del submenú la colorea como el latón oxidado que es, así que el port la dibuja con una **paleta personalizada, no original**: la fila 0 con los índices de la rueda (13, 7, 12, 5, 14, de luz a sombra) cambiados por una rampa ocre oxidada, redondeada a 15 bits y subida a (304,511) (`port_valve_clut` en `port2/runtime/main.c`; `port/fixes.txt`, subScreenDrawItemIcon). El marco del icono queda exactamente como el de los demás objetos: el índice 15 sigue blanco, y las esquinas / el biselado interior, que comparten los índices 13 / 12 / 5 con la rueda, se re-indexan en una copia corregida de la celda (subida encima) a índices libres con los grises de la fila 0. |
| Demo Pure 8: orugas en la VRAM de AREA00; el evento "Motocross Course" empieza normal | orugas **no encontradas**; título Motocross **confirmado** en la tira | Según Folkin, "Pure 8" es la misma versión que nuestra PrePre 8 (`docs/PREPRE8.md`). Las 11 páginas TIM de AREA00 renderizadas: TIM000 solo tiene sprites de Tomba, cerdos y el ave/dragón gris; el resto son fondos/texturas/fuentes. Ninguna oruga (sin búsqueda de paletas). La tira sí lleva モトクロスコース. Arranque del evento sin probar. |
| Demo Pure 9: un cofre del anciano de 10.000 años donde la versión final tiene el de 1000 | sin verificar | Ese disco no está en nuestro conjunto. |

Títulos de evento de PrePre 8 / "Pure 8" (lista de Folkin, detalle en `docs/PREPRE8.md`): 29 títulos que no están en la versión final. Huecos probables, sin verificar: Five Chests (Forgotten Things / Key) -> Lost and Found (el título de área 0 魔女の忘れもの encaja), Great Thief in the Underground Prison -> antes de The Great Escape, Tumbling Down / Again -> Where did I come from? / Lava Caves, Where the Light Falls -> evento 32 (trompetas). Confirmados como glifos en la tira de TIM000: Young Boy Sets Off, Large Puddle, Blow Away the Fog, Meeting the 100 Year Old Man, Elder; los de cofres, ladrón, caída y luz no están en la tira. Actualización 2026-10-30: la tabla completa (167 títulos, con cofres, ladrón, rodar, luz y muchos de áreas posteriores como 島のヒミツ1-5, 大停電, 人魚の王を救え, 「大団円1-3」) está decodificada desde TOMBA.EXE, ver docs/PREPRE8.md.

Imagen (no se sube): `item_icons_63_64_65_62_52_grey.png` (Flower Seeds, Pipe, Wine, Rueda de hierro, Esencia lluvia).

## Barrido de zombis (PAL)

Barrido estatico de MAIN0, X000..X019 (no existen X007/X012/X015) y los 8 overlays SYSTEM (D0SPSUB 1-3, G0OVER, I0NFO 1-3,
O0PTSUB0), con script (escaner desechable, no incluido): cada simbolo `type:func` de `config/symbol_addrs_*.txt` se busca en los
binarios crudos como `jal`/`j`, puntero de 32 bits (tablas) o pareja `lui`+`addiu/ori/lw..`; despues se cierra la alcanzabilidad en el
grafo de llamadas (una funcion solo llamada por muertas esta muerta). Raices: cualquier referencia desde datos, la primera
funcion de cada overlay y punteros de MAIN0 al espacio de overlays. Un nombre cuenta solo si **todas** sus copias (la libreria del
jugador esta enlazada en cada overlay) estan muertas. Limites: referencias construidas en ejecucion (`jalr` calculado, offsets)
son invisibles; "muerta" = sin referencia estatica.

**Resultado: 575 nombres de funcion sin referencia viva; 263 son libreria PsyQ (libgpu/libcd/libspu/libgte...).** De las otras 312,
89 ya estaban marcadas "unreferenced / no caller" en `notes/names.md`; 223 no. **Veredicto: todas son codigo muerto, ninguna es
contenido recortado.** Nada de este barrido es restaurable como funcion del juego.

| Clase | N | Que (nuevo = sin marcar en names.md) | Confianza |
|---|---|---|---|
| Casos `scriptOp*` sueltos + el ejecutor antiguo | 53 | MAIN0 `scriptOp*` (49: 0x80038238..0x8003AF94, `opNop`) y `runScript` 0x800391B0, `runScriptContext` 0x8003926C, `scriptRunOpcode` 0x80039150, `scriptLoadHeader` 0x80038B04: los casos del interprete de scripts WS (magia `"WS"` 0x530057, 64 objetos) como funciones aparte mas una copia anterior del ejecutor. El ejecutor vivo (codigo tras 0x80039338) llama directo a `execCoreOpcode` / `execGameOpcode`, que integran los mismos casos. `scriptOpSetObjField6B.c` ya dice "a separate function here". | verificado sin refs; "copia" = lectura del C |
| Copias `step*` en overlays | ~60 | El compilador integro el switch de pasos en `updateX`; la copia `stepX` independiente se quedo en el binario: `stepWindDiesEvent` (X003 0x80133938), `phoenixFavoriteCompleteScene`, `stepUnderwaterPigBagStart`, `stepBucket`, `stepPaintingSlotHintTalk`, `stepTownHallWorkerTalk`, `stepScaredGoldVolunteerTalk`, `stepSoakedPhoenixGuideTalk`, `stepTreasureChestGuardTalk`, `stepTenThousandYearElderTalk`, `juiceManJoinFightStep`, `overheadActor*Step` (I0NFO)... Mismos ids de dialogo que su gemela viva. | verificado (C leido en 12) |
| `check*Trigger` / `spawn*` de un uso | ~25 | `checkWindDiesEventTrigger` (X003, evento 0xAA), `checkKeyHolesTrigger`, `checkBigKeyPaintingPanels`, `spawnBigKeyPiece`, `spawnMermaidForMathBeads`, `spawnHauntedPaintingObj` (X004), `spawnLostSeedKidDwarf`, `initPuppyRumorNpc`, `playPuppyBarkNearby`, `checkPuppyRumorNpcTalk` (X001), `spawnGoldenFlowerIfAffordable` (X002), `spawnBaccusVillagePigShotPieces`, `spawnFlowerForestPigShotPieces` (X014), `spawnCharlesNpcAlt` (X000, Charles, tipo 0x15, salvo 0x8009CEBD >= 6): la misma logica esta en el `update*` vivo que la integra; el NPC/evento al que sirven esta en el juego. | verificado en los 6 leidos; el resto por nombre/forma: suposicion |
| Ayudas de la libreria del jugador (X000.. todas) | ~35 | `playerTiltTowardSlope`, `playerAirControlTimed`, `setWeaponAimFromPad*`, `playerBiteEntry*`, `playerTryStartJump`, `playerGrabSpinEntry`... muertas en todos los overlays (casi todas copias integradas de la misma maquina de estados). | verificado sin refs |
| Restos del motor en MAIN0 | ~60 | `dispatchAreaActor{Init,Update,Draw1,Draw2,Spawn}`, `dispatchAreaNpcInit`, `dispatchAreaCollisionHook{A,B}`, `dispatchAreaUnlayeredHandler`, `spawnUnlayeredObject*`, `onOverlap*`, `tryAttach*`, `modelDrawF3/G3`, `initMsgBoxA/B`, `showMessageBoxTimed`, `enqueueCdRead`, `lzDecompressToBuffer`, `getEventState`, `computeRotatedQuad`, `spawnItemLinked`, `stubCamera1..3`. Envoltorios por area y ayudas cuyos llamadores se optimizaron o nunca se escribieron. | verificado sin refs; papel por nombre |
| Manejadores vacios | 5 | `updateL4Type08Object`, `updateL8Type07Object`, `stubNop*` (`i0nfo2`, X0nn), `stubFreeObj*`: cuerpos vacios. | verificado |
| Un posible resto (suposicion) | 1 | **`updateWaterCurrent` (X010 0x8012E9E0, 388 B)**: objeto de la jungla que fija la deriva de "corriente" (`GVARS.driftH=0`, `driftY=-0x40`), pone un nivel de agua segun seccion / areas purificadas (`-0xEA`, `-0x50`, `-0x3B8`) y devuelve al jugador nadando (pasos 0x3E, 2, 0xD) al paso 0x3D en ese nivel. Sus companeras `a10SetPlatformY` y `a10ClampPlayerAboveY` tambien estan muertas. Sin llamada ni puntero en PAL. El agua viva de la jungla se resuelve en los pasos del jugador; puede ser un objeto de nivel de agua anterior. No se asocio a ningun objeto colocado. **Revisado (no restaurado):** tambien muerta en la JP retail (X10 `func_8012D004`/`8012CEEC`/`8012CF3C`, sin puntero en X10.BIN) y la beta 1998-02-03 no tiene X10, asi que ninguna version muestra la llamada. Esta superada: el `playerUpdateWaterLevel` vivo (en todos los overlays del jugador) ya hace lo mismo cada frame para AREA10 secciones 1/5 (`w56=-0xE4`) y 3/7 (`-0x8C` purificada / `-0x3C0`) y fija la misma deriva, `playerStepSwimUnderwater` (0x3E) sale a la superficie en `+0x56` y `playerHandleHazardContact` pasa el paso 2 a nadar 0x3D. Sus niveles antiguos (`-0xEA`, `-0x50`, `-0x3B8`) no coinciden con la superficie viva (52 px en 3/7 purificada), asi que conectarla pegaria a Tombi a una segunda linea de agua invisible. Objeto de nivel de agua anterior, sustituido por el codigo del jugador; no se conecta en el port. | papel: superada |

**No nuevo pero confirmado:** los tres `topdownA11/A16/A17ActorDialogueBank2` de I0NFO2 (pueblos de la Civilizacion recortados, mensajes (2,4)/(2,5), (2,0)/(2,2), (2,5..10)) y `topdownA05/A08/A11/A16/A17/A19Scenery*` no tienen llamador: el codigo de escenas de los pueblos recortados (ver Perdido, docs/CIVILIZACION.md).

### 2. Ranuras de manejador de objetos que ninguna sala coloca
`tools/objects.py list --all-slots` (6533 registros) frente a las tablas de MAIN0. Ranuras sin colocar que tampoco crea nadie
(grep de `allocObjectLayerN` + `->type =`): capa 4 tipo 8 (`updateL4Type08Object`, vacio), capa 8 tipo 7 (vacio), capa 2 tipo 0x30
(`updateL2Type30Object`: despachador por area al decorado de pueblo de I0NFO2 de las areas 5/8/11/16/17/19; esas salas son los
pueblos recortados), capa 5 tipos 9/0xA (`updateL5Type09Item`, `updateL5Type0AItem`: objetos objeto; se crean con `ObjSpawn` con tipo
variable, no se puede probar que no se usen), capa 7 tipos 5/10/13 y capas 2/4 tipos 0x19, 0x2D, 0x2E, 0x45, 0x4C, 0x41
(`overheadActorHandler`, `topdownItemSpotHandler`, `topdownTalkSpotHandler`, `tunnelActorHandler`, `tunnelTalkSpotHandler`,
`tunnelStaticObjHandler`: secciones cenitales / tuneles del AREA02 de I0NFO / I0NFO3, colocadas por los objetos de seccion de esos
overlays). **Ningun manejador sin colocar tiene contenido real.** La capa 8 no tiene ningun registro de colocacion (sus dos
manejadores vivos los crea el codigo).

### 3. Ids de evento
Nada que anadir a "Eventos sin titulo PAL (0xFF)" y "Eventos sin usar": los 19 sin referencia y los 6 ligados a la Civilizacion quedan como estan listados.

### 4. Dialogo
`tools/dialogue_refs.py --resolve` (resolutor de argumentos calculados, ~3 min) toma los 30 grupos que marca el escaner literal y resuelve el `(k, dx)` calculado de cada llamada a las funciones de mensaje del C emparejado (sin los `*_slps01144.c` JP): literales de bytes de estado (`DAT_8009cebd/e/f`; los contadores con ++ quedan como "quiza"), tablas leidas de los binarios (`A00_TUTORIAL_JUMP_MESSAGE_IDS`, profesor de natacion `D_8012F3C8` = 27..32 segun la configuracion de botones, `D_80125E20`, `TOPDOWN_A16_ACTOR_DIALOGUE_IDS`, las dos tablas de puntos de charla), las tablas de secuencias de escena (`e[1]` / `c->arg`), la tabla de carteles de MAIN0 `D_80079EBC` limitada a los subtipos que las salas colocan de verdad (`objects.py`), los overlays I0NFO/I0NFO2/I0NFO3 (barrido de jal + C; I0NFO2 se atribuye a su ciudad por el nombre de la funcion) y las macros `CALL/SPAWN`. Resultado para los 30: **7 grupos son totalmente alcanzables** (MFNT040/080/090/100 g1, MFNT020 g5, MFNT160 g3, MFNT180 g1), 12 en parte y **11 sin ninguna linea alcanzable** (abajo). Indeterminado: MFNT000 g7 (contador `DAT_8009cebf`; las lineas del barquero (7,3..5) ya estan arriba), el resto de MFNT000/001 g1 (tabla de secuencias de `updateHoodedOldMan` sin resolver), MFNT170 g5 dx 1/7/10/15/18 (ternarios sobre `D_8009CFA5`). Limite: el op 0x85 de WSS con argumentos variables (AREA15: 0140 x13, 0200 x27, 0201 x3, 0210 x1, SCR0100 x1) es invisible, asi que las filas de MFNT020 son "sin referencia estatica", no prueba.

| Lineas nuevas sin usar (PAL; ES / EN) | Evidencia | Sugiere |
|---|---|---|
| MFNT050 (1,5) "RATÓN DESAPARECIDO..." / "MISSING MOUSE...", (1,6) "¡ENHORABUENA! ..." / "CONGRATULATIONS!...", (1,7) "¡SE NECESITA AVENTURERO!..." / "ADVENTURER WANTED! The pump at the lake is broken..." | `D_80079EBC` tiene entradas para (1,5) (subtipos 14, 19) y (1,7) (20), pero ninguna sala coloca esos subtipos; (1,6) lo pisa la tabla de ayudas por configuracion de botones | **restaurado (port, --cut-content)**: el cartel del tablón lo crea el código (`spawnNpcSet09`, subtipos 19/20) pero fuera de alcance; bajado al suelo, (1,6) añadido antes de (1,7) (ver Restaurado) |
| MFNT010 (1,4) "Bosque de setas ¿Guardar DATOS?" / "Mushroom Forest Save DATA?", (1,5) "Tobogán de hoja" / "Leaf Slider" | (1,4) no esta en ninguna entrada de la tabla; (1,5) solo en entradas sin colocar; AREA01 s1 no tiene cartel de guardado | no restaurado: las cinco salas de AREA01 ya tienen cada una su cartel de guardado con nombre (Bosque de las 100 flores s0/s1, Muelle inestable, Torre de vigilancia, Plaza de la caridad); Bosque de setas / Tobogán de hoja nombran lugares sin sala aquí (la playa del Bosque de setas de las frases recortadas del barquero; AREA07, que comparte MFNT010/X001, no está en el disco) |
| MFNT060 (1,0) "La roca de la sirena" / "The Mermaid's Singing Rock Save DATA?"; MFNT020 (1,2) y MFNT190 (1,0..2) "Ciudad oculta / Ciudad de Enanos / Cabaña Enano Mayor" / "Hidden Village / Dwarf Village / Dwarf Elder's Hut" | AREA06 coloca un cartel (subtipo 36), AREA19 ninguno; el (1,2) de AREA02 no se coloca en ningun sitio | roca de AREA06 **restaurada** (port, --cut-content; ver Restaurado). Guardados enanos no restaurados: ninguna sala de AREA02 s0-s4 ni de AREA19 tiene un cartel al que enganchar la creación (el único de AREA02 es el (1,3) de s5), la cabaña del anciano no tiene sala lateral propia, y AREA19 (la aldea purificada, que usa la X002 de AREA02) no carga desde la selección de depuración en el port (fallo en `queueLoadList`), así que no se podía comprobar una posición |
| MFNT020 (6,11) "¿Qué? ¿Querías preguntarme algo? ¿Un cable?..." / "What? Did you wanna ask me about something? A wire? ..not just an ordinary but an invincible wire?", (6,18) "¡Eh, hola! ¡Echa un vistazo! ¡El camino está despejado!" / "Hi, there! Take a look! The path is cleared!", (6,19) "¡Lo he acabado!! ¡Me merezco una buena palmada!" / "I finally finished it!! I deserve a big ol' pat on the back!" | `tunnelActorOpenDialogue` (I0NFO3) solo abre (6,0..10,12,15,17) | lineas del enano excavador de una escena recortada de "tunel terminado" y de una pregunta recortada sobre el cable (cf. la cadena del cable irrompible) |
| MFNT020 (8,0..2) "¡El cable del ladrón está roto!" / "The thief's wire broke!", (8,2) "Suena como si algo se moviera..." / "It sounds like something moved..." | ninguna llamada con k=8 en X002 / I0NFO / I0NFO3 | escena recortada del cable del ladron (AREA02) |
| MFNT190 (3,7) "La cesta de comida... Has conocido a mi padre..." / "That basket of food... You must have met my father...", (3,8) "¿Te has comido mi almuerzo?" / "What? You ate my lunch?" | `topdownA19ActorOpenDialogue` (I0NFO2) usa (3,10..13) y `topdownA19ActorTalkStep` (3,4)/(3,6) | pareja de lineas recortada de un habitante de la ciudad oculta (AREA19) |
| MFNT170 (4,0) "¿Me has encontrado otra vez, eh?..." / "Found me again, eh? Is there anything you can't find?!" | sin llamador | PNJ recortado de la Ciudad de hierro (Civilizacion) |

Relleno, no contenido (`···???` / `..........???`): MFNT020 g4, g6 dx 13/14/16, MFNT110 g4 dx 8/9/12-15/17, MFNT170 g3 dx 4/6/7/9/10, MFNT030 (1,1), MFNT110/130/140 (1,0). MFNT000 (8,0) es el cartel de motocross ya listado; los nombres de guardado de Ciudad de Piedra / de hierro (MFNT160/170 (1,0)) son de las ciudades recortadas.

### 5. Ficheros y listas de carga
Cada fichero de `game/disc/fs` frente a las nueve tablas de ficheros MAINn: 2199 de 2287 estan listados; 77 no (el resto son los ejecutables MAIN0..8, SCES_013.31, SYSTEM.CNF):
- **`AREA19/X019..X819.BIN` y `SYSTEM/X019..X819.BIN`**: ya listados (nunca se cargan).
- **`I0NFO2.BIN` ... `I8NFO2.BIN` en AREA11, 16, 17, 18**: identicos byte a byte a `SYSTEM/I?NFO2.BIN` (sha1 comprobado en 0 y 8): copias del overlay de pueblo que ninguna tabla lista, porque los pueblos cargan `SYSTEM/I?NFO2`. Nuevo pero inocuo: resto de cuando los overlays de pueblo estaban en las carpetas de area.
- **CLUT sin listar: `AREA11/CLUT01,04,05`, `AREA16/CLUT01-03`, `AREA17/CLUT01,07-11`, `AREA18/CLUT01` (13 ficheros, 4457 B) y `AREA15/CLUT01-09` (9 ficheros, 161 B)**: los 13 son identicos entre si (prefijo sha1 `fa9c666a`), los 9 tambien (`2aca3f83`): paletas de relleno / plantilla, sin contenido. Las CLUT usadas de esas areas (AREA11/02,03; AREA16/04-07; AREA17/02-06; AREA18/02,03) son las que usa docs/CIVILIZACION.md.
- `ZZZ/DUMMY.DAT`: relleno.
Asi que **no hay recurso grafico ni sonoro sin cargar** mas alla de lo conocido (X019, los sets de la Civilizacion).

### Resumen
El disco PAL no guarda ninguna funcion zombi oculta que el inventario anterior no hubiera visto: el codigo muerto son restos de
integracion/portado y los datos muertos son relleno. Lo unico que merece una segunda mirada es `updateWaterCurrent`; el barrido de
dialogos (4) es donde un resolutor de `(k, dx)` calculados aun podria encontrar lineas nuevas.

## Hallazgos de codigo de PrePre Vol. 8 (2026-10-30)

Fuente: [PREPRE8.md](PREPRE8.md), "Functions vs PAL". Nada de esto esta en PAL, JP retail ni la beta de 1998.

| Hallazgo | Evidencia | Estado |
|---|---|---|
| Tarjeta `TO BE CONTINUED...` de fin de demo | cadena 0x80010228, dibujada en (20, 26) durante 360 fotogramas por la tarea 0x80021050 cuando el byte 0x800DE4A3 == 2 | perdido (solo codigo; el disparador no esta trazado) |
| Generador de oleadas de enemigos por scroll en area 1 sala 0 | 0x80083D88 imprime `enemy=%01d`; 8 umbrales de scroll X (0x800AF1A4), tabla de retardo aleatorio (0x800AF194); las otras salas no lo generan | perdido (tipo de enemigo sin identificar) |
| Vista de depuracion de camara con waypoints (`area_pos`, `pos_num`, `tar_x/y/z`) | 0x800268C0 | solo depuracion |
| Menus de prueba: SOUND OUTPUT (tipos de reverb), MUSIC PLAY, MEMORY CARD, CDROM LOAD | codigo muerto desde 0x800854B4, sin llamador | solo depuracion, no es contenido |
| Modos de demo del titulo `PLAY DEMO`, `RANKING DEMO` | cadenas 0x80010174/0x80010180, llamadas desde la maquina de estados 0x8001ECD4 | perdido (no hay datos de demo en el disco) |
| Salidas del pueblo a las salas 4 (DATA003, casas de paja/torre de vigia/rio) y 5 (DATA004, casa con letrero de cerdo) | registros de entrada `D_800A273C` (sala 0 -> sala 4, sala 1 -> sala 5, y vuelta) + disparadores por posicion en el control del jugador 0x80028B50 (estados 4/5/9/10); el unico letrero de DATA004 dice 「ふるいけ」 | perdido en versiones posteriores con estos modelos; alcanzable segun el codigo (sin emular). ふるいけ existe en el juego final, asi que DATA004 es una ふるいけ temprana, no un lugar perdido; DATA003 no tiene nombre ni NPC ni nada que lo ligue a una civilizacion |

No encontrado (2026-10-30, no volver a buscar): ninguna linea de dialogo ni registro de sala inicia los eventos 20 (5つの箱の
忘れもの) o 22 (地下ロウの大ドロボウ), ni ningun id fuera de 1-4, 6, 9-11, 13; DATA003/DATA004 no generan ningun tipo de objeto
ausente del pueblo; no hay selector de salas de depuracion en TOMBA.EXE; el area 1 no tiene lista de carga. Detalles:
PREPRE8.md "Event ids from data, and the rooms".

## Busqueda en el codigo, 2 de noviembre de 2026 (Torre de flores, versiones, scripts de animacion sin usar)

Solo analisis estatico: escaneres desechables y desensamblado (capstone), sin commitear. **[V]** verificado en los binarios,
**[I]** inferido. Nada de esto esta restaurado en el port.

### La zona de la Torre de flores (pregunta de la comunidad: "¿hay mas codigo?")
- **El interior de la Torre de flores es AREA13 seccion 0** [V]. En PAL X001 `updateAreaScroll` (0x80116990), en el caso de la seccion 4
  (Plaza de la caridad, AREA01 s4): mientras `GAME.playerAP > 999999` **y** `GCD.bce48 != 0` (= evento 164 "La Torre de flores"
  iniciado, byte 0x8009CDA4 + 0xA4), si Tombi esta en x 0xB5C..0xB7B, el juego lo hace caminar hacia el fondo (x 0xB6C, profundidad >= 0x8D1).
  A los 60 fotogramas pone `nextArea 0x0D / seccion 0 / punto 0`. Es la unica escritura de `nextArea = 13` del juego (revisamos
  todas las de src/). El limite es la constante 999999 (`lui 0xF` + `0x423F`), asi que hacen falta **1.000.000 PA**. JP X01
  tiene la misma constante (0x80116238) [V]. La pista en el juego es la frase del guardian MFNT010 (2,126): "si quieres entrar en la torre,
  ¡vas a necesitar muchos PA!".
- **La sala tiene un solo objeto** [V] (`tools/objects.py list --area 13`): un objeto de item, capa 5 tipo 4, subtipo 132 en
  (160, -470). Su recompensa es la 1 = `rewardItem`, es decir, el item 0x84 **Pez sagrado** (MFNT #135). No hay NPC, ni enemigo, ni evento.
  En s0, X013 `updateAreaScroll` (0x80115FB4) solo espera a que Tombi entre en la salida de x 0xA0 y lo devuelve a
  AREA01 s4, punto 4.
- **No hay mas codigo de la torre** [V]. La parte de area de X013 son 27 funciones y 10 KB en total (scroll, paso de llevar al jugador,
  sprites de fondo, Comida de pez poderoso, la secuencia de s1 y las estadisticas del final). Nuestro escaneo de funciones muertas de X013 solo
  encuentra sin referencia el auxiliar `func_801174C4`, que llevan todos los overlays. `MFNT130` tiene 208 lineas: el grupo 0 (sistema/items) mas un
  relleno `..........???` (#207), sin ningun dialogo. La seccion 1 es el final (evento 0 "El brazalete del abuelo", `S_END`).
  El nombre `AREA13_PIGISLAND` de las fuentes viene de las constantes de psx_tomba. En los datos, el area es "cima de la Torre de flores + final".
- Restaurable: no falta nada. El titulo del evento en la beta, "The Million Tower" (docs/BETA_980203.md), alude a esta
  puerta del millon de PA [I].

### Versiones: codigo presente en una sola
- **USA (SCUS-94236) frente a PAL** [V]: pasamos `tools/xmatch.py` + `tools/fn_vs_pal.py` por el exe USA y los 17 `SYSTEM/Xnn.BIN` (base
  0x800E8028, la misma que PAL). Todas las piezas de >= 300 B con similitud < 40 son bloques de datos. **No hay codigo exclusivo de USA en los
  overlays de area.** No comparamos INFO/DSPSUB/GOVER/OPTSUB.
- **JP final dibuja las estadisticas del final con la fuente de depuracion** [V]: en JP `X13.BIN`, `updateSceneSequence` (0x801162DC) llama a
  `sprintf` + `drawDebugText` con `TIME  %3d:%2d`, `AP    %8ld`, `EVENT %3d / %3d` (fichero +0x2B718). PAL/USA dibujan los mismos
  numeros con sprites de glifos (`drawGameStats` 0x801179B4). El exe JP tambien escribe `TRY AGAIN...`, ` GAME OVER ` y `NOW LOADING`
  con `drawDebugText` (0x8001A1D8, 0x8001C0C4, 0x8001B864). PAL usa graficos para ellos. Mas detalle en "La fuente de depuracion" abajo.
- **Codigo por area de JP** [V el metodo, I el veredicto]: las piezas de baja similitud que leimos (X04 0x80117938, X03 0x8011D630, X10
  0x8011C81C, X09 0x8011F65C, X01 0x80123014, X06 0x80117AB4) son codigo de jugador / colision / bloques de modelo reescrito, con las mismas llamadas.
  No hay ninguna funcion exclusiva de JP salvo las estadisticas de X13.
- **Correccion a docs/BETA_980203.md ("Glyph scene object", beta 0x8002F084):** la funcion esta en PAL. Es MAIN0
  0x8002CF40..0x8002DC50 (3344 B), la entrada 0x80079C14 de la tabla del pool de objetos (justo despues de `dispatchMsgBoxHandler`). Llama a `loadMessageGlyph`,
  `freeMessageGlyphs` x4, `GetClut`, `openMenuScreen`, `SfxPlay2` x3 y las llamadas de matrices [V]. En USA esta en 0x8002FA24. Se paso por alto porque
  `notes/functions_main0.csv` y la tabla de simbolos se la saltan (`clearTalkPose` acaba en 0x8002CF40), asi que no tiene nombre. No es codigo recortado.

### Funciones y handlers muertos (segunda pasada)
Nuestro grafo de llamadas reproduce el barrido de zombis de arriba. De las funciones de parte de area de >= 150 B que lista, 66 no estan mencionadas en este fichero. Todas menos una
son copias de switch de pasos con un gemelo vivo (p. ej. X005 `mansionKeyHintTalk` = la misma linea (2,0x33) que `stepBacoTownNpcTalk`;
X016 `stepJuiceSellerTalk`, `stepBrokenStoneGearsTalk`; X004 `stepBigKeyPainting`; X001 `stepRaceEvent`) o auxiliares. La excepcion:
- **X001 `segmentedCrawlerWander` (0x801334DC, 1352 B)** [V]: una copia anterior y mas simple de la rutina de movimiento de la oruga
  verde. No tiene referencia en PAL, USA ni JP. Detalle, imagenes y prueba en el port en "Rutas de patrulla sin usar" abajo.

### Scripts de animacion sin referencia
Buscamos entradas de animacion de 8 bytes (formato de tools/anim.py) que acaben en 0x8000 o en un salto 0x4000 hacia atras, y nos quedamos solo con las que no
tienen ningun puntero ni par lui/addiu. Todos estan **entre scripts referenciados de las tablas del mismo objeto**. **Renderizados (seguimiento):** corrimos el
port sin ventana hasta la sala del dueño con la seleccion de area de depuracion, volcamos la VRAM en bruto (gancho temporal del port, no subido) y cortamos las
piezas del SPR de esa VRAM con el desplazamiento de tpage (`w1e`) y la CLUT del dueño (renderizador desechable, no subido). El decodificador muestra que
todos acaban en 0x8000 (se reproducen una vez y se quedan en el ultimo fotograma); ninguno es un bucle.

| Overlay | Direcciones | Fotogramas | Que muestra (render) |
|---|---|---|---|
| X009 / X011 | 0x8012F144, 0x8012F164, 0x8012F17C / 0x8011C6E4, 0x8011C704, 0x8011C71C | 43 44 45 44 / 46 47 48 / 49 50 (50 fijo 255 ticks) | **Medusa (AREA09 s0) y pulpo volador (AREA11 s1), una sola tabla SPR** (tipo 0x3A, base 4 / 9, CLUT 30930 / 31058). Es una secuencia: recoge los tentaculos y se aplasta dos veces (43-45), se estira lanzando los tentaculos hacia abajo (46-48) y queda de pie sobre los tentaculos enroscados (49-50, fijo). El fotograma 42 no esta en ningun script. Parece una pose de aterrizar / estar en el suelo, no un ataque [I]. La sala de AREA11 ni siquiera carga la zona de pagina de los fotogramas 37-50 (el nado lateral 37-41, que su tabla viva usa, sale basura), asi que el render del pulpo toma los pixeles de AREA09 y la CLUT de AREA11 |
| X003 | 0x80139130 | 12-15 | tabla de la planta saltarina: una mata de hierba que se mece (decorado) |
| X003 | 0x8013A074, 0x8013A0A4, 0x8013A0E4, 0x8013A114; 0x8013A194, 0x8013A1A4, 0x8013A1B4 | 29-32-30, 33-35, 38-41-39, 42-44 (ida y vuelta); 24-25, 26-27, 46-47 | tabla de los aldeanos del viento (AREA03 s0, SPR 0x32): gestos al hablar del **enano del sombrero marron** (29-35) y del **enano del sombrero mexicano** (38-44), y tres movimientos del **enano viejo abrigado con bufanda roja** (24-27, 46-47). Animaciones de charla que nadie elige |
| X004 (tambien AREA12) | 0x80134E7C, 0x80134E9C | 16-19 / 19-16 | una **losa de piedra que gira** (pared o puerta giratoria que se abre y se cierra) |
| X004 | 0x801354E4 (un solo fotograma, 185, que el primer barrido no vio), 0x801354EC, 0x8013551C, 0x8013554C | 185; 195 187 188 196 188 187; 186-189-187; 190-192-185 | AREA04 s4 SPR 0x32 base 1: otra vez el **enano del sombrero mexicano** (tres gestos de charla). **Seguimiento:** los cuatro scripts son una copia del juego del trabajador del ayuntamiento / NPC de la pista del cuadro (tabla 0x8013540C = 0x80135430 fotograma 173, 0x80135438, 0x8013546C, 0x801354A0, leida por `D_801311B8[subtipo]` y `D_801311D0[subtipo]`) con todos los fotogramas +12 y los saltos de bucle cambiados por paradas. La tabla que elegiria a este segundo personaje no existe |
| X008 | 0x80118504, 0x8011853C | 26-29-26, 30-33 | sin render. **Dueño encontrado (seguimiento): el objeto Tubería.** El bloque 0x801184DC.. es una tira de scripts de objeto dibujados con `SPR_DATA[3]` (fotogramas 2-4, 24-35, 13-15, 0-5). 0x801184C8 si tiene referencia: el descriptor de objeto 81 de MAIN0 (0x8007AF44, objeto 64 = Tubería por `DAT_8007b084[64]`) apunta ahi, e `initItemObject` toma `anims[0]` (byte 7 del descriptor = 0) = 0x801184DC, la rueda de madera (fotograma 2) de AREA08 s1. Las demas palabras de 0x801184C8 no son punteros, asi que nada elige el resto del bloque |
| X010 | 0x80131DFC, 0x80131E14 | 5-7, 8-10 | decorado de AREA10 s0 (base 12): matorral de la jungla y tiras de cascada / espuma |
| X014 | 0x8012A090; 19 scripts en 0x8012A58C..0x8012A780 | 12-20; 9-33, 47-62 | sin render: en la sala del jefe (s7) y en s2 esas zonas de pagina no estan cargadas con las bases que usa la sala (solo 0-8, 18-21, 26-29) |
| X002 = X019 | 0x8011F2BC, 0x8011F2CC | 6-7, 8-9 | sin render: no cargado en AREA02 s1 con base 4 ni 10 |

Solo serian restaurables como decorado o poses de charla: ningun codigo los elige. La secuencia de la medusa/pulpo es la mas completa.

#### Comprobacion en ejecucion (port `--log-anim`), 2 de noviembre de 2026
Pregunta: ¿podria una seleccion calculada (base de tabla + desplazamiento), que una busqueda estatica no ve, llegar a estos scripts? **No: ninguno se elige
nunca, ni en ejecucion ni por ningun camino del codigo.** [V]

- **Herramienta (solo del port, desactivada por defecto):** `--log-anim [FICHERO]` (port2/runtime/animlog.c, ganchos en port/fixes.txt). Lee el TObj +0x24
  tras cada actualizacion de objeto del pool principal, dentro de `AnimLoadDuration` (la llamada que sigue a cada `o->anim = script`), `AnimJump`, `AnimAdvance`,
  `AnimAdvanceWithBox` (las cuatro copias) y `tickAnimationFixedRate`, y apunta el fotograma que muestra cada objeto visible en `drawObject`. Un puntero dentro
  de las ventanas, o un script de medusa/pulpo mostrando los fotogramas 42-50, imprime `[anim] HIT`. Al salir, el resumen lista cada direccion de script y
  fotograma por overlay, sala, categoria y tipo. `--log-anim-force CAT:TIPO,...` ademas pasa cada objeto de esos tipos por b04 1..3 x step 0..11 (state 0),
  una combinacion cada 110 cuadros, para que cada rama de su switch se ejecute aunque el jugador no la provoque. Ventanas: la tabla de arriba, con X004
  ampliada a 0x801354E4..0x8013558C y los 19 scripts de X014 como cuatro rangos.
- **Ejecuciones (sin ventana, seleccion de area de depuracion):** todas las secciones de AREA02, 03, 04, 08, 09, 10, 11 (s1, s2) y 14 (62 salas x 3600 cuadros,
  guion generico de andar / saltar / atacar / correr), y 19 salas de dueños dos veces (5800 cuadros cada una): una con Tombi teletransportado (`--poke` de su
  posicion) junto a cada tipo de dueño por turno, atacando, saltando y pulsando Circulo; otra con `--log-anim-force` sobre los tipos de dueño (8451 cambios de
  estado forzados). 447.200 cuadros en total. **0 lineas HIT; los fotogramas 42-50 nunca se dibujan.**
- **Lo que hicieron los dueños:** la medusa (cat 2 tipo 0x21) uso 16 de los 19 scripts de su tabla (flotar, nadar, agarrar, golpe, caida herida, hija que sigue,
  derrota); el pulpo volador (tipo 0x2A) uso uno, porque `flyingOctopusMove` solo elige `D_8011C420[0]` (su tabla es la de la medusa copiada, asi que sus
  scripts 43-50 estan doblemente muertos); el trabajador del ayuntamiento de AREA04 s17/s18 (tipo 0x32) uso 0x80135430 y el bucle 0x801354A0, cuyo salto en
  0x801354DC vuelve a 0x801354A0, el script justo antes de la ventana; el objeto Tubería de AREA08 solo mostro 0x801184DC; plantas saltarinas, aldeanos del
  viento, plantas mordedoras, humo y los decorados de AREA10/14/02 recorrieron sus tablas con normalidad.

| Script(s) | Veredicto | Pruebas |
|---|---|---|
| X009 0x8012F144/164/17C (medusa 43-50) | nunca | AREA09 todas las secciones; s0 natural + forzado (2 objetos); 42-50 nunca dibujados |
| X011 0x8011C6E4/704/71C (copia del pulpo) | nunca | AREA11 s1/s2; el codigo solo elige la entrada 0 de su tabla |
| X003 0x80139130 (planta 12-15) | nunca | AREA03 todas; tipos 2:24, 3:28, 3:29, 2:1D natural + forzado en s0/s1/s4/s5 |
| X003 0x8013A074..0x8013A1B4 (poses de charla) | nunca | tipos 2:32, 2:37 natural (Circulo pulsado a su lado) + forzado en s0/s1/s4/s5 |
| X004 0x80134E7C/9C (losa) | nunca | AREA04 las 20 secciones; planta mordedora 2:07, humo 3:2A/2B, coleccionable 2:29 forzados en s0/s9 |
| X004 0x801354E4..0x8013554C (juego NPC +12) | nunca | trabajador del ayuntamiento 2:32 en s17/s18 natural + forzado; 2:35/3:3C en s15 |
| X008 0x80118504/3C (bloque de la Tubería) | nunca | AREA08 todas; la Tubería solo muestra 0x801184DC (el descriptor usa `anims[0]`) |
| X010 0x80131DFC/E14 (decorado) | nunca | AREA10 las 9 secciones; decorado 3:3F, 4:35, planta 2:07 forzados en s0/s4 |
| X014 0x8012A090, 0x8012A58C..0x8012A780 | nunca | AREA14 las 8 secciones; decorado 2:4B, 3:50/51/52/23/61, 4:3A/3C forzados en s2/s3/s7 |
| X002 0x8011F2BC/CC | nunca | AREA02 todas; decorado 2:18, 3:2C forzados en s1/s2 |
| X019 0x80116178/88 (copia de los de X002) | sin ejecutar | el poke del selector para el area 19 acaba en AREA02; solo estatico |

**Seleccion calculada, revisada de nuevo en estatico** (capstone sobre los nueve overlays dueños y MAIN0, scripts desechables):
- **Cada escritura en TObj +0x24** se siguio hasta su registro de origen: una palabra cargada de una tabla (`lw`) o una constante lui/addiu. Las unicas fuentes
  aritmeticas son las rutinas de avance (`AnimAdvance`, `AnimAdvanceWithBox`, `tickAnimationFixedRate`, las copias `komaPigAnimAdvance` de X003/X004: +8 a la
  siguiente entrada, o la palabra puntero tras una entrada 0x4000), que siguen un script hasta su final y nunca pasan un terminador 0x8000. (X009 0x8011A3E0
  escribe en +0x24 de una estructura de camara, no de un objeto.) Cuentas de `sw rX, 0x24(obj)`: X002 59 carga / 73 const, X003 221 / 73 (+3 avance), X004
  217 / 73 (+3), X008 35 / 73, X009 125 / 78, X010 133 / 78, X011 73 / 73, X014 76 / 74, X019 35 / 73.
- **`AnimJump(o, n)` es la unica aritmetica base + indice sobre un script** (`o->anim += n` entradas, MAIN0 0x8001FE94). En el codigo de area de los nueve
  overlays toda llamada pasa n = 0, directamente o por los envoltorios de NPC `npcSetAnim` / `sevenFriendsSetAnim` / `sevenFriendsMemberSetAnim` (todos los
  llamadores pasan k = 0); los envoltorios de X004 / X010 (`mermaidSetAnim`, `townHallWorkerSetAnim`, `scaredGoldVolunteerSetAnim`,
  `paintingSlotHintNpcSetAnim`, `tenThousandYearElderSetAnim`) no tienen llamadores. Las constantes distintas de cero (2, 3, 4, 0xF, `lbu` de una tabla) estan
  en el codigo comun del jugador y mueven los scripts de Tombi en MAIN0.
- **Indexar tablas** (`tabla[n]`, `D_x[subtipo].anims[n]`, los `anims[b0c]` de los descriptores de objeto) carga una palabra, y ninguna palabra contiene una
  direccion de las ventanas: ni en el overlay dueño, ni en MAIN0, ni en ningun fichero del disco leido en el mismo overlay (barrido de los 2287 ficheros; los
  unicos aciertos en bruto estan dentro de peliculas STR y datos de imagen GAM).

**Imagenes** (en el scratchpad de la sesion, no en el repo): un GIF por script `<prefijo>_<direccion>.gif`, una hoja por dueño `<prefijo>_sheet.png` y un
atlas `<prefijo>_spr.png`; prefijos `x009_jellyfish`, `x011_octopus` (y `*_sequence.gif`: los tres scripts seguidos), `x003_monkey`, `x003_hopplant`,
`x004_bitingplant`, `x004_sombrero`, `x010_decor`, `x014_bossdecor`.

### Revisado, nada nuevo
- **Variables de guardado solo leidas:** cada byte del GCD que algun codigo lee y nadie escribe por direccion absoluta es un elemento del array de eventos
  (0x8009CDA4 + evento) o del de cantidades de items (0x8009D0A4 + item), que se escriben por indice. No hay ningun flag olvidado.
- Musica: ya cubierta por `tools/music_refs.py`.

### Sin revisar
Ids de SFX que nunca suenan (los ids vienen tambien de datos y de tablas de animacion); los pixeles de los fotogramas sin referencia de X002/X008/X014 (ver arriba); `X01..X19`, `INFO*`, `DSPSUB*` de la beta (no estan en local); los overlays de sistema de USA/JP frente a PAL.

## La fuente de depuracion (drawDebugText): seguimiento, 2 de noviembre de 2026

Pregunta: JP dibuja las estadisticas del final y TRY AGAIN / GAME OVER / NOW LOADING como texto de depuracion. ¿Hay una fuente con la que escribir
lo que queramos? **Si, en todas las versiones, PAL incluida, y el port ya puede usarla.** [V salvo marca]

- **No es el FntPrint de Psy-Q.** No hay `FntLoad` / `FntOpen` / `FntPrint` / `KanjiFntPrint` identificados en los mapas de simbolos de PAL, USA, JP ni la beta.
  El juego tiene su propio impresor, `drawDebugText(x, y, color, cadena)` (MAIN0 PAL 0x80020F14, 572 B, C emparejado en `src/drawDebugText.c`;
  USA 0x80023AA4, JP 0x80021E5C, beta 0x800233A4, el mismo codigo). Cada caracter es un `SPRT_8` de 8x8 (codigo 0x75) con u = (c & 15) * 8,
  v = (c >> 4) * 8 en la pagina de textura 0x15 (4 bpp, VRAM x 320, y 256) y la CLUT (368, 496 + color). Cada llamada añade un `DR_MODE`.
  Todo va al bufer de primitivas de depuracion, 0x780 bytes por bufer de cuadro: unos 115-120 caracteres por cuadro en total. Los que pasan
  se descartan (visto en el port: una quinta linea no salio).
- **Los glifos son parte de la pagina de textura del sistema** `SYSTEM/A00000.GAM` (cargada en (320, 256) al arrancar y presente en todos los cuadros
  que volcamos: titulo, seleccion de area, salas de AREA00/01; los menus usan otras celdas de la misma pagina). La zona de la fuente (128 x 128 px, la mitad
  izquierda) es **identica byte a byte en PAL, USA y JP**. Celdas: 0x10-0x1F = digitos hexadecimales `0-9A-F`; 0x20-0x7E = ASCII imprimible
  **con minusculas**, salvo que 0x40 es **`©`** en vez de `@` (por eso el `@1997 WHOOPEE CAMP Co.,Ltd.` de JP sale con ©); 0x80-0x89 = digitos
  0-9 apagados. De 0x90 en adelante hay arte del HUD y de efectos de la misma pagina, no fuente. Sin acentos ni kana. Glifos sombreados (degradado y
  sombra); las 4 filas de CLUT dan blanco, azul, naranja y verde.
- **Quien la llama.** PAL / USA: `bootStartupTask` estados 2-3 (sello `98/8/26` `14:10` en PAL, `98/3/22` en USA; inalcanzable, el estado 1 salta al 4)
  y la seleccion de area de depuracion `displayDebugScreen` (viva con el flag `0x1F8001B4`). JP añade, todo vivo en la version final: el
  `@1997 WHOOPEE CAMP Co.,Ltd.` y `PUSH [START] BUTTON` del titulo, `TRY AGAIN...`, `NOW LOADING`, ` GAME OVER ` en (96, 96), las estadisticas de X13
  en (32, 64/88/112), y una lectura de colision `flag= %01d`, `hit pos x/y= %04d`, `no hit`, `pos x/y= %04d`, `dir = %01d` (JP 0x8002FB98..0x8002FCF4).
  PAL / USA cambiaron todos los usos de JP por graficos y quitaron las cadenas de la lectura.
- **Port.** `drawDebugText` es codigo nativo en el port y la pagina esta cargada, asi que el codigo del port puede llamarla desde cualquier gancho
  por cuadro (probado con un gancho desechable en `drawHudIfShown`: las estadisticas JP y un texto de prueba salen en el juego en los 4 colores; el texto va a la
  tabla de orden del juego, detras del cuadro de mensaje y por encima de la escena). Usos: capas de depuracion / investigacion, un contador de FPS
  dentro de la imagen (el `--show-fps` actual se dibuja a la resolucion de la ventana con sus propios digitos 3x5, mas nitido a alta resolucion interna),
  textos cortos nuevos. Limites: 8x8 de ancho fijo, ASCII mas ©, 4 colores, unos 115-120 caracteres por cuadro para todos, solo en pantalla.

## Rutas de patrulla sin usar

Barrido (2 de noviembre de 2026): la lista de funciones de cada X0nn contra destinos jal/j, punteros y pares lui/addiu del overlay y de MAIN0. Luego buscamos
en las funciones muertas codigo de movimiento (posicion += velocidad, limites fijos, direccion por `COS_TABLE`, tablas de puntos de paso) y casos de subtipo
que ninguna colocacion alcanza. Volcamos los juegos de NPC de I0NFO2 (cajas de patrulla vistas desde arriba) con `tools/topdown.py npcs`. Las funciones muertas
de MAIN0 / X000 / I0NFO son las del barrido de zombis (ninguna mueve un objeto por una ruta). [V] salvo marca.

### X001 segmentedCrawlerWander: la primera ruta de la oruga
- **El dueño es la oruga verde del Muelle tambaleante** (render: cabeza con la parte de abajo naranja y una boca oscura, y 3 o 5 segmentos verdes
  redondos con un pincho rojo y una mancha rosa). No es la araña. Capa 2 tipo 0x13, `updateSegmentedCrawler` 0x80134C00. Ningun registro APD la
  coloca. La crea el objeto sin capa tipo 7 `updateSegmentedCrawlerSpawner` (0x8013C028, tabla sin capa de MAIN0 D_8007BF34 indice 7). Ese objeto sale de la lista
  sin capa por seccion `PTR_8007bee4[area][seccion]` = {2, 1, 7}, que solo esta en **AREA01 seccion 2 (Muelle tambaleante)**. El mismo puntero
  aparece como AREA07 s2 (la copia de AREA01 que no esta en el disco) y como indice 8 de AREA00 (mas alla de sus 6 secciones).
- Generador: la primera oruga a los 600 cuadros y luego una cada 480 / 600 / 360 / 440 (al azar), como mucho 2 a la vez, en uno de los cuatro puntos
  D_8013CA10 (h, y) = (525, -510), (710, -580), (940, -630), (1130, -570), a la profundidad del jugador. Subtipo = paridad del cuadro & 1: **0 = cabeza +
  5 segmentos, 1 = cabeza + 3**. Los subtipos 2 y 3 existen en el switch pero nunca se crean.
- En esta sala el eje horizontal es el campo `b` del TObj (Tombi entra en 215). Los limites 0x118 (280) y 0x551 (1361) abarcan el tramo del puente de cuerda [I],
  y la oruga anda por la copa de los arboles encima (y de -400 a -630; Tombi en el puente esta a -229 .. -259). Fuera de pantalla su y se limita a
  -0x190 como mucho.
- **Ruta viva (`segmentedCrawlerMove`, 0x80132B64)**: baja 110 cuadros (JP: 60), gira a la derecha, corre hasta h >= 0x551 *o* hasta que
  `segmentedCrawlerCheckWall` choca, da la vuelta (por abajo o por arriba segun y < -0x140), corre a la izquierda hasta h < 0x118 o una pared, gira y
  **repite siempre** en dos carriles.
- **`segmentedCrawlerWander` (0x801334DC, 1352 B)**, sin referencia en X001, MAIN0 ni ninguna tabla: el mismo inicio (pero 60 cuadros, como el Move de
  JP), **sin pared, sin carriles y sin bucle**: a la derecha hasta h >= 0x551, media vuelta a angulo 0x80 (estado 7; el 9 no hace nada), a la izquierda
  hasta h < 0x118, giro por arriba hasta mirar a la derecha (estado 13) y **el estado 14 no tiene caso: la cabeza se para para siempre** en h 280
  y el cuerpo se queda detras. Es una patrulla de ida y vuelta unica, un primer borrador de Move (Move = Wander + paredes + carriles + bucle). **Comparte** las
  tablas de animacion con Move y `segmentedCrawlerGrabbed` (D_8013F034: 12 scripts de cabeza por direccion, fotogramas 57/58, 152/153, 150/151, 148/149 y
  124-131; D_8013F064; el cuerpo usa D_8013F1BC, fotogramas 0-15). La nota anterior ("tabla propia", "sube") estaba mal (el angulo 0xC0 es +y, hacia abajo).
- Sala: solo el Muelle tambaleante. Sus limites son las mismas constantes que la rutina viva, asi que la patrulla se escribio para esta sala. Los cuatro casos de
  subtipo duplicados de `updateSegmentedCrawler` son el sitio donde un subtipo llamaba a Wander [I].
- Versiones: **sin referencia en las tres versiones finales**. JP X01 la tiene en 0x80131DB8 (base 0x800E9AF8) y su `updateSegmentedCrawler` solo
  llama a Move (0x80133618). USA X01 en 0x80132814 (el fichero X01 de USA se mapea con base 0x800E7388), sin referencia. El X01 de la beta no esta en local. **PrePre Vol. 8
  (seguimiento):** su unico TOMBA.EXE si tiene la oruga, como clase 2 tipo 0x13 (el mismo id que PAL), con sus propias rutinas de movimiento por
  subtipo; ni Wander ni Move estan (similitud enmascarada <= 17 %), y el objeto nunca se coloca ni se crea en la demo (docs/PREPRE8.md "The segmented caterpillar").
- **Port: `--crawler-patrol`** (port/fixes.txt, desactivado por defecto) hace que la cabeza use Wander en vez de Move. Grabado: las dos orugas bajan,
  van a la derecha hasta 1361, giran, vuelven y se quedan aparcadas una encima de otra en h 280 mirando a la derecha. La version final sigue en bucle.
  GIF lado a lado. La patrulla queda mas alta que los carriles normales; para grabar subimos la camara fijando la y de Tombi (gancho
  desechable), porque las orugas andan por encima de la vista normal.

### Otros movimientos sin usar
| Funcion | Dueño / sala | Ruta | Versiones | Veredicto |
|---|---|---|---|---|
| X009 `flowerTearsSeekerWalkOff` 0x80128B4C (248 B) | el NPC que busca las lagrimas de flor (MFNT090S) | anda a la izquierda a 1 px por cuadro hasta salir de pantalla y se oculta (b04 2). Es el `flowerTearsSeekerPace` vivo (va y viene entre h 0x708 y 0x780 con gravedad y colision) sin el giro ni la colision | sin referencia en PAL, JP (0x80127CD4) y USA (0x80127E94) | salida de escena que nadie llama |
| X002 `sevenFriendsMemberFlyUp` 0x80119088 (240 B) | un miembro de los Siete Amigos (AREA02) | espera su temporizador, sube acelerando (-0x10 por cuadro hasta -0x600), cambia a la anim 1 en -0x80 y se libera fuera de pantalla | sin referencia en PAL, JP (0x801182E4) y USA (0x801183D8) | salida volando; la escena viva hace su propia salida [I] |
| X001 `komaPigCheckWall` 0x8012BC84 | cerdo koma, AREA01 | (sonda de pared, no ruta) | las copias de X003 / X004 estan vivas | los cerdos koma de AREA01 nunca miran paredes |

Vivas, para que conste: X010 `spearPigWanderB` (subtipo 1 del cerdo de la lanza, colocado en AREA10 s0) y X009 `flowerTearsSeekerPace`.

### Cajas de patrulla vistas desde arriba (juegos de NPC de I0NFO2)
Campos en docs/CIVILIZACION.md: comportamiento 1 vertical, 2 horizontal, 3 / 4 diagonal; la caja son los limites. **La unica patrulla que corre en la
version final es AREA19 s2 (juego 11) actor 4** (vertical, y 268..304). Todas las demas estan en los pueblos recortados de Civilizacion y nunca corren:
juego 0D (AREA11 s0) actores 3 (diagonal), 4, 5, 8 (horizontal), 7, 9 (vertical); juego 0E (AREA16 s0, cajas relativas al actor) 2, 7 (diagonal), 4 (vertical),
5 (diagonal); juego 0F (AREA17 s0) 6, 9, 12 (horizontal), 8, 13, 14, 15 (diagonal); los juegos 0C (AREA08) y 10 (AREA18) no tienen patrullas.

### Sin revisar
Tablas de puntos de paso leidas por codigo vivo con entradas que ninguna ruta alcanza (p. ej. `updateWaypointRide` de AREA09). Estados inalcanzables dentro de
otras rutinas de movimiento vivas.

## Hoja de glifos de los túneles (AREA02/B301): disposición, código y texto, 2 de noviembre de 2026

Continuación de la fila de B301.000 en "Perdido" (preguntas de la comunidad: ¿son baldosas de suelo sueltas?, ¿queda
código para colocarlas?, ¿las menciona el texto japonés?). Las imágenes no se suben (píxeles del juego).

### Disposición
- **Rejilla:** el bloque son exactamente 10 x 3 celdas de 24 x 24 px, sin separación (u 0..239, v 72..143). Fila 0
  (v 72..95): numerales mayas 1, 2, 3, 4 (puntos), 5 (barra), 6..9 (barra + puntos), 0 (concha). Fila 1 (v 96..119):
  **diez** símbolos, no once: sol, pájaro, luna creciente, estrella enmarcada, arco y flecha, flechas cruzadas,
  engranaje, diez aritos alrededor de una barra, rejilla, montículo con un arco. Fila 2 (v 120..143): u 0..71 piedra
  lisa, u 72..95 "=" (dos barras = 10 maya), u 96..239 índice 0 (vacío).
- **Una sola baldosa de piedra bajo cada glifo:** las 21 celdas con glifo (numerales, símbolos, "=") son la misma
  baldosa de 24 x 24 con el glifo pintado encima (mediana del 97 % de los píxeles sin glifo idénticos a la moda por
  píxel; 62 % en la estrella enmarcada, cuyo recuadro interior tapa más). La baldosa solo enlaza a medias: salto medio
  de luminancia en las costuras 17,4 (izquierda/derecha) y 17,2 (arriba/abajo) frente a 11,9/15,7 dentro y 21,5
  entre columnas al azar.
- **Los marcos son una tira, no placas:** en la fila 1 el marco de 1 px (índice 4 del CLUT) es la fila superior e
  inferior de cada celda y la columna **derecha** (u = 24k + 23); solo la primera celda tiene además columna izquierda
  (u 0). Recortadas sueltas, las celdas 1..9 tienen marco por tres lados: los símbolos se dibujaron como una fila
  continua de recuadros (o para ponerse uno junto a otro en ese orden). Los numerales no tienen marco.
- **La fila 2 u 0..71 es una pieza de piedra de 72 x 24,** no tres baldosas: las costuras entre sus celdas (7,8; 9,2)
  no superan sus saltos interiores (8,4), mientras que sus costuras de enlace (18,1 / 19,3) son como al azar (17,3).
- **No es una tira de fuente:** celdas de 24 x 24 con el glifo centrado (puntos y barras en las filas 7..17), sin
  subrejilla de 8x8 ni 16x16.
- **No está recortada del suelo de los túneles:** el gris (CLUT 0x7D8F, grises neutros 80..184) no coincide con
  ninguna ventana de 24 x 24 de las páginas 12..15 de B301 (como mucho 18 % de índices iguales, el azar); los suelos
  son el empedrado claro de la página 14 (CLUT 0x784F, grises 168..224), arena y tierra. 0x7D8F es la fila 502, la
  única libre entre los CLUT del fondo de la página 13 0x7D4F (501) y 0x7DCF (503).
- **Los túneles no tienen baldosas de suelo:** el fondo de I0NFO3 son polígonos libres (ver abajo) con ~1 texel por
  unidad de mundo en los suelos (mediana mundo/texel 1,11 en el empedrado, 1,06 en la tierra), y la única rejilla es
  el mapa de colisión de 8 x 8. Una celda de 24 px cubriría 3 x 3 celdas de colisión.

### Código (I0NFO3, sección 3 de AREA02): ningún camino de colocación
- **Formato del fondo (corrige los "882 polígonos" de arriba):** el bloque de tipo 0x50 (D306 + 968) son 24 offsets
  de piezas, y cada pieza son grupos `u32 cuenta` + polígonos en el orden FT3, FT4, GT3, GT4 (texturizados: CLUT,
  tpage, 1 o 3/4 palabras de color, luego x, y, z, u, v por vértice) y después los grupos sin textura. 2.506
  polígonos texturizados; la página 13 solo en v 0..69 (piezas 0x5C/0x60, los fondos alternativos, CLUT 0x7C0F) y
  v 146..157 (CLUT 0x7DCF, las piedrecitas del anillo); ningún CLUT 0x7D8F; nada muestrea v 72..143. Dibujando todas
  las piezas sale la red de túneles entera a 1:1 (arena, empedrado, un estrado de piedra agrietado y un anillo de
  siete piedras piramidales sobre la arena hacia x 385..445, y -992..-940).
- **No existe fórmula baldosa -> UV:** `tunnelCollideMapTile` / `tunnelCheckTileTrigger` leen `TUNNEL_MAP_TILES`
  (D303, 240 x 168 celdas de 8 px, copiado a 0x80124000) solo como colisión. Sus valores son 0..9 (forma) más el
  bit 7 (disparo) y nada más (ningún valor sin usar que pudiera elegir gráficos).
- **Celdas de disparo:** 17 grupos. Cinco están sobre las cinco salidas de `tunnelSectionStep`; dos son las zonas
  que cambian el fondo: sobre una celda de disparo `tunnelPlayerMainStep` guarda 1 + el índice del rectángulo de
  `TUNNEL_TRIGGER_RECTS` que contiene al jugador en `PLAYER.b6b` = `D_800A60A3`, el valor que lee
  `tunnelSelectBackdropPieces` (rect 0 -> fondo 1, rect 1 = (0,0,0,0) de relleno, rect 2 -> fondo 3). Las otras
  diez (5 x 5 celdas en x 408..447 y -1184..-1145 y 592..631 y -1144..-1105, irregulares en 864..903 / -1048..-1017
  y 992..1023 / -968..-929, seis celdas sueltas) no tienen lector: nada en I0NFO3 usa el bit de disparo del jugador
  fuera de los rectángulos, y el único lector de `PLAYER.b68` en MAIN0 (opcode de script 0x93) llama a la dirección
  de overlay lateral 0x800ECB40, que en I0NFO3 cae dentro de `tunnelItemSpotHandler`. Ninguna mide 3 x 3 celdas.
- **Ninguna lógica de puzle:** las 55 funciones de I0NFO3 están decompiladas y con nombre; ninguna cuenta pasos,
  compara una secuencia ni lleva contadores de dígitos. Colocación (tipo 0x83): 20 registros de tipo 7 (compuertas
  en pareja) y 2 de tipo 0x3F; tabla de actores: 11 actores de tipo 0x45; los puntos de charla muestran todos (1, 0)
  (el aviso de guardar). Desplazamientos de tpage: 0 (jugador), 2..4 (actores), 0xF (cofres del tiempo), 0x15 (puntos
  de charla); ninguna tabla de sprites de la sección muestrea la página 13 en v 72..143.
- **Versiones:** el I0NFO3 de USA tiene los mismos rectángulos y elecciones de diálogo; el INFO3 japonés también
  (rectángulos en el offset 0x67A4). El mapa de colisión japonés (`AREA02/D304.000` del disco JP) difiere del
  PAL/USA en 136 celdas de pared, nunca en un bit de disparo. El B301 japonés solo difiere del PAL en y 144..246
  (palabras 64..175); el bloque de glifos es idéntico. 0x7D8F no aparece en ningún INFO3 ni X02 de JP/USA. Las
  versiones beta 980203 y PrePre 8 no tienen datos de AREA02.

### El texto: un puzle de "garabatos" recortado con numerales mayas
- **La pista del excavador, saltada por el código.** Grupo 6 de diálogo de los túneles de AREA02
  (`tunnelActorOpenDialogue`, el excavador del caso 8): el código muestra dx 0xF, inicia el evento 161 "Underground
  Treasure" y luego muestra dx 0x11; **dx 0x10 nunca se muestra** (mismas constantes en PAL, USA y JP). El PAL #439 =
  dx 0x10 es `···???`; la línea japonesa (AREA02 A003 #433) es:
  言い伝えだと・・・宝物庫へ行くには仕掛けをクリアしないとダメらしい・・・ でも オレはさぁ 宝物庫を掘り当ててやるんだ！
  ん・・・仕掛けを解く方法？ それは仕掛けをつくったヤツが残した岩のらくがきを 調べてみろよ たしか・・・地上に６コあるハズだから
  見つければ やくに立つかもな・・・
  «Según la leyenda... para llegar a la cámara del tesoro hay que superar un mecanismo... ¡Pero yo voy a llegar
  a la cámara cavando! Mm... ¿cómo se resuelve el mecanismo? Estudia los garabatos de roca que dejó quien lo
  construyó. Creo que hay seis allá arriba, en la superficie; si los encuentras, a lo mejor te sirven...»
  En el juego final, el evento 161 se completa abriendo el cofre del tiempo de subtipo 7 en los túneles (300.000 PA,
  `tunnelItemSpotCheckStep`); no hay mecanismo.
- **Las seis rocas usan puntos y barras en base 20.** Grupo 2 dx 3/4 de AREA16 (PAL #211/#212, JP AREA16 A003
  #188/#189), sin referencia en X016:
  キミ、いろんなところを旅してるんだって？ だったら、へんな点とか線がきざんである らくがき岩・・・見たことないかなぁ・・・
  おれ、ずーっとまえに木人の加工場で 見たことがあるんだけど、どうも気になってなぁ・・・
  «Viajas por todas partes, ¿no? Entonces, ¿has visto alguna roca de garabatos con puntos y rayas raros grabados...?
  Yo vi una hace mucho en el taller de la gente de madera y no se me va de la cabeza...»
  ウワサじゃ 全部で６つのらくがき岩があって、そこに書かれてあるナゾをとけば たくさんの お宝が 手に入るらしいんだけど
  ・・・なんかむずかしそうだなぁ ・・・が１・・・ーが５の・・・ ２０進数とかなんとか・・・ おれにはとても わかんないよ・・・
  «Dicen que hay seis rocas de garabatos en total, y que si resuelves el acertijo escrito en ellas consigues
  muchos tesoros... pero parece difícil... "·" es 1... "–" es 5... base veinte o algo así... yo no lo entiendo...»
  (el "·" y la "ー" son los mismos glifos que los puntos suspensivos y la raya de vocal larga, así que se lee como un
  murmullo). El PAL conservó las líneas pero perdió la notación: «1 es..por 5...a la 20ª potencia...» (todos los
  idiomas dicen "potencia"). Punto = 1, barra = 5 y base 20 son los numerales mayas de la hoja. **Corrección:** las
  rocas no están recortadas; sus placas están en las páginas de textura de las salas y dos están colocadas en el juego
  final (ver "Rocas de garabatos (らくがき岩): las placas existen" más abajo). Lo recortado es todo lo que las rodea: el
  evento 67 解読？らくがき岩 «¿Descifrar? La roca de garabatos» tiene título japonés (y pista japonesa, DSPSUB#1 #67
  らくがき岩の　もじを　かいどくする «descifrar la escritura de las rocas de garabatos») pero ningún inicio, final ni
  lectura de su marca (notes/secrets.md).
- **PrePre 8 nombra el suelo.** La tabla de títulos de evento de PrePre 8 (`tools/prepre8_text.py`, cadena en
  0x800AB038, glifos comprobados en `SYS008.000`) tiene らくがき床のナゾ «El enigma del suelo de garabatos», seguido de
  かくされた入口 «La entrada escondida»; también tiene 解読?らくがき岩 y 森の地下には何がある? «¿Qué hay bajo el
  bosque?» (en el juego final "What's Under the Forest?" es el evento propio de los túneles). Ninguna versión final
  tiene un título de "suelo de garabatos".
- **Otras coincidencias, sin relación:** 数字玉１..１０ / "Math Bead 1-10" son objetos del juego final de la misión del
  Anciano de 10.000 Años (texto japonés: «una bola con el N escrito que te pidió el Anciano de 10.000 Años»), que se
  recogen bajo el agua en Trick Village; van numerados del 1 al 10 como los numerales pero nada los une a la hoja.
  Las demás líneas `···???` de los túneles son la vagoneta (PAL #436/#437 = JP #430/#431 «necesita agua... agua que
  arde», «¿ves?, ¡funcionó con agua!»; #457 = JP #448 «¡la vagoneta arrancó!») y la escena de la compuerta (PAL #406,
  #414..#418 = JP #400, #408..#412). Ningún diálogo del JP final, la demo japonesa ni PrePre 8 usa 太陽, 鳥, 弓, 矢,
  石板, パネル, 記号, 暗号 ni パスワード para esto.

**Lectura:** lo más probable es que la hoja sea el juego de baldosas del mecanismo recortado del "suelo de garabatos"
que guardaba la cámara del tesoro del Laberinto Subterráneo: una fila de diez símbolos enmarcados y placas de
numerales en notación de puntos y barras en base 20, cuya clave estaba en seis rocas de garabatos de la superficie.
No queda el suelo ni código para él; las rocas sobreviven en parte (cinco texturas de placa, dos colocadas como simple
decorado, abajo); el juego final conservó el tesoro (evento 161, un cofre del tiempo) y dos líneas de pista, una de
ellas sin usar.

## Rocas de garabatos (らくがき岩): las placas existen, 2 de noviembre de 2026

Corrige la sección anterior, que decía que las seis rocas nunca se hicieron. Un ripeo de la comunidad muestra cinco
placas de piedra con numerales de puntos y barras; están en los datos del juego. Las imágenes no se suben (píxeles del
juego).

### Dónde están
Las cinco usan la misma baldosa de placa de 42 x 51 px (marco, veta de la piedra e índices de 4 bpp idénticos fuera de
las marcas; solo cambian las marcas), así que se buscó ese patrón de índices en todo el disco (búsqueda por nibbles en
cada fichero, GAM descomprimidos, PAL, USA y JP): cinco placas distintas; están todas las coincidencias.

| Número | Fichero (PAL) | VRAM (página 4 bpp, u, v) | Sala | Se dibuja en el juego final |
|---|---|---|---|---|
| 2 | `AREA01/A003.GAM` (página de área, en la palabra (768,0)) x 102, y 64 | página 12 (768,0), u 102..143, v 64..114 | AREA01 (Bosque de las 100 flores / Bosque de los Enanos), todas las secciones | **sí**: modelo de objeto #22 de la tabla de modelos de s0/s1 (#10 en s3), CLUT 0x7849 (144,481), colocado por un registro de capa 4 tipo 0x13 (`updateStaticDecorModel`) en (4110, -399, z 48) en s1 y s3, justo a la izquierda de la salida del extremo derecho de s1. Igual en AREA07 (copia de las listas de AREA01). |
| 5 | `AREA04/A003.GAM` (página de área, palabra (832,0)) x 654, y 192 | página 15 (960,0), u 142..183, v 192..242 | AREA04 (Mansión Encantada), todas las secciones | **sí**: modelo #22 de las tablas de s0..s3, CLUT 0x7ACF (240,491), registro decorativo tipo 0x13 en (1080, -675, z 940) en s0 y s1: un bloque de piedra con la placa en la terraza a la derecha de la puerta con cara de la mansión. Igual en AREA12 (copia de AREA04). |
| 18 | `AREA03/A002.GAM` (página de área, palabra (0,256)) x 398, y 24 | página 17 (64,256), u 142..183, v 24..74 | AREA03 (Montaña tormentosa / Cuevas de lava / Nido del Fénix), todas las secciones | no |
| 139 | `AREA10/B004, B104, B203, B302, B404, B504, B603, B702` x 266, y 176 | página 17 (64,256), u 10..51, v 176..226 | AREA10 (Jungla), secciones 0..7 | no |
| 59 | `AREA11/B100.GAM` (página de sala, palabra (512,0)) x 10, y 136 | página 8 (512,0), u 10..51, v 136..186 | AREA11 sección 1 | no |
| (2) | `AREA09/B002.GAM` x 870, y 64 | página 15 (960,0), u 102..143, v 64..114 | AREA09 (Bosque de setas) sección 0 | no; idéntica píxel a píxel al "2" de AREA01 |

- **USA y JP** tienen las mismas placas en los mismos offsets (USA AREA04/A004, AREA09/B004; JP igual). **Solo JP:** la
  placa "59" está además en seis páginas de sala de AREA10 del JP (B003, B102, B203, B403, B502, B603, en y 136), es
  decir, la jungla japonesa también llevaba la textura de la roca 59; PAL/USA solo la conservan en AREA11.
- **Cómo se decidió "se dibuja":** se comprobó contra las cajas UV de las placas cada polígono texturizado de cada
  modelo de sala (kind 0x50, también los contenedores que empiezan por WHM de AREA01/09) y de cada tabla de modelos de
  objeto (piezas kind 0x51..0x5F) de AREA01/03/04/07/09/10/11/12: solo los dos modelos de arriba muestrean una placa.
  Para las otras tres, ningún polígono de sala, ningún modelo de objeto y ninguna parte SPR (solo mapas de sub-pantalla
  de página entera) muestrea la placa. El generador de la capa 4 (`spawnObjectFromRecord`) toma el modelo del byte 14
  del registro (`be`) cuando el byte 12 tiene el bit 0x10: be = 22 (AREA04) y 22 / 10 (AREA01).
- **Comprobado en el port:** con el modo vuelo del menú de depuración del port (`--debug-menu`, L1+R1+Select),
  AREA04 s0 con el jugador en (978, -716) muestra la roca "5" a la derecha de Tombi en la terraza de la mansión
  (`--dump-draws`: CLUT (240,491), página (960,0), uv 129..197 x 186..248, frames 1809+); AREA01 s1 en (4252, -521)
  muestra la pequeña placa "2" en el muro de piedra antes de la salida (CLUT (144,481), página (768,0)).
- **Ningún código las usa:** `updateStaticDecorModel` (X001 = X004, 0x800EE18C) solo pone los flags de dibujo y dibuja;
  sin diálogo, flag, evento ni comprobación. El evento 67 no tiene inicio/final/lectura (arriba). El aldeano de
  Stonetown dice que vio una "en el taller de los hombres de madera" (木人の加工場, AREA16 #188): encaja con la textura
  "59" de la página de AREA11. Ninguna otra línea japonesa nombra una roca, un número, un orden o un símbolo.

### Lectura de los números (base 20 maya)
Punto = 1, barra = 5, los niveles se leen de abajo (x1) a arriba (x20); dentro de un nivel los puntos van sobre las barras.
- AREA01: dos puntos, un nivel: **2**.
- AREA04: una barra: **5**.
- AREA03: tres puntos sobre tres barras, espaciado uniforme, un nivel: 3 + 15 = **18**.
- AREA10: nivel superior un punto sobre una barra (6), hueco claro, nivel inferior cuatro puntos sobre tres barras (19):
  6 x 20 + 19 = **139**.
- AREA11: nivel superior dos puntos (2), hueco, nivel inferior 19: 2 x 20 + 19 = **59**.
(Leídas como un solo nivel las dos últimas serían 6+19 = 25 y 2+19 = 21; el hueco y el "base 20" del aldeano hacen que
la lectura en dos niveles sea la buscada.) La sexta roca no está en ningún disco (PAL, USA, JP); la copia del "2" en
AREA09 es la única placa extra.

**Frente a la hoja de B301:** la hoja tiene baldosas de numeral sueltas 0 (concha), 1..9 y "=" (dos barras, 10), de
24 x 24, más diez símbolos enmarcados. Las rocas necesitan dígitos hasta 19 (18, y 19 en dos de ellas), que la hoja no
tiene, y sus marcas son más grandes y van sobre otra placa de piedra enmarcada de 42 x 51, así que las rocas no son las
baldosas del suelo.

**Puzle posible (inferencia, nada en el código ni en el texto lo dice):** seis rocas repartidas por las zonas de la
superficie (bosque, montaña, mansión, jungla, Civilización) daban cada una un número; el "mecanismo" de la cámara era un
suelo de baldosas de numerales y símbolos donde había que pisar o fijar las correctas. Con baldosas de dígitos solo
hasta 10 y una baldosa "=", el diseño más coherente es una ecuación o una secuencia: p. ej. los seis números en el orden
de las rocas (o sumados/combinados; los cinco conocidos suman 223) dan las baldosas que hay que pisar, o el número de
cada roca elige uno de los diez símbolos. Los valores conocidos 2, 5, 18, 59, 139 no forman una serie evidente y, con
una roca perdida, la respuesta buscada no se puede recuperar.

## PrePre Vol. 9: animaciones de Tombi que retail no tiene (2026-11-10)

Ver la sección "PrePre Vol. 9 (PCPX-96093): Tombi animations and art retail does not have" de [CUT_CONTENT.md](CUT_CONTENT.md) (la documentación nueva se escribe en inglés).
