
#!/bin/sh

SRC_PATH=/home/sfischer/simulateur/myBacteria/src
EXE_NAME=simulator

PARAMS_IN=input/params.in

${SRC_PATH}/${EXE_NAME} ${PARAMS_IN}
# valgrind --tool=callgrind ${SRC_PATH}/${EXE_NAME} ${PARAMS_IN}
# valgrind --leak-check=full ${SRC_PATH}/${EXE_NAME} ${PARAMS_IN}
