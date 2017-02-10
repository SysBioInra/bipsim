
#!/bin/sh

SRC_PATH=/home/sfischer/simulateur/myBacteria/src
EXE_NAME=simulator

PARAMS_IN="input/params.in"

LOG_FILE=log.txt

rm -f $LOG_FILE
for FILE in $PARAMS_IN
do
    ${SRC_PATH}/${EXE_NAME} ${FILE} 2>&1 | tee -a $LOG_FILE
done

Rscript post_treatment.R 2>&1 | tee -a $LOG_FILE
