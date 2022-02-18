#!/bin/bash

# get current directory
DIR="$( cd "$( dirname "$0" )" && pwd )"

# create new window and call client application
cd $DIR/../scripts && ./create-client.sh $DIR/..

# call server application
cd $DIR/.. && ./server 9898