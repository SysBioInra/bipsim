
#!/bin/sh

SRC_PATH=/home/sfischer/simulateur/myBacteria/src
SCRIPT=/home/sfischer/simulations/scripts
EXE_NAME=simulator

PARAMS_IN="input/params.in"
OUTPUT=output

LOG_FILE=log.txt

rm -f $LOG_FILE
for FILE in $PARAMS_IN
do
    ${SRC_PATH}/${EXE_NAME} ${FILE} | tee -a $LOG_FILE
done

Rscript ${SCRIPT} ${OUTPUT} | tee -a $LOG_FILE
