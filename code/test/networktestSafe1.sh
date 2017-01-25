#!/bin/bash
cd ../build

echo "Test du reseau, partie reseau avec perte de paquets, machine 1, serveur, 30% de pertes. Ce programme restera en attente jusqu'à arrêt de machine "
exec ./nachos-step6 -m 1 -o4 0

exit 0