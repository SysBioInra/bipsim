
#!/bin/sh

EXE_NAME=bipsim
LOG_FILE=log.txt
SCRIPT_PATH=../scripts
SCRIPT_NAME=post_treatment.R
PARAMS_IN="input/params.in"
OUTPUT="output"

# run simulations
rm -f $LOG_FILE
for FILE in ${PARAMS_IN}
do
    ${EXE_NAME} ${FILE} | tee -a $LOG_FILE
done

# post treatment
Rscript ${SCRIPT_PATH}/${SCRIPT_NAME} ${OUTPUT} | tee -a $LOG_FILE
