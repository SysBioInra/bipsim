
FILE=transcription.in
rm $FILE

printf "\n" >> $FILE
printf "\n######################" >> $FILE
printf "\n#       UNITS        #" >> $FILE
printf "\n######################" >> $FILE
printf "\n#" >> $FILE
printf "\nFreeChemical RNAP 1000" >> $FILE

# initiation
printf "\n" >> $FILE
printf "\n# initiation" >> $FILE
sigma=("SigA" "SigB")
for factor in ${sigma[@]}
do
    printf "\nFreeChemical %s 1000" $factor >> $FILE
    printf "\nFreeChemical RNAP_%s" $factor >> $FILE
    printf "\nBoundChemical bound_RNAP_%s" $factor >> $FILE
    printf "\nBoundChemical stable_RNAP_%s" $factor >> $FILE
done

# elongation
printf "\n" >> $FILE
printf "\n# elongation" >> $FILE
printf "\nFreeChemical Mg2+ 1000" >> $FILE
printf "\nBoundChemical loaded_RNAP" >> $FILE
printf "\nLoadingTable NTP_loading A ATP loaded_RNAP 1" >> $FILE
printf ", C CTP loaded_RNAP 1" >> $FILE
printf ", G GTP loaded_RNAP 1" >> $FILE
printf ", T UTP loaded_RNAP 1" >> $FILE
printf "\nBoundChemical stable_RNAP" >> $FILE
printf "\nBoundChemical stalled_RNAP" >> $FILE
printf "\nBoundChemical translocating_RNAP" >> $FILE

#termination
printf "\n" >> $FILE
printf "\n# termination" >> $FILE
printf "\nFreeChemical NutA 100" >> $FILE
printf "\nFreeChemical rna" >> $FILE

printf "\n" >> $FILE
printf "\n######################" >> $FILE
printf "\n#     REACTIONS      #" >> $FILE
printf "\n######################" >> $FILE
printf "\n#" >> $FILE

# initiation
printf "\n" >> $FILE
printf "\n# initiation" >> $FILE
for factor in ${sigma[@]}
do
    printf "\nChemicalReaction RNAP -1 %s -1 RNAP_%s 1 rates 1 1" $factor $factor >> $FILE
done
printf "\n" >> $FILE
for factor in ${sigma[@]}
do
    printf "\nSequenceBinding RNAP_%s bound_RNAP_%s promoter_%s" $factor $factor $factor >> $FILE
done
printf "\n" >> $FILE
for factor in ${sigma[@]}
do
    printf "\nChemicalReaction bound_RNAP_%s -1 stable_RNAP_%s 1 rates 1 0" $factor $factor >> $FILE
done
printf "\n" >> $FILE
for factor in ${sigma[@]}
do
    printf "\nChemicalReaction stable_RNAP_%s -1 stable_RNAP 1 %s 1 rates 1 0" $factor $factor >> $FILE
done

# elongation
printf "\n" >> $FILE
printf "\n# elongation" >> $FILE
printf "\nProductLoading stable_RNAP NTP_loading" >> $FILE
printf "\nChemicalReaction loaded_RNAP -1 Mg2+ -2 translocating_RNAP 1 PPi 1 Mg2+ 2 rates 1 0" >> $FILE
printf "\nTranslocation translocating_RNAP stable_RNAP stalled_RNAP 1 6 hairpin" >> $FILE

# termination
printf "\n" >> $FILE
printf "\n# termination" >> $FILE
printf "\nRelease stalled_RNAP NutA -1 NutA 1 RNAP 1 rna 1 rate 1 produces rnas" >> $FILE
