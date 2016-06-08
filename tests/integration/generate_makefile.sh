
FILE=Makefile.am
rm -f $FILE

############################# LINE TO MODIFY ##################################
TEST_DIRECTORIES=("tRNA_consumption")
###############################################################################

printf "\n" >> $FILE
printf "#########################\n" >> $FILE
printf "#         FLAGS         #\n" >> $FILE
printf "#########################\n" >> $FILE
printf "#\n" >> $FILE
printf '''AM_CPPFLAGS = $(BOOST_CPPFLAGS) -I$(top_srcdir)/src\n''' >> $FILE
printf '''AM_LDFLAGS = $(BOOST_LDFLAGS)\n''' >> $FILE
printf "\n" >> $FILE
printf "#########################\n" >> $FILE
printf "# THE PROGRAMS TO BUILD #\n" >> $FILE
printf "#########################\n" >> $FILE
printf "#\n" >> $FILE
printf "# the programs to build (the names of the final binaries)\n" >> $FILE
printf "check_PROGRAMS =\n" >> $FILE
printf "check_PROGRAMS +=" >> $FILE
for DIR in ${TEST_DIRECTORIES[@]}
do
    printf " %s/test" $DIR >> $FILE
done
printf "\n\n" >> $FILE
printf '''# declare scripts to be used by `make check`\n''' >> $FILE
printf "TESTS =\n" >> $FILE
printf "TESTS +=" >> $FILE
for DIR in ${TEST_DIRECTORIES[@]}
do
    printf " %s/run_test.sh" $DIR >> $FILE
done
printf "\n\n" >> $FILE
printf "# we use the convenience library created in src to link to the simulator\n" >> $FILE
for DIR in ${TEST_DIRECTORIES[@]}
do
    printf "%s_test_SOURCES = %s/test.cpp\n" $DIR $DIR >> $FILE
    printf '''%s_test_LDADD = $(BOOST_UNIT_TEST_FRAMEWORK_LIB) $(top_srcdir)/src/libsrc.a\n''' $DIR >> $FILE
    printf "\n" >> $FILE
done
