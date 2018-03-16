
CURR_DIR=`pwd`
LOG_FILE=${CURR_DIR}"/log_simulations.txt"
EXP_DIR="gene_expression/agregated_constant gene_expression/agregated_stacked gene_expression/hybrid_constant gene_expression/hybrid_stacked gene_expression/detailed_constant gene_expression/detailed_stacked gene_expression/detailed_detailed"

rm -f $LOG_FILE
for DIR in $EXP_DIR
do
    cd $DIR
    sh run.sh 2>&1 | tee -a $LOG_FILE
    cd $CURR_DIR
done
