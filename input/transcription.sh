
FILE=transcription.in
rm $FILE

printf "\nunit Chemical RNAP 1000" >> $FILE

# initiation
printf "\n\n# initiation\n" >> $FILE
sigma=("SigA" "SigB")
for factor in ${sigma[@]}
do
    printf "\nunit Chemical %s 1000" $factor >> $FILE
    printf "\nunit Chemical RNAP_%s" $factor >> $FILE
    printf "\nunit BoundChemical bound_RNAP_%s" $factor >> $FILE
    printf "\nunit BoundChemical stable_RNAP_%s" $factor >> $FILE
done

printf "\n" >> $FILE
for factor in ${sigma[@]}
do
    printf "\nreaction Complexation RNAP %s RNAP_%s 1 1" $factor $factor >> $FILE
done

printf "\n" >> $FILE
for factor in ${sigma[@]}
do
    printf "\nreaction Binding RNAP_%s bound_RNAP_%s promoter_%s" $factor $factor $factor >> $FILE
done

printf "\n" >> $FILE
for factor in ${sigma[@]}
do
    printf "\nreaction ChemicalReaction bound_RNAP_%s -1 stable_RNAP_%s 1 rates 1 0" $factor $factor >> $FILE
done

printf "\n" >> $FILE
for factor in ${sigma[@]}
do
    printf "\nreaction Complexation stable_RNAP %s stable_RNAP_%s 0 1" $factor $factor >> $FILE
done

# elongation
printf "\n\n# elongation\n" >> $FILE
printf "\nunit Chemical Mg2+ 1000" >> $FILE
printf "\nunit BoundChemical loaded_RNAP" >> $FILE
printf "\nunit DecodingTable NTP_loading" >> $FILE
printf " A ATP loaded_RNAP 1" >> $FILE
printf " C CTP loaded_RNAP 1" >> $FILE
printf " G GTP loaded_RNAP 1" >> $FILE
printf " T UTP loaded_RNAP 1" >> $FILE
printf "\nunit BaseLoader stable_RNAP NTP_loading" >> $FILE
printf "\nunit BoundChemical stalled_RNAP" >> $FILE
printf "\nunit ProcessiveChemical translocating_RNAP stalled_RNAP hairpin" >> $FILE

printf "\n" >> $FILE
printf "\nreaction BaseLoading stable_RNAP" >> $FILE
printf "\nreaction ChemicalReaction loaded_RNAP -1 Mg2+ -2 translocating_RNAP 1 PPi 1 Mg2+ 2 rates 1 0" >> $FILE
printf "\nreaction Elongation translocating_RNAP stable_RNAP 1 6" >> $FILE

#termination
printf "\n\n# termination\n" >> $FILE
printf "\nunit Chemical NutA 100" >> $FILE
printf "\nunit Chemical rna" >> $FILE
printf "\nreaction Release stalled_RNAP NutA -1 NutA 1 RNAP 1 rna 1 rate 1 produces rnas" >> $FILE
