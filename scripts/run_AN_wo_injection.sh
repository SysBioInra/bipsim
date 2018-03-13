
#!/bin/sh

SRC_PATH=/home/sfischer/simulateur/myBacteria/src
SCRIPT_PATH=/home/sfischer/simulateur/simulations/scripts
EXE_NAME=mybacteria
LOG_FILE=log.txt
SCRIPT_NAME=AN_post_treatment.R
PARAMS_IN="input/params.in"
OUTPUT="output"

# run simulations
rm -f $LOG_FILE
for FILE in ${PARAMS_IN}
do
    ${SRC_PATH}/${EXE_NAME} ${FILE} | tee -a $LOG_FILE
done

# post treatment
Rscript ${SCRIPT_PATH}/${SCRIPT_NAME} ${OUTPUT} | tee -a $LOG_FILE

