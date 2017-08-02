
#!/bin/sh

SRC_PATH=/home/sfischer/simulateur/myBacteria/src
SCRIPT_PATH=/home/sfischer/simulateur/simulations/scripts
EXE_NAME=mybacteria
LOG_FILE=log.txt
SCRIPT_NAME=paulsson.py
SCRIPT_ARGS="output/concentrations.out ../data/input/parametre_simulation.csv"
PARAMS_IN="input/params.in"

rm -f $LOG_FILE
for FILE in $PARAMS_IN
do
    ${SRC_PATH}/${EXE_NAME} ${FILE} 2>&1 | tee -a $LOG_FILE
done

python ${SCRIPT_PATH}/${SCRIPT_NAME} ${SCRIPT_ARGS} 2>&1 | tee -a $LOG_FILE
