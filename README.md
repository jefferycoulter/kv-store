to build, run `make` in the base directory.

**on macOS**: in the `tests` folder, there are two scripts, one for a single client connection and one for "multiple" (two) clients.  just open a terminal window in the `tests` directory and run `./single_client.sh` or 
`./multiple_clients.sh [<number of clients>]` which both launch the server from IP 127.0.0.1 and port 9898.  these scripts will launch the server in the base terminal window and create a new window for each of the clients to be launched in (note that the server is written [line 52] to handle up to five clients at a time).  you can then enter `set [<key>] [<value>]` or `get [<key>]` commands from the client windows.  to disconnect a client, enter `:q` and this will disconnect from the server and exit the client program. <br />

**on linux**: since the above tests use applescript in some places, they only work on macOS. i couldn't download xdotools or get xterm to work with the ssh linux terminals like silo or shark. i also couldn't figure out how to automate the process with `screen`, so you will have to open new terminals yourself to launch multiple clients and enter the input manually.

the key-values are stored in `kv-store.txt`
