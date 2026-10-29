# Contenido recortado: inventario y estado

Versión inglesa: [CUT_CONTENT.md](CUT_CONTENT.md).

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
| Títulos de evento de PrePre8 少年旅立ち / あれま大きな水たまり | Tira de glifos 16x16 de TIM000 (x 0..224, y 104..232). | Solo texto: una cadena WFM3 de título (el codificador existe). Necesita un evento que la lleve (ver evento 6). |
| Interior 2D de la casa del anciano (PrePre8) | `TIM007` 8 bpp + CLUT00 fila 20, `DATA002` (solo colisión). | Un camino de sala 2D que PAL no tiene para esa casa; mejor como galería. Medio-difícil. |
| Pájaro gris enemigo (PrePre8) | TIM000 (~16 poses); candidato tipo 15, `func_8003D40C` en TOMBA.EXE. | Sin IA equivalente en PAL; solo decoración o galería. Difícil como jugabilidad. |
| 15 sonidos de PrePre8 | JOCHU 1, 5, 11, 12, 19-21, 23-25, 27; S_AREA01 0, 25, 28, 29. | Fácil meterlos en un banco; no hay evidencia de dónde sonaban. |
| OPTION MODE / visor de enemigos (PrePre8) | 0x800854B4 (`OPTION MODE`, pruebas de música/sonido/CD/tarjeta), 0x80083D88 (`enemy=%01d`), código muerto en esa versión. | Reimplementación solo en el port; PAL no tiene ese código. |
| Cactus / cartel del GoGoCart (AREA06) | VRAM de `A001.GAM` (577,256), (516,256), (581,348); sin parte SPR ni polígono; D004.000 tiene 60 modelos y `a06AddModelsBySection` usa los 60. | Partes de sprite nuevas y colocación: nuestras. |
| Minero de AREA17 B201 y grandullón de AREA16 B501 (de lado) | Texeles sin ninguna parte SPR. La página B201 se ve con una paleta real: AREA11/CLUT02 fila 36 (la misma fila en AREA17/CLUT03, AREA17/CLUT06 y AREA18/CLUT02; la fila del «Male Lumberjack Town Resident») colorea a su hombre de lado con mochila igual que el sprite en color de TCRF (los 14 colores están en esa fila; la silueta se parece pero no es píxel a píxel, el fotograma de TCRF es distinto). Identificación nuestra. | Habría que escribir las tablas SPR a mano; animación desconocida. |
| Bocadillo «?» pequeño | SPR 0x34 f49; sin guion ni puntero. | Solo gráficos. |

## Perdido

| Hallazgo | Por qué |
|---|---|
| Pueblos de la Civilización (secciones cenitales de AREA11/16/17/18) | Los lleva docs/CIVILIZATION.md (`--civ-test`): el código de I0NFO2 está completo, pero las listas de carga están vacías y sus ficheros de mapa / SPR / paletas no están en ningún disco. 63 frases (clase B*). |
| Fotogramas de la flor dorada creciendo (AREA02 SPR 0x3B f27-32) | Los rectángulos a los que apuntan están vacíos en la VRAM PAL: el arte no está en el disco. |
| Texto JP de clase D ligado a la Civilización (MFNT110/170/180: balsero, generador, carbón, Dr. Gantetsu, pueblo nuevo) | Sus escenas son los pueblos cenitales cortados. |
| Guion de diálogos de PrePre8 | La fuente 10x10 está, pero no se encontró texto que la use (abierto, ver docs/PREPRE8.md). |

## Cómo se usa

```
tombi2 --data data --cut-content [--early-title /ruta/a/extraccion_prepre8]
python3 -I tools/prepre8.py extract PREPRE8.bin /ruta/a/extraccion_prepre8   # una vez, desde tu propio disco
```
