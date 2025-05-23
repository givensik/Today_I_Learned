#!/bin/bash

docker rm -f tar
docker run -d -p 32496:11111 --name tar --restart unless-stopped --privileged tar