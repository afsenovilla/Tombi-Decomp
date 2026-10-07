# Variables en el scratchpad (0x1F800000, RAM rapida de 1 KB) usadas por MAIN0
El juego guarda aqui sus globales mas calientes (Ghidra las llama `DAT_1f8001xx`).

| Direccion | Uso (por confirmar) |
|---|---|
| 1F800164 | puntero a la siguiente primitiva libre |
| 1F8001C8 | contador de frames (bit 0 se usa para alternar buffers) |
| 1F8001D4 | puntero a la entrada actual de la tabla de hilos |
| 1F8001E0 | tabla de ordenacion (OT) del buffer actual |
| 1F8001E4 | OT del buffer anterior |
| 1F8001F4 | indice del buffer de doble buffer (0/1) |
| 1F800208 | puntero a la lista de objetos libres (pool) |
| 1F800238 | numero de objetos libres |

Otras estructuras (RAM normal): doble buffer de dibujo en 8009D6A8 (stride 0xD10),
tabla de hilos en 801FD800 (0x38 bytes por entrada, 6 entradas), 24 voces de sonido.
