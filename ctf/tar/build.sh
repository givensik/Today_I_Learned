#!/bin/sh

docker rm -f tar
docker rmi -f tar

docker build -t tar .
docker run -d --name tar tar tail -f /dev/null

docker rm -f tar