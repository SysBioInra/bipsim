# run this script on a freshly checked out repo to avoid copying results file etc.

SIM_DIR=simulation-1.1
mkdir $SIM_DIR

cp README_archive.md $SIM_DIR/README.md

mkdir $SIM_DIR/gene_expression
EXP_DIR="aggregated_constant aggregated_stacked hybrid_constant hybrid_stacked detailed_constant detailed_stacked detailed_detailed"

for DIR in $EXP_DIR
do
    mkdir $SIM_DIR/gene_expression/$DIR
    mkdir $SIM_DIR/gene_expression/$DIR/output
    cp -r ../gene_expression/$DIR/input $SIM_DIR/gene_expression/$DIR/
done

mkdir $SIM_DIR/paulsson
mkdir $SIM_DIR/paulsson/output
cp -r ../paulsson/minimal/input $SIM_DIR/paulsson/

mkdir $SIM_DIR/multifork
mkdir $SIM_DIR/multifork/output
cp -r ../multifork/input $SIM_DIR/multifork/

cp -r ../guided_example $SIM_DIR/
mkdir $SIM_DIR/guided_example/output
mkdir $SIM_DIR/guided_example/output/annotations
mkdir $SIM_DIR/guided_example/output/binding
mkdir $SIM_DIR/guided_example/output/free_chemicals
mkdir $SIM_DIR/guided_example/output/polymerization
mkdir $SIM_DIR/guided_example/output/regulation
mkdir $SIM_DIR/guided_example/output/sequences

cp -r ../tbox_leader $SIM_DIR/
mkdir $SIM_DIR/tbox_leader/output

cp -r ../gupta_benchmark $SIM_DIR/
cp -r ../ncascades $SIM_DIR/

tar czf $SIM_DIR.tar.gz $SIM_DIR
rm -rf $SIM_DIR