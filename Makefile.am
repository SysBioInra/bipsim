
# this is file: Makefile.am used by configure to generate Makefiles

# the subdirectories of the project to go into
SUBDIRS = src tests

ACLOCAL_AMFLAGS = -I m4

# add sample directory to distribution
SAMPLE_DIR=sample
SAMPLE_INPUT_DIR=$(SAMPLE_DIR)/input
SAMPLE_OUTPUT_DIR=$(SAMPLE_DIR)/output
SAMPLE_MAIN_FILES=format.in params.in params_tetracyclin.in run.sh README
SAMPLE_INPUT_FILES=dna.in metabolites.in proteins.in replication.in rnas.in tetracyclin.in transcription.in translation.in
dist-hook:
	for d in $(SAMPLE_DIR) $(SAMPLE_INPUT_DIR) $(SAMPLE_OUTPUT_DIR); do \
		mkdir $(distdir)/$$d; \
	done
	for f in $(SAMPLE_MAIN_FILES); do \
		cp $(SAMPLE_DIR)/$$f $(distdir)/$(SAMPLE_DIR); \
	done
	for f in $(SAMPLE_INPUT_FILES); do \
		cp $(SAMPLE_INPUT_DIR)/$$f $(distdir)/$(SAMPLE_INPUT_DIR); \
	done

# boost
#EXTRA_DIST = bootstrap

