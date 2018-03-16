
#!/bin/sh

EXE_NAME=mybacteria
LOG_FILE=log.txt
SCRIPT_PATH=../scripts
SCRIPT_NAME=multifork.R
OUTPUT=output
PARAMS_IN="input/params.in"

rm -f $LOG_FILE
for FILE in $PARAMS_IN
do
    ${EXE_NAME} ${FILE} 2>&1 | tee -a $LOG_FILE
done

# post treatment
Rscript ${SCRIPT_PATH}/${SCRIPT_NAME} ${OUTPUT} | tee -a $LOG_FILE
