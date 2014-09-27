# ------------------------------------------------------------------------------
# (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
# RTI grants Licensee a license to use, modify, compile, and create derivative
# works of the Software.  Licensee has the right to distribute object form only
# for use with RTI products.  The Software is provided "as is", with no warranty
# of any type, including any warranty for fitness for any purpose. RTI is under
# no obligation to maintain or support the Software.  RTI shall not be liable for
# any incidental or consequential damages arising out of the use or inability to
# use the software.
# ------------------------------------------------------------------------------

#### GENERAL DEFINITIONS ####
$(warning Set DEBUG=1 to build the debug version!)

ifeq ($(NDDSHOME),)
  $(error Please define NDDSHOME)
endif
NDDSHOME:=$(shell readlink -f $(NDDSHOME))
$(warning NDDSHOME=$(NDDSHOME))
export NDDSHOME

ifeq ($(ROUTER_HOME),)
  $(error Please define ROUTER_HOME)
endif
ROUTER_HOME:=$(shell readlink -f $(ROUTER_HOME))
$(warning ROUTER_HOME=$(ROUTER_HOME))
export ROUTER_HOME

ifeq ($(DNP3_HOME),)
  $(error Please define DNP3_HOME)
endif
DNP3_HOME:=$(shell readlink -f $(DNP3_HOME))
$(warning DNP3_HOME=$(DNP3_HOME))
export DNP3_HOME

ifeq ($(PLATFORM),)
  $(warning Your host architecture appears to be $(shell uname -p))
  $(warning RTI Connext DDS platform not defined e.g.: {i86Linux3.xgcc4.6.3, \
  			i86Linux2.6gcc4.4.5, x64Linux2.6gcc4.4.5})
  $(error Please define the PLATFORM variable)
endif
export PLATFORM

DNP3_TOOLS_HOME	= $(DNP3_HOME)/../dnp3-tools
DNP3_TOOLS_HOME_EXISTS=$(shell test -d $(DNP3_TOOLS_HOME) && echo yes||echo no)
ifeq ($(DNP3_TOOLS_HOME_EXISTS),no)
  $(error $(DNP3_TOOLS_HOME) missing! Please build dnp3 repo first!)
endif

ifeq ($(MSGBUSHOME),)
	MSGBUSHOME = $(CURDIR)/../dnp3_message_bus
endif

ifeq ($(NDDSGENHOME),)
	NDDSGENHOME = $(NDDSHOME)
endif


# By default, build the debug version:
ifndef DEBUG
	DEBUG = 0
endif

ifeq ($(DEBUG),1)
    DNP3_LIBMODE = debug
    FILE_SUFFIX = d
    CFLAGS += -ggdb3 -O0
else
    DNP3_LIBMODE = release
    FILE_SUFFIX =
    CFLAGS += -O2
endif
 
ifneq ($(findstring i86,$(PLATFORM)),)
  CFLAGS += -m32
  C_LD_FLAGS += -m32
endif
ifneq ($(findstring x64,$(PLATFORM)),)
  CFLAGS += -m64
  C_LD_FLAGS += -m64
endif

NDDS_C_LIBS =	-L$(NDDSHOME)/lib/$(PLATFORM) \
		-lnddscz$(FILE_SUFFIX) \
		-lnddscorez$(FILE_SUFFIX)

NDDS_CPP_LIBS =	-L$(NDDSHOME)/lib/$(PLATFORM) \
		-lnddscppz$(FILE_SUFFIX) \
		-lnddscz$(FILE_SUFFIX) \
		-lnddscorez$(FILE_SUFFIX) \

NDDSGEN = $(NDDSGENHOME)/scripts/rtiddsgen


IDL_NAME_SimpleAPDU = SimpleAPDU
IDL_PATH = resource/idl

IDL_C_SOURCES = $(IDL_OUT_C_DIR)/$(IDL_NAME_SimpleAPDU:%=%.c) \
	        $(IDL_OUT_C_DIR)/$(IDL_NAME_SimpleAPDU:%=%Plugin.c) \
                $(IDL_OUT_C_DIR)/$(IDL_NAME_SimpleAPDU:%=%Support.c)

IDL_C_HEADERS = $(IDL_OUT_C_DIR)/$(IDL_NAME_SimpleAPDU:%=%.h) \
		$(IDL_OUT_C_DIR)/$(IDL_NAME_SimpleAPDU:%=%Plugin.h) \
		$(IDL_OUT_C_DIR)/$(IDL_NAME_SimpleAPDU:%=%Support.h)

IDL_C_OUT = $(IDL_C_SOURCES) $(IDL_C_HEADERS)

IDL_OUT_C_DIR   = src/dds_c
					 

ADAPTER_SRC_DIR = src/adapter

ADAPTER_SOURCES = \
			DNP3Adapter.c \
			DNP3Connection.c \
			DNP3StreamReader.c \
			DNP3StreamWriter.c

ADAPTER_SOURCES_CPP = \
			APDUConverter.cxx

ADAPTER_OBJECTS = $(addprefix $(BINOUTPUT_DIR)/, \
		    $(ADAPTER_SOURCES:%.c=%.adapter.o) )

ADAPTER_OBJECTS_CPP = $(addprefix $(BINOUTPUT_DIR)/, \
			$(ADAPTER_SOURCES_CPP:%.cxx=%.adapter.cpp.o) )


IDL_C_OBJECTS = $(addprefix $(BINOUTPUT_DIR)/, \
		$(IDL_C_SOURCES:$(IDL_OUT_C_DIR)/%.c=%.idl.o) )
		    
OBJECTS =   $(ADAPTER_OBJECTS) \
	    $(ADAPTER_OBJECTS_CPP) \
	    $(IDL_C_OBJECTS) \

TESTERS_C = $(addprefix $(BINOUTPUT_DIR)/, \
	    $(TEST_SOURCES_C:$(TEST_SRC_DIR)%.c=%) )

CFLAGS += -fPIC		

CC = g++
CLD = g++	
			
		
ifeq ($(CC),)
	CC = g++
endif

ifeq ($(CLD),)
	CLD = g++
endif

DEFINES = -DRTI_UNIX -DRTI_LINUX

INCLUDES_C = -Iinclude -Iinclude/adapter \
			-I$(IDL_OUT_C_DIR) -Isrc \
			-I$(NDDSHOME)/include \
			-I$(NDDSHOME)/include/ndds \
			-I$(ROUTER_HOME)/include \
			-I$(MSGBUSHOME)/include \
			-I$(DNP3_HOME) \
			-I$(DNP3_TOOLS_HOME)/boostlib/boost_1_47_0/include
			
MSGBUS_SO = $(MSGBUSHOME)/../lib/$(PLATFORM)/libdnp3messagebus$(FILE_SUFFIX).so


DNP3_LIBS = $(DNP3_HOME)/DNP3/Linux_i686/$(DNP3_LIBMODE)/libdnp3.a \
	    $(DNP3_HOME)/APL/Linux_i686/$(DNP3_LIBMODE)/libapl.a

ROUTER_LIBS		= -L$(ROUTER_HOME)/bin/$(PLATFORM) \
       		   	  -lrtirsinfrastructure			
       		   	  
RTI_C_LIBS	= $(NDDS_C_LIBS) $(ROUTER_LIBS)


MSGBUS_LIBS = -L$(MSGBUSHOME)/../lib/$(PLATFORM)

ifeq ($(DEBUG),1)
  MSGBUS_LIBS += -ldnp3messagebusd
else
  MSGBUS_LIBS += -ldnp3messagebus
endif

SYSTEM_LIBS = -ldl -lnsl -lm -lpthread -lrt

BINOUTPUT_DIR = objs/$(PLATFORM)
LIBOUTPUT_DIR = ../lib/$(PLATFORM)

BUILD_OUTPUT_DIRS = objs \
		    $(BINOUTPUT_DIR) \
		    $(IDL_OUT_C_DIR)

DNP3ADAPTER_LIB = $(LIBOUTPUT_DIR)/libdnp3adapter$(FILE_SUFFIX).so

.PHONY: all build \
	idlClean cleanIdl \
	objsClean cleanObjs \
	package packageClean packageList

first-target : build
	@echo build completed successfully

build : idlGenerate \
    $(BUILD_OUTPUT_DIRS) \
	$(IDL_C_OBJECTS) \
	$(DNP3ADAPTER_LIB) \
	$(MSGBUS_SO)
	

$(DNP3ADAPTER_LIB) : $(ADAPTER_OBJECTS) \
	    $(ADAPTER_OBJECTS_CPP) \
	    $(IDL_C_OBJECTS)
	gcc $(C_LD_FLAGS) -shared \
	  -o $(@) \
		$(^) \
		$(MSGBUS_LIBS) \
		$(DNP3_LIBS) \
		$(BOOST_LIBS) \
		$(RTI_C_LIBS) \
		$(SYSTEM_LIBS)

 
$(BINOUTPUT_DIR)/%.adapter.o : $(ADAPTER_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $(DEFINES) $(INCLUDES_C) -c $<
	
$(BINOUTPUT_DIR)/%.adapter.cpp.o : $(ADAPTER_SRC_DIR)/%.cxx
	$(CC) $(CFLAGS) -o $@ $(DEFINES) $(INCLUDES_C) -c $<

$(BINOUTPUT_DIR)/%.idl.o : $(IDL_OUT_C_DIR)/%.c
	gcc $(CFLAGS) -o $@ $(DEFINES) $(INCLUDES_C) -c $<
	
$(BINOUTPUT_DIR)/% : 
	$(CLD) $(C_LD_FLAGS) -o $(@) \
	$<  \
	$(NDDS_CPP_LIBS) \
        $(SYSTEM_LIBS)
        
$(BINOUTPUT_DIR) $(LIBOUTPUT_DIR):
	mkdir -p $@

objs :
	mkdir -p objs
	
%.dir :
	@test -d $* || echo "creating dir '$*'" && mkdir -p $*


####### cleanup targets

clean : idlClean
	rm -rf $(BUILD_OUTPUT_DIRS)

# offer cleanObjs and objsClean as synonyms
cleanObjs: objsClean

objsClean:
	rm -rf objs

# offer cleanIdl and idlClean as synonyms
cleanIdl: idlClean

idlClean:
	rm -rf $(IDL_OUT_C_DIR)

###### IDL Targets ######

idlGenerate: $(IDL_OUT_C_DIR) $(IDL_C_OUT) \
	$(IDL_PATH)/$(IDL_NAME_SimpleAPDU).xml
   
$(IDL_OUT_C_DIR):
	mkdir -p $(IDL_OUT_C_DIR)

$(IDL_C_OUT): $(IDL_PATH)/$(IDL_NAME_SimpleAPDU).idl  
	$(NDDSGEN) -convertToXML -replace -language C \
	    -d $(IDL_OUT_C_DIR) $(IDL_PATH)/$(IDL_NAME_SimpleAPDU).idl
	mv $(IDL_OUT_C_DIR)/$(IDL_NAME_SimpleAPDU).xml $(IDL_PATH)/
	#echo "<dds>" > $(IDL_PATH)/AUTO_QOS_PROFILES_SimpleAPDU.xml
	#sed 1d < $(IDL_PATH)/$(IDL_NAME_SimpleAPDU).xml \
	#    >> $(IDL_PATH)/AUTO_QOS_PROFILES_SimpleAPDU.xml
	#echo "</dds>" >> $(IDL_PATH)/AUTO_QOS_PROFILES_SimpleAPDU.xml

###### // IDL Targets ######

###############################################################################
# Test

test: build
	resource/dnp3/simple-test/run_simple-test.sh 


######## package targets (see "cleanup targets" for cleaning package output)
DOWNLOADS_DIR := ../build

PACKAGE_BASENAME = rticonnextdds-dnp3
PACKAGE_DIR = $(DOWNLOADS_DIR)/$(PACKAGE_BASENAME)
PACKAGE_VERSION = $(shell git describe)
PACKAGE_BUNDLE = $(PACKAGE_BASENAME)_$(PACKAGE_VERSION).tgz
PACKAGE_OUTPUT_DIRS = $(PACKAGE_DIR)

# the list of files we should package in the rs_adapter folder will be in
# a set defined by this variable, but the find command will yield some files we
# might not want to put in the final package. We'll filter those out later.
RS_ADAPTER_DIST_FILES_RAW := $(shell find -L -type f)

# change the "POV" of where the files are, to make it easier operating on them
RS_ADAPTER_DIST_FILES := $(patsubst ./%, %, $(RS_ADAPTER_DIST_FILES_RAW))

# define a list of files we don't want packaged
RS_ADAPTER_NON_DIST_FILES := \
				%/.nfs% \
				%.DS_Store \
				%.swp \
				%.swo \
				.project \
				.cproject \
				.settings% \
				%.gitignore \
				%.gdbinit \
				%.log \
				makefile \
				include% \
				src% \
				objs/%.o

# now remove files that aren't supposed to be packaged
RS_ADAPTER_DIST_FILES := $(filter-out \
				$(RS_ADAPTER_NON_DIST_FILES), \
				$(RS_ADAPTER_DIST_FILES) )

# define list of target files to package
RS_ADAPTER_PACKAGE = $(addprefix $(PACKAGE_DIR)/rs_adapter/, \
		    $(RS_ADAPTER_DIST_FILES) )

# undo the "POV" correction we did back when we defined the dist files
RS_ADAPTER_PACKAGE_DEPS = $(patsubst %, ./%, $(RS_ADAPTER_DIST_FILES))


package : $(RS_ADAPTER_PACKAGE) \
 $(PACKAGE_DIR)/../$(PACKAGE_BUNDLE)
	
packageList :
	find -L $(PACKAGE_DIR) -type f
	

packageClean:
	rm -rf $(PACKAGE_DIR)/../$(PACKAGE_BASENAME)_*.tgz $(PACKAGE_DIR) 

$(PACKAGE_DIR)/rs_adapter/% : \
	    $(PACKAGE_DIR)/rs_adapter.dir \
	    $(PACKAGE_DIR)/rs_adapter/resource.dir \
	    $(PACKAGE_DIR)/rs_adapter/resource/idl.dir \
	    $(PACKAGE_DIR)/rs_adapter/resource/dnp3.dir \
	    $(PACKAGE_DIR)/rs_adapter/resource/dnp3/simple-test.dir     
	cp ./$* $@

$(PACKAGE_DIR)/../$(PACKAGE_BUNDLE): 
	echo $(PACKAGE_VERSION) > $(PACKAGE_DIR)/version.txt
	-mkdir -p $(PACKAGE_DIR)/lib/$(PLATFORM)
	cp -p ../lib/$(PLATFORM)/*.so $(PACKAGE_DIR)/lib/$(PLATFORM)
	-mkdir -p $(PACKAGE_DIR)/dnp3/TestSet/Linux_i686/debug/
	-mkdir -p $(PACKAGE_DIR)/dnp3/TestSet/Linux_i686/release/
	-cp $(DNP3_HOME)/TestSet/Linux_i686/debug/testset.exe $(PACKAGE_DIR)/dnp3/TestSet/Linux_i686/debug/
	-cp $(DNP3_HOME)/TestSet/Linux_i686/release/testset.exe $(PACKAGE_DIR)/dnp3/TestSet/Linux_i686/release/
	tar -czvf $@ -C $(PACKAGE_DIR)/.. $(PACKAGE_BASENAME)
#------------------------------------------------------------------------------


