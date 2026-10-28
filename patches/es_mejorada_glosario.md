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
