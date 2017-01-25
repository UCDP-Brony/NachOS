#!/bin/bash
cd ../build

echo "Test thread, avec Join, SynchPutChar. Doit afficher des lettres de a à j en désordre."
exec ./nachos-step6 -rs -x ./userThreadJoinChar

exit 0