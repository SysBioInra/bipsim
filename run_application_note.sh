
CURR_DIR=`pwd`
LOG_FILE=${CURR_DIR}"/log_application_note.txt"
EXP_DIR="AN_detailed AN_hybrid AN_agregated paulsson_agregated paulsson_detailed"

rm -f $LOG_FILE
for DIR in $EXP_DIR
do
    cd $DIR
    sh run.sh 2>&1 | tee -a $LOG_FILE
    cd $CURR_DIR
done
