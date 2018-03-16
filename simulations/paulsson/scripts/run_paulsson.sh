
#!/bin/sh

EXE_NAME=bipsim
LOG_FILE=log.txt
SCRIPT_PATH=../scripts
SCRIPT_NAME=paulsson.py
SCRIPT_ARGS="output/chemicals.out ../../data/parametre_simulation.csv"
PARAMS_IN="input/params.in"

rm -f $LOG_FILE
for FILE in $PARAMS_IN
do
    ${EXE_NAME} ${FILE} 2>&1 | tee -a $LOG_FILE
done

python ${SCRIPT_PATH}/${SCRIPT_NAME} ${SCRIPT_ARGS} 2>&1 | tee -a $LOG_FILE
