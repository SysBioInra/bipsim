
CURR_DIR=`pwd`
LOG_FILE=${CURR_DIR}"/log_application_note.txt"
EXP_DIR="AN_agregated_constant AN_agregated_stacked AN_hybrid_constant AN_hybrid_stacked AN_detailed_constant AN_detailed_stacked AN_detailed_detailed paulsson_agregated paulsson_detailed"

rm -f $LOG_FILE
for DIR in $EXP_DIR
do
    cd $DIR
    sh run.sh 2>&1 | tee -a $LOG_FILE
    cd $CURR_DIR
done
