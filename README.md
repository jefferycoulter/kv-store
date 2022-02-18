to build, run `make` in the base directory.

it was originally compiled on mac and it's basic c, so it should probably work on linux as is. <br />

in the `tests` folder, there are two scripts, one for a single client connection and one for "multiple" (two) clients.  just open a terminal window in the `tests` directory and run `./single_client.sh` or 
`./multiple_client.sh [<number of clients>]` which both laungh the server from IP 127.0.0.1 and port 9898.  these scripts will launch the server in the base terminal window and create a new window for each of the clients to be launched in (note that the server is written [line 52] to handle up to five clients at a time).  you can then enter `set [<key>] [<value>]` or `get [<key>]` commands from the client windows.  to disconnect a client, enter `:q` and this will disconnect from the server and exit the client program. <br />

the key-values are stored in `kv-store.txt`
