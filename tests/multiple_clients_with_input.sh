#!/bin/bash

# get current directory
DIR="$( cd "$( dirname "$0" )" && pwd )"

# create new windows and call client application
cd $DIR/../scripts && ./create-client-input.sh $DIR/.. $1


# call server application
cd $DIR/.. && ./server 9898