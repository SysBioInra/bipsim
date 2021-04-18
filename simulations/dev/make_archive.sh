# run this script on a freshly checked out repo to avoid copying results file etc.

mkdir simulation_archive

cp README_archive.md simulation_archive/README.md

mkdir simulation_archive/gene_expression
EXP_DIR="aggregated_constant aggregated_stacked hybrid_constant hybrid_stacked detailed_constant detailed_stacked detailed_detailed"

for DIR in $EXP_DIR
do
    mkdir simulation_archive/gene_expression/$DIR
    mkdir simulation_archive/gene_expression/$DIR/output
    cp -r ../gene_expression/$DIR/input simulation_archive/gene_expression/$DIR/
done

mkdir simulation_archive/paulsson
mkdir simulation_archive/paulsson/output
cp -r ../paulsson/minimal/input simulation_archive/paulsson/

mkdir simulation_archive/multifork
mkdir simulation_archive/multifork/output
cp -r ../multifork/input simulation_archive/multifork/

cp -r ../guided_example simulation_archive/
mkdir simulation_archive/guided_example/output

cp -r ../tbox_leader simulation_archive/
mkdir simulation_archive/tbox_leader/output

cp -r ../gupta_benchmark simulation_archive/
cp -r ../ncascades simulation_archive/


