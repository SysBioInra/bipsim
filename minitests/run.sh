
#!/bin/sh

SRC_PATH=/home/sfischer/simulateur/myBacteria/src
EXE_NAME=mybacteria
PARAMS_IN=input/params.in
LOG_FILE=log.txt

rm -rf ${LOG_FILE}
${SRC_PATH}/${EXE_NAME} ${PARAMS_IN} 2>&1 | tee -a ${LOG_FILE}
# valgrind --tool=callgrind ${SRC_PATH}/${EXE_NAME} ${PARAMS_IN}
#valgrind --leak-check=full --show-leak-kinds=all ${SRC_PATH}/${EXE_NAME} ${PARAMS_IN}
