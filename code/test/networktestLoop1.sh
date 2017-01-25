#!/bin/bash
cd ../build

echo "Test reseau, partie boucle logique, machine 1"
exec ./nachos-step6 -m 1 -o2 2

exit 0