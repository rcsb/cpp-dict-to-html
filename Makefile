#
#        CIFOBJ-DICT2HTML module makefile
#
#----------------------------------------------------------------------------
# Project specific path defintions.
#----------------------------------------------------------------------------
M_INCL_DIR       = ../include
M_LIB_DIR        = ../lib
M_BIN_DIR        = ../bin

PROJ_DIR         = .

L_INCL_DIR       = $(PROJ_DIR)/include
SRC_DIR          = $(PROJ_DIR)/src
OBJ_DIR          = $(PROJ_DIR)/obj
L_LIB_DIR        = $(PROJ_DIR)/lib
L_BIN_DIR        = $(PROJ_DIR)/bin
TEST_DIR         = $(PROJ_DIR)/test
TEST_RES_DIR     = $(PROJ_DIR)/test-html
ICONS_DIR        = $(PROJ_DIR)/icons
HTML_INCLUDE_DIR = $(PROJ_DIR)/html-includes

VPATH = $(OBJ_DIR) $(L_BIN_DIR)

#----------------------------------------------------------------------------
# LINCLUDES and LDEFINES are appended to CFLAGS and C++FLAGS
#----------------------------------------------------------------------------
LDEFINES  =
LINCLUDES = -I$(L_INCL_DIR) -I$(M_INCL_DIR)

#----------------------------------------------------------------------------
# Include the appropriate compiler/platform definitions ...
#----------------------------------------------------------------------------
include ../etc/Makefile.platform

# Dependent libraries
CIF_FILE_UTIL_LIB = $(M_LIB_DIR)/cif-file-util.a
CIF_FILE_LIB      = $(M_LIB_DIR)/cif-file.a
CIFPARSE_LIB      = $(M_LIB_DIR)/cifparse-obj.a
TABLES_LIB        = $(M_LIB_DIR)/tables.a
COMMON_LIB        = $(M_LIB_DIR)/common.a
REGEX_LIB         = $(M_LIB_DIR)/regex.a

ALL_DEP_LIBS = $(CIF_FILE_UTIL_LIB) $(CIF_FILE_LIB) $(CIFPARSE_LIB) \
  $(TABLES_LIB) $(COMMON_LIB) $(REGEX_LIB)

# Module libraries
MOD_LIB = dict2html.a

# Aggregate library
AGR_LIB = all.a

# Temporary library. Used to obtain the agregate library.
TMP_LIB = tmp.a

L_MOD_LIB = $(L_LIB_DIR)/$(MOD_LIB)
M_MOD_LIB = $(M_LIB_DIR)/$(MOD_LIB)
L_AGR_LIB = $(L_LIB_DIR)/$(AGR_LIB)
M_AGR_LIB = $(M_LIB_DIR)/$(AGR_LIB)

# Base main file names. Must have ".ext" at the end of the file.
BASE_MAIN_FILES = dict2HTML.ext

# Base other file names. Must have ".ext" at the end of the file.
BASE_OTHER_FILES =

# Main source files. Replace ".ext" with ".C"
SRC_MAIN_FILES = ${BASE_MAIN_FILES:.ext=.C}

# Other source files. Replace ".ext" with ".C"
SRC_BASE_OTHER_FILES = ${BASE_OTHER_FILES:.ext=.C}

SRC_EXTRA_FILES = demoGetArgs.C \
                  htmlUtil.C

SRC_OTHER_FILES = $(SRC_BASE_OTHER_FILES) $(SRC_EXTRA_FILES)

SRC_FILES = $(SRC_MAIN_FILES) $(SRC_OTHER_FILES)

# Main object files. Replace ".ext" with ".o"
OBJ_MAIN_FILES = ${BASE_MAIN_FILES:.ext=.o}

# Other object files. Replace ".C" with ".o"
OBJ_OTHER_FILES = ${SRC_OTHER_FILES:.C=.o}

OBJ_FILES = $(OBJ_MAIN_FILES) $(OBJ_OTHER_FILES)

# Base header files. Replace ".ext" with ".h"
HADER_BASE_OTHER_FILES = ${BASE_OTHER_FILES:.ext=.h}

EXTRA_HEADER_FILES = htmlUtil.h

HEADER_FILES = $(HADER_BASE_OTHER_FILES) $(EXTRA_HEADER_FILES)

ALL_OBJ_FILES = *.o

# Executables. Remove ".ext"
TARGETS = ${BASE_MAIN_FILES:.ext=}

# Scripts
TARGET_SCRIPTS = DictToHTML.csh

# Test related files
TEST_FILES = $(TEST_DIR)/test.sh \
             $(TEST_DIR)/mmcif_ddl.dic \
             $(TEST_DIR)/mmcif_std.dic

# HTML include files
HTML_INCLUDE_FILES = $(HTML_INCLUDE_DIR)/footer.txt \
                     $(HTML_INCLUDE_DIR)/header.txt

.PHONY: ../etc/Makefile.platform all install test export clean clean_build clean_test

.PRECIOUS: $(OBJ_DIR)/%.o

# All
all: install


# Installation
install: $(TARGETS)


# Test
test: all
	@sh -c 'cd $(TEST_DIR); ./test.sh'

export:
	mkdir -p $(EXPORT_DIR)
	@cp Makefile $(EXPORT_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(L_INCL_DIR)
	@cd $(L_INCL_DIR); $(EXPORT) $(EXPORT_LIST) $(HEADER_FILES) ../$(EXPORT_DIR)/$(L_INCL_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(SRC_DIR)
	@cd $(SRC_DIR); $(EXPORT) $(EXPORT_LIST) $(SRC_FILES) ../$(EXPORT_DIR)/$(SRC_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(OBJ_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(L_LIB_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(L_BIN_DIR)
	@cd $(L_BIN_DIR); cp $(TARGET_SCRIPTS) ../$(EXPORT_DIR)/$(L_BIN_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(TEST_DIR)
	@cp $(TEST_FILES) $(EXPORT_DIR)/$(TEST_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(TEST_RES_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(ICONS_DIR);
	@cp $(ICONS_DIR)/*.gif $(EXPORT_DIR)/$(ICONS_DIR)
	@cd $(EXPORT_DIR); mkdir -p $(HTML_INCLUDE_DIR);
	@cp $(HTML_INCLUDE_FILES) $(EXPORT_DIR)/$(HTML_INCLUDE_DIR)


clean: clean_build clean_test


# Rule for making executables
%: $(OBJ_DIR)/%.o $(M_MOD_LIB) $(ALL_DEP_LIBS)
	$(CCC) $(LDFLAGS) $< $(M_MOD_LIB) $(ALL_DEP_LIBS) $(MALLOCLIB) -lm -o $(L_BIN_DIR)/$@
	@cp -f $(L_BIN_DIR)/$@ $(M_BIN_DIR)/$@
	@cd $(L_BIN_DIR); ../$(INSTALL) -m 0755 $(TARGET_SCRIPTS) ../$(M_BIN_DIR)
	@mkdir -p ../html
	@mkdir -p ../html/icons
	@mkdir -p ../html/html-includes
	@cp -f ./icons/*.gif ../html/icons
	@cp -f ./html-includes/*.txt ../html/html-includes


# Rule for build cleaning
clean_build:
	@cd $(M_INCL_DIR); rm -f $(HEADER_FILES)
	@rm -f $(OBJ_DIR)/*.o
	@rm -rf $(OBJ_DIR)/ii_files
	@rm -f $(L_MOD_LIB)
	@rm -f $(M_MOD_LIB)
	@rm -f $(M_AGR_LIB)
	@cd $(L_BIN_DIR); rm -f $(TARGETS)
	@cd $(M_BIN_DIR); rm -f $(TARGETS) $(TARGET_SCRIPTS)
	@rm -f ../html/icons/*.gif
	@rm -f ../html/html-includes/*.txt


# Rule for test results cleaning
clean_test:
	@rm -f $(TEST_RES_DIR)/*.log
	@rm -rf $(TEST_RES_DIR)/*.dic


$(M_MOD_LIB): $(L_MOD_LIB)
#       Install header files
	@cd $(L_INCL_DIR); \
            ../$(INSTALL) $(INSTALLOPTS) $(HEADER_FILES) ../$(M_INCL_DIR)

#       Install module library
	$(INSTALL) $(INSTALLOPTS) $(L_MOD_LIB) $(M_LIB_DIR)

#       Create agregate library
	@cd $(L_LIB_DIR); cp ../$(COMMON_LIB) $(TMP_LIB)
	@cd $(L_LIB_DIR); $(AR) $(AR_GETFLAGS) $(TMP_LIB)
	@cd $(L_LIB_DIR); rm -f $(TMP_LIB)

	@cd $(L_LIB_DIR); cp ../$(TABLES_LIB) $(TMP_LIB)
	@cd $(L_LIB_DIR); $(AR) $(AR_GETFLAGS) $(TMP_LIB)
	@cd $(L_LIB_DIR); rm -f $(TMP_LIB)

	@cd $(L_LIB_DIR); cp ../$(CIF_FILE_LIB) $(TMP_LIB)
	@cd $(L_LIB_DIR); $(AR) $(AR_GETFLAGS) $(TMP_LIB)
	@cd $(L_LIB_DIR); rm -f $(TMP_LIB)

	@cd $(L_LIB_DIR); cp $(MOD_LIB) $(TMP_LIB)
	@cd $(L_LIB_DIR); $(AR) $(AR_GETFLAGS) $(TMP_LIB)
	@cd $(L_LIB_DIR); rm -f $(TMP_LIB)

	@cd $(L_LIB_DIR); $(AR) $(AR_PUTFLAGS) $(AGR_LIB) $(ALL_OBJ_FILES)
	@cd $(L_LIB_DIR); rm -f $(ALL_OBJ_FILES)

	$(INSTALL) $(INSTALLOPTS) $(L_AGR_LIB) $(M_LIB_DIR)
	@rm -f $(L_AGR_LIB)


$(L_MOD_LIB): $(OBJ_OTHER_FILES)
#       Create module library
	@cd $(OBJ_DIR); $(AR) $(AR_PUTFLAGS) ../$@ $(OBJ_OTHER_FILES)
	$(RANLIB) $@
	@echo $@ " is up to date."


# Rule for making object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.C
	$(CCC) $(C++FLAGS) -DHTMLSSI -c $< -o $@


# Phony rule for making object files
%.o: $(SRC_DIR)/%.C
	$(CCC) $(C++FLAGS) -DHTMLSSI -c $< -o $(OBJ_DIR)/$@

