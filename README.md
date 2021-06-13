# P2-SSOO-RR
**1. Paquetes recibidos por el servidor**
* code = 0: se recibe el nombre del jugador
* code = 1: se recibe la clase del jugador
* code = 2: indica que el lider quiere comenzar la partida, payload = enemigo
* code = -1: el cliente se desea desconectar 

**2. Paquetes recibidos por el cliente**
* code = 2: cliente recibe el mensaje de bienvenida del servidor
* code = 3: notificacion para el lider de que se ha conectado un nuevo jugador, payload = nombreJugador-claseJugador
* code= 4: notificación para el lider, indica que no se puede comenzar a jugar porque hay jugadores conectados que no han ingresado su nombre
* code=5: notificacion a todos los clientes de que comienza el juego, payload = nombre del enemigo

