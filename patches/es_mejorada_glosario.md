# Glosario de la traducción española mejorada (`es_mejorada.tombipatch.json`)

Retraducción del texto español del disco PAL a partir del texto inglés del mismo disco (archivos `*B.WFM`), con el
español original como contexto. Español de España, tono desenfadado de plataformas. Se conservan los nombres
oficiales de la versión española (lugares, personajes, objetos) salvo errores claros, para que el texto no editado
siga siendo coherente.

Restricciones: solo caracteres de la fuente de cada archivo (no hay `:` `;` `%` `ü` `W`; cada archivo tiene su propio
juego de glifos, p. ej. `Ú` no existe en MFNT001), mismos códigos de control (pausas, `{WAIT}{CLEAR}`, colores, voces,
`{MOVE}`, `{CHOICE}`, iconos de botón), mismo número de páginas. Validación: `python tools/check_text_patch.py
game/disc/Tombi.bin patches/es_mejorada.tombipatch.json`.

| Inglés | Español (este parche) | Nota |
|---|---|---|
| Tombi | Tombi | |
| Evil Pigs | Cerdiablos | nombre oficial español |
| Evil Pig Bag | Bolsa Cerdiablo / Bolsa Cerdo (de fuego, Ratón...) | mayúscula: es un objeto clave |
| Koma Pigs | Cerdos Koma | |
| 100 Year Old Wise Man | (sabio) Anciano centenario | |
| 1,000 / 10,000 Year Old Man | Anciano milenario / Anciano de 10.000 años | |
| Dwarf Elder | Enano Mayor | |
| Animal Dash | Carrera animal | antes "Huida animal" (error) |
| Charity Statues / Flower Tower | Estatuas de la caridad / Torre de flores | |
| Ol' Pond | Viejo estanque | |
| Yan of the Hidden Village | Yan, el de la Ciudad oculta | |
| Village | Ciudad (nombres propios: Ciudad Baco, La ciudad de los Enanos...) | oficial |
| Mr. Turtle | el Sr. Tortuga | antes "la Sra. Tortuga" |
| Psychic Fish | Pez psíquico | antes "Pez potencia" |
| Blackjack | Mangual | antes "Bola"; bola de pinchos con cadena (como el francés *Fléau d'armes*) |
| chest (100 year old chests) | cofre (cofres centenarios) | antes "arcón" |
| Directional button | botón de dirección | |
| Exercise Routine | Ejercicio | |
| Item (menú de Select: Item / Event / Status / Map) | Objetos (Objetos / Eventos / Estado / Mapa) | antes "Elmto": la tira de glifos del menú está llena y no tiene `O` ni `b`; el port las dibuja (`tools/pause_text.py`, clave `EXE/MAIN5.EXE` → `pause`). En plural, como Eventos (decisión del usuario); caben en el hueco de 8 bytes de cada entrada; el mismo término que "pantalla de objetos" y la pestaña *Objetos* del panel |
| AP | AP | los puntos se quedan en AP, como los gráficos del HUD y de la lista de guardado (antes "PA"; también en la pista del evento *La caja AP*) |
| Kokka birds | pájaros Kokka | |
| Pig / ol' coot / geezer | vejete, vejestorio | registro coloquial |
| Ha ha ha | ¡Ja, ja, ja! | con comas |
| See ya! | ¡Hasta luego! | |
| GrappleJack | Manguancho | antes "Garfio-gancho"; mangual + gancho (Grapple + Blackjack), decisión del usuario |
| Dashing Pants | Pantalón veloz | antes "Pantalón elegante" |
| Funky Parasol | Sombrilla chula | antes "Sombrilla rara" |
| Miner's Hat | Casco de minero | antes "Gorro (del) minero" |
| 1 UP | Vida extra | antes "1 +" |
| Tear Bottle | Tarro lágrimas | el título y la lista dicen *Dame el tarro de lágrimas*; la pista, "Consigue el Tarro lágrimas del Bonsugee amarillo." |
| Red Fortune Teller | la Adivina Roja (título: *La adivina roja*) | es una mujer: cartel de su casa, panel y las demás lenguas (Wahrsagerin, devineresse, indovina); antes "el adivino rojo". El azul sí es *el adivino azul* |
| Phoenix Mountain | Montaña del Fénix (título: *La Montaña del Fénix*) | como en los diálogos; antes "Montaña de Fénix" en el título |
| Lava Caves | Cuevas de lava | antes "Cuevas lava" en el título (recortado para caber) |
| Masakari Jungle (AREA10 s0) | Jungla Masakari | rótulo del punto de guardado (antes solo "Jungla", el inglés dice "Jungle"): es el nombre del sitio en el archivo de guardado (D5SPSUB2) y en la lista de lugares del submenú (D5SPSUB) |
| Deep Jungle | Jungla profunda | la región del mapa (D5SPSUB) y el Cerdo naranja: *El Cerdo de la Jungla profunda* (antes "Cerdo de la jungla oscura") |
| Old Tree Hill | Colina del árbol viejo | el árbol es "el árbol viejo" en todos los diálogos; antes "Colina del árbol noble" en la lista de lugares, el archivo de guardado y los destinos de viaje |
| Baron's Strength (evento 113) | *El potencial de Barón* | el curandero cree que el cachorro tiene poderes especiales ("puede volar con esas orejas"); antes "Fuerza de Barón" |
| Charles' Pants | Pantalones de Charles (título: *Los pantalones de Charles*) | como el objeto |
| Precious Treasure Chest? | *¿Un cofre del tesoro?* | antes "de tesoro" |
| Pump Rocks | rocas acordeón, en minúscula (título: *Las rocas acordeón*) | el nombre del español original (se hunden al pisarlas); el parche había puesto "Rocas bombeo" |
| Trick Village | Ciudad de trucos | antes "Ciudad trucos" en el título |
| Tree of Knowledge / Wise Old Tree | Árbol del Saber (título y lista: *El Árbol del Saber lo sabe*; pista: "Visita al Árbol del Saber.") | como en todos los diálogos; antes "Árbol del Saber conoce" (recortado) y "Árbol sabio viejo" en la pista |
| Tool (Power Up for Tools?!, evento 116) | arma (*¿¡Armas más fuertes!?*, pista "¿¡Tu arma se vuelve más fuerte!?") | antes "Útil/Útiles"; el japonés dice 道具が強くなる y los diálogos llaman "arma" al mangual, el bumerán...; "herramienta" cuando es de ladrón (pista del evento 125) |
| slices of cheese (evento 47) | trozos de queso ("Reúne 10 trozos de queso y devuélveselos al coleccionista.") | antes "lonchas"; también en el diálogo del coleccionista ("Cuando reúnas diez trozos de queso, vuelve a verme.") |
| Bronze / Silver / Gold Medal | *Quiero la medalla de bronce / de plata / de oro* | los tres títulos iguales; antes "Quiero el bronce", "Quiero la plata" |

## Mensajes de objeto y evento (sin concordancia de género)

El parche activa `verbFirst` (tools/verb_order.py: el verbo se dibuja antes del nombre del objeto y las constantes de
anchura de la caja de EXE/MAIN5.EXE siguen a los textos, `Project.verb_deltas`), así que los verbos son formas
personales que valen para cualquier objeto:

| Diálogo (todos los MFNTxxxS.WFM) | Original | Este parche |
|---|---|---|
| #0 objeto usado | `Usado(a) Rana` | `Usas Rana` |
| #1 objeto equipado | `Equipada Rana` | `Equipas Rana` |
| #2 objeto obtenido | `Rana Ganada` | `Obtienes Rana` |
| CFNT999S #0, cartel de evento completado | `¡DESPEJADA!` | `¡SUPERADO!` |

La `O` mayúscula no existe en la fuente de 16 px de los MFNT: se añade al banco de glifos de cada archivo copiándola de
la fuente del mismo tamaño de otro archivo (igual que hace el editor). En general, si a un archivo le falta un glifo se
añade así en vez de cambiar la frase; `check_text_patch.py` lo lista como nota (`--strict-glyphs` lo convierte en error).
`¡COMPLETADO!` (12 letras) no cabe: el cartel lo dibuja EXE/MAINn.EXE con una tabla de K glifos y K posiciones seguidas
de otros datos (title_layout.banner_find); K = 11 en español, que ya es el más largo del disco (inglés 6 `CLEAR!`,
alemán 10 `GESCHAFFT!`, francés 8, italiano 9...). Ampliarla exigiría mover datos y código del ejecutable; el port
nativo no tiene ese límite (`freeTitles`: el cartel sale del texto #0), la consola sí.

El cartel necesita exactamente 11 letras: con `¡SUPERADO!` (10) el port nativo se quedaba en la pantalla de carga al
entrar en AREA00; `check_text_patch.py` ahora exige el número exacto de letras de la tabla de MAIN5.EXE.

## Títulos de evento sin límite de letras (port nativo)

El parche lleva `"freeTitles": true`: el port nativo coloca las letras de los títulos de evento a partir del texto
(docs/PORT.md "Free-form event titles"), así que un título puede ser más largo que su lista de posiciones del disco y ya
no hace falta recortarlo. En la consola (ISO) se ven solo las primeras letras (`check_text_patch.py` lo avisa). Títulos
alargados o corregidos con esto: *Gas de flor de melocotón*, *Pista de motocross*, *Idioma Enano para novatos*,
*A la Montaña del Fénix...*, *Preparados, listos, ¡ya!*, *Hierbas para curar a Barón*, *El potencial de Barón*,
*Una Bolsa Cerdo tormentosa*, *La Montaña del Fénix*, *Cuevas de lava*, *Los pantalones de Charles*, *¿Un cofre del
tesoro?*, *Zumo de Fruta mortal*, *Planta un jardín de flores*, *La adivina roja*, *¿Cuadro de la Gran llave?*,
*Cerdo de la Jungla profunda*, *Las rocas acordeón*, *Dame el tarro de lágrimas*, *Ciudad de trucos*, *Un manjar para
el Gran pez*, *Quiero la medalla de bronce / de plata / de oro*, *¡¿Echa un vistazo?!*, *¿Tesoros en la mansión?*,
*La fuente rota*, *El señor de los cielos*, *¿Qué es un Funga?*, *El Árbol del Saber lo sabe*, *El Bosque de las 100
flores*, *¿¡Armas más fuertes!?*, *¿El mejor llanto?*, *¿Qué se cuece?*, *Tobogán de hojas*.

La lista de eventos del submenú (D5SPSUB/D5SPSUB3, que también usa el panel) dice lo mismo que los títulos (el salto de
línea del título es un espacio en la lista), con `¡` y `¿` (la fuente de la lista los tiene: `tools/subscreen_text.py`
los escribe). Cada nombre de evento va en `eventNames` (clave: la cadena del disco), así que no toca los nombres de
objeto o lugar iguales: *La Montaña del Fénix* frente al lugar *Montaña del Fénix*, *Los pantalones de Charles* frente
al objeto *Pantalones de Charles*, *Un manjar para el Gran pez* frente a *Comida para el Gran pez*. La lista muestra 27
letras como mucho (fuente de 8 px desde x 103): por eso *A la Montaña del Fénix...*, *¿Cuadro de la Gran llave?*,
*Cerdo de la Jungla profunda* y *Hierbas para curar a Barón*, también en los títulos. Las pistas de evento (el globo
de "Ver" en la lista, banco WFM3 de D5SPSUB) se editan con `eventDescriptions`; el banco no puede crecer (lo siguen las
cadenas de la lista), así que las pistas nuevas deben ocupar más o menos lo mismo que las del disco.

## Lista de lugares del archivo de guardado (SYSTEM/D5SPSUB2.BIN)

Un nombre que no cabe en su hueco ya no se recorta: `tools/memcard_text.py` reempaqueta todo el bloque de nombres de
lugar (los repetidos, como los diez "Castillo de Hierro", se comparten, y eso deja sitio) y reescribe las tablas.
Corregidos: *Cuevas de lava* (antes "Cueva de lava" por falta de sitio), *Nido del Fénix*, *Plaza de la caridad*,
*Cámara del Anciano de 1.000.000 años* (antes "Cámara Anciano 1 millón años"; cabe en la fila como el de los 10.000
años), *Cámara del Anciano milenario*, *Cámara del ladrón uno / dos / tres* (antes "Una cámara del ladrón", "Dos / Tres
cámaras de ladrones": error de traducción de *Thief's Room One/Two/Three*), *Cámara de la cerradura*, *Cámara del
escondite*, *Cámara de la risa*, *Cámara de los trucos*, *Cámara de la sombra*, *Cámara del llanto*, *Muros exteriores
de la Isla de Cerdos* (antes "Muros externos de Isla Cerdos").
