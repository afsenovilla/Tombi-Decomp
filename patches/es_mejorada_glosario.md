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
| Charity Statues / Flower Tower | Estatuas de caridad / Torre de flores | |
| Ol' Pond | Viejo estanque | |
| Yan of the Hidden Village | Yan, el de la Ciudad oculta | |
| Village | Ciudad (nombres propios: Ciudad Baco, Ciudad de Enanos...) | oficial |
| Mr. Turtle | el Sr. Tortuga | antes "la Sra. Tortuga" |
| Psychic Fish | Pez psíquico | antes "Pez potencia" |
| Blackjack | Porra | antes "Bola" |
| chest (100 year old chests) | cofre (cofres centenarios) | antes "arcón" |
| Directional button | botón de dirección | |
| Exercise Routine | Ejercicio | |
| AP | PA | |
| Kokka birds | pájaros Kokka | |
| Pig / ol' coot / geezer | vejete, vejestorio | registro coloquial |
| Ha ha ha | ¡Ja, ja, ja! | con comas |
| See ya! | ¡Hasta luego! | |
| GrappleJack | Garfio-porra | antes "Garfio-gancho" (Grapple + Blackjack = Porra) |
| Dashing Pants | Pantalón veloz | antes "Pantalón elegante" |
| Funky Parasol | Sombrilla chula | antes "Sombrilla rara" |
| Miner's Hat | Casco de minero | antes "Gorro (del) minero" |
| 1 UP | Vida extra | antes "1 +" |
| Tear Bottle | Tarro lágrimas | también en el título "Necesito un Tarro lágrimas" |

## Mensajes de objeto y evento (sin concordancia de género)

El parche activa `verbFirst` (tools/verb_order.py: el verbo se dibuja antes del nombre del objeto y las constantes de
anchura de la caja de EXE/MAIN5.EXE siguen a los textos, `Project.verb_deltas`), así que los verbos son formas
personales que valen para cualquier objeto:

| Diálogo (todos los MFNTxxxS.WFM) | Original | Este parche |
|---|---|---|
| #0 objeto usado | `Usado(a) Rana` | `Usas Rana` |
| #1 objeto equipado | `Equipada Rana` | `Equipas Rana` |
| #2 objeto obtenido | `Rana Ganada` | `Obtienes Rana` |
| CFNT999S #0, cartel de evento completado | `¡DESPEJADA!` | `¡ESTUPENDO!` |

La `O` mayúscula no existe en la fuente de 16 px de los MFNT: se añade al banco de glifos de cada archivo copiándola de
la fuente del mismo tamaño de otro archivo (igual que hace el editor). En general, si a un archivo le falta un glifo se
añade así en vez de cambiar la frase; `check_text_patch.py` lo lista como nota (`--strict-glyphs` lo convierte en error).
`¡COMPLETADO!` (12 letras) no cabe: el cartel lo dibuja EXE/MAINn.EXE con una tabla de K glifos y K posiciones seguidas
de otros datos (title_layout.banner_find); K = 11 en español, que ya es el más largo del disco (inglés 6 `CLEAR!`,
alemán 10 `GESCHAFFT!`, francés 8, italiano 9...). Ampliarla exigiría mover datos y código del ejecutable.

El cartel necesita exactamente 11 letras: con `¡SUPERADO!` (10) el port nativo se quedaba en la pantalla de carga al
entrar en AREA00; `check_text_patch.py` ahora exige el número exacto de letras de la tabla de MAIN5.EXE.
