
#!/bin/sh

SRC_PATH=/home/sfischer/simulateur/myBacteria/src
SCRIPT_PATH=/home/sfischer/simulateur/simulations/scripts
EXE_NAME=mybacteria
LOG_FILE=log.txt
SCRIPT_NAME=paulsson_post_treatment.R
PARAMS_IN="input/params.in"

rm -f $LOG_FILE
for FILE in $PARAMS_IN
do
    ${SRC_PATH}/${EXE_NAME} ${FILE} 2>&1 | tee -a $LOG_FILE
done

Rscript ${SCRIPT_PATH}/${SCRIPT_NAME} 2>&1 | tee -a $LOG_FILE
