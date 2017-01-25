#!/bin/bash
cd ../build

echo "Test du reseau, partie reseau avec perte de paquets, machine 0, client, 30% de pertes "
exec ./nachos-step6 -m 0 -o3 1 -l 0.7

exit 0