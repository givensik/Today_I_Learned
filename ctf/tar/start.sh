#!/bin/bash

nsjail -Ml --port 11111 -T /tmp --user 99999 --group 99999 --time_limit 15 --chroot /jail -- /bin/sh /run_py.sh