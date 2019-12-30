#!/usr/bin/env sh
set -x
ssh -o BatchMode=yes -o StrictHostKeyChecking=no stone@88.198.86.0 "uptime"
scp -o StrictHostKeyChecking=no $1 stone@88.198.86.0:/var/www/files.worxcoin.io/htdocs/stone/releases/

