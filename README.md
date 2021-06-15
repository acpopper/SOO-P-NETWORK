# P2-SSOO-RR
**1. Paquetes recibidos por el servidor**
* code = 0: se recibe el nombre del jugador
* code = 1: se recibe la clase del jugador
* code = 2: indica que el si el lider quiere comenzar la partida, payload = 0: no quiere iniciar aun o enemigo: 1,2 o 3
* code = -1: el cliente se desea desconectar 

**2. Paquetes recibidos por el cliente**
* code = 2: cliente recibe el mensaje de bienvenida del servidor
* code = 3: notificacion para el lider de que se ha conectado un nuevo jugador, payload = nombreJugador-claseJugador
* code= 4: notificación para el lider, payload indica codigo de error al comenzar la partida, payload = 0: no se puede comenzar porque no todos los jugadores conectados han ingresado su nombre, payload = 1: el lider debe comenzar la partida porque ya esta el max posibles aunque el haya decidido esperar por mas jugadores
* code=5: notificacion a todos los clientes de que comienza el juego, payload = nombre del enemigo
* code=100: conexión rechazada debido a que ya se completo el número de jugadores

**Supuestos**
1. Si ya se completo el máximo de jugadores, la conexión del proximo cliente que intente conectarse será rechazada
2. si el lider decide no comenzar la partida y ya esta la cantidad de jugadores maxima, el servidor le manda un mensaje diciendo que debe comenzar e inmediatamente le pide que ingrese el enemigo.


