#!/bin/bash

# get current directory
DIR="$( cd "$( dirname "$0" )" && pwd )"

# create new windows and call client application
for ((i=0; i<$1; i++)); do
    cd $DIR/../scripts && ./create-client.sh $DIR/..
done

# call server application
cd $DIR/.. && ./server 9898