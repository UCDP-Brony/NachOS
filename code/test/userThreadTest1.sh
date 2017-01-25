#!/bin/bash
cd ../build

echo "Test thread, sans Join, SynchPutChar. Ne doit rien afficher."
exec ./nachos-step6 -rs -x './userThreadChar'

exit 0