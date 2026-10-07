# Estructura de objeto del juego (deducida de MAIN0 + X000; todo por confirmar)

Las funciones reciben un puntero `obj` y acceden por desplazamiento. Campos vistos:

| Off | Tam | Uso |
|---|---|---|
| +0x00 | u8 | objeto activo (0 = libre) |
| +0x01 | u8 | visible (lo pone ObjCullRegister) |
| +0x02 | u8 | tipo |
| +0x03 | u8 | subtipo |
| +0x06 | u8 | subestado (switch de las maquinas de estados) |
| +0x10 | s32 | coordenada de eje A, 16.16 (la parte entera esta en +0x12) |
| +0x14 | s32 | Y, 16.16 (parte entera en +0x16) |
| +0x18 | s32 | coordenada de eje B, 16.16 (parte entera en +0x1A) |
| +0x1C | u8 | categoria (&0x7F: 1..8 = lista de visibles) |
| +0x24 | ptr | script de animacion actual (entradas de 8 bytes; +6 = duracion con flags 0x4000/0x8000/0xC000) |
| +0x28 | ptr | tabla de movimiento por fotograma |
| +0x2C | u16 | temporizador de animacion |
| +0x2E | u16 | indice de fotograma / orientacion |
| +0x40 | ptr | puntero a la coordenada "horizontal" (apunta a +0x10 o +0x18) |
| +0x44 | ptr | puntero a la coordenada "profundidad" (apunta al otro) |
| +0x6C..+0x72 | 4 x s16 | caja de colision (desplazamientos) |
| +0x7E | s16 | velocidad Y |
| +0x80 | s16 | velocidad horizontal |
| +0x82 | s16 | velocidad (Y/profundidad) |

## Eje horizontal intercambiable
`ObjAlloc` hace que +0x40/+0x44 apunten a +0x10/+0x18 **o al reves** segun el bit 0 de la
variable de scratchpad `1F8001C8`. Es decir, el juego (2.5D) puede intercambiar el eje X y el Z
de todos los objetos con un solo bit. Por eso 1F8001C8 es una orientacion de camara/nivel y
no un contador de frames como supuse al principio.
