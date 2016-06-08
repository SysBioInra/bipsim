FILE=Makefile.am
rm -f $FILE

############################# LINE TO MODIFY ##################################
PROGRAMS=("biasedwheel_test" "inputdata_test" "randomhandler_test" \
    "transformationtable_test" "producttable_test" "cellstate_test" \
    "compositiontable_test" "ratetree_test" "vectorlist_test" \
    "rategroup_test" "hybridratecontainer_test" "partialstrand_test" \
    "sitegroup_test" "sequenceoccupation_test" "freechemical_test" \
    "boundchemical_test")
###############################################################################

printf "\n" >> $FILE
printf "#########################\n" >> $FILE
printf "#         FLAGS         #\n" >> $FILE
printf "#########################\n" >> $FILE
printf "#\n" >> $FILE
printf '''AM_CPPFLAGS = -I$(top_srcdir)/src $(BOOST_CPPFLAGS)\n''' >> $FILE
printf '''AM_LDFLAGS = $(BOOST_LDFLAGS)\n''' >> $FILE
printf "\n" >> $FILE
printf "#########################\n" >> $FILE
printf "# THE PROGRAMS TO BUILD #\n" >> $FILE
printf "#########################\n" >> $FILE
printf "#\n" >> $FILE
printf "# the programs to build (the names of the final binaries)\n" >> $FILE
printf "check_PROGRAMS =\n" >> $FILE
printf "check_PROGRAMS +=" >> $FILE
for program in ${PROGRAMS[@]}
do
    printf " %s" $program >> $FILE
done
printf "\n\n" >> $FILE
printf '''# declare programs to be part of `make check`\n''' >> $FILE
printf '''TESTS = $(check_PROGRAMS)\n''' >> $FILE
printf "\n" >> $FILE
printf "# we use the convenience library created in src to link to the simulator\n" >> $FILE
for program in ${PROGRAMS[@]}
do
    printf "%s_SOURCES = %s.cpp\n" $program $program >> $FILE
    printf '''%s_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libsrc.a\n''' $program >> $FILE
    printf "\n" >> $FILE
done
