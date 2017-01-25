#!/bin/bash
cd ../build

echo "Test reseau, partie boucle logique, machine 0, origine "
exec ./nachos-step6 -m 0 -o2 1

exit 0