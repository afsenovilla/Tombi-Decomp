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
