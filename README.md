# Baseball-Game
Baseball-Game using semaphore and sharing memory(Memory map) in C.

## How to start?

1) Complie two C files in src folders.

gcc -o Server server.c -lpthread</br>
gcc -o Client client.c -lpthread

2) Start Server program first.

./Server (Terminal 1)

3) Next, Start Client program twice at different terminal.

./Client (Terminal 2)</br>
./Client (Terminal 3)

4) Enjoy the game!
