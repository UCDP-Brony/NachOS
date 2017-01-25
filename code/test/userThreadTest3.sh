#!/bin/bash
cd ../build

echo "Test thread, avec Join, SynchPutString. Doit afficher les lettre de a à j dans l'ordre par groupe de 5."
exec ./nachos-step6 -rs -x ./userThreadJoinString

exit 0