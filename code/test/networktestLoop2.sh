#!/bin/bash
cd ../build

echo "Test reseau, partie boucle logique, machine 1"
exec ./nachos-step6 -m 2 -o2 0

exit 0