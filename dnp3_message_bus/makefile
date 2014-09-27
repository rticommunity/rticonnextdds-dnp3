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
 
ifeq ($(DNP3_HOME),)
  $(error Please define DNP3_HOME)
endif
ifeq ($(PLATFORM),)
  $(warning Your host architecture appears to be $(shell uname -p))
  $(warning RTI Connext DDS platform not defined e.g.: {i86Linux3.xgcc4.6.3, \
  			i86Linux2.6gcc4.4.5, x64Linux2.6gcc4.4.5})
  $(error Please define the PLATFORM variable)
endif
export PLATFORM

DNP3_TOOLS_HOME	= $(DNP3_HOME)/../dnp3-tools
DNP3_TOOLS_HOME_EXISTS= $(shell test -d $(DNP3_TOOLS_HOME) && echo yes || echo no)
ifeq ($(DNP3_TOOLS_HOME_EXISTS),no)
  $(error $(DNP3_TOOLS_HOME) missing! Please build dnp3 repo first!)
endif

CC				= g++
CLD				= g++

ifeq ($(DEBUG),1)
CFLAGS 			+= -g -O0
DNP3_RELEASE	= debug
else
CFLAGS 			+= -O2
DNP3_RELEASE	= release
endif

ifneq ($(findstring i86,$(PLATFORM)),)
  CFLAGS			+= -DRTI_UNIX -DRTI_LINUX -m32
  CLDFLAGS 		+= -m32
endif
ifneq ($(findstring x64,$(PLATFORM)),)
  CFLAGS			+= -DRTI_UNIX -DRTI_LINUX -DRTI_64BIT -m64
  CLDFLAGS 		+= -m64 -L /lib64 -l pthread
endif

#CLDFLAGS += -static-libstdc++ -static-libgcc
CFLAGS			+= -fPIC

SYSLIBS 		= -ldl -lnsl -lm -lpthread -lrt 

#### DIRECTORY DEFINITIONS ####
BOOST_DIR		= $(DNP3_TOOLS_HOME)/boostlib/boost_1_47_0/Linux_i686
SOURCES_DIR		= src
BUILD_DIR_NAME	= objs
BUILD_DIRS 		= $(BUILD_DIR_NAME).dir $(BUILD_DIR_NAME)/$(PLATFORM).dir
OUTPUT_DIR_NAME	= ../lib
OUTPUT_DIRS		= $(OUTPUT_DIR_NAME).dir $(OUTPUT_DIR_NAME)/$(PLATFORM).dir

#### DEPENDENCIES DEFINITIONS ####
INCLUDES 		+= -Iinclude -Iinclude/dnp3msgbus \
				   -I$(DNP3_HOME) \
				   -I$(DNP3_TOOLS_HOME)/boostlib/boost_1_47_0/include


DNP3_LIBS 		= $(DNP3_HOME)/APL/Linux_i686/$(DNP3_RELEASE)/libapl.a \
				 $(DNP3_HOME)/DNP3/Linux_i686/$(DNP3_RELEASE)/libdnp3.a
				  
#BOOST_LIBS		= -L$(BOOST_DIR) -lboost_thread -lboost_system
BOOST_LIBS		= $(BOOST_DIR)/libboost_thread.a \
             	  $(BOOST_DIR)/libboost_system.a

#### SOURCE FILES DEFINITIONS ####
SOURCES_CPP		+= MessageBus.cpp \
		   		   Connection.cpp \
		   		   Proxy.cpp \
		   		   MessageBusC.cpp \
		   		   APDU.cpp \
		   		   Utils.cpp \
		   		   lcfg_static.cpp

SOURCES_C		+= MessageBusTester.c \
				   	   		   
		   		   		   		   
#SOURCES_C		+= MessageBus.c \
#		   		   APDU.c \
#		   		   Utils.c
		   		   
		   		   
#MAIN_SRC		= DNP3Tester.cpp
MAIN_SRC		= MessageBusTester.c

#### COMPILATION OUTPUT DEFINITIONS ####
SOURCES_CPP_OUT	= $(SOURCES_CPP:%.cpp=$(BUILD_DIR_NAME)/$(PLATFORM)/%.cpp.o)
SOURCES_C_OUT	= $(SOURCES_C:%.c=$(BUILD_DIR_NAME)/$(PLATFORM)/%.c.o)
MAIN_OUT		= $(MAIN_SRC:%.c=$(BUILD_DIR_NAME)/$(PLATFORM)/%.c.o)
SOURCES_OUT		= $(SOURCES_CPP_OUT) $(SOURCES_C_OUT) $(MAIN_OUT)  

ifeq ($(DEBUG),1)
SO_OUT			= $(OUTPUT_DIR_NAME)/$(PLATFORM)/libdnp3messagebusd.so
else
SO_OUT			= $(OUTPUT_DIR_NAME)/$(PLATFORM)/libdnp3messagebus.so
endif

MAIN_EXEC		= $(MAIN_SRC:%.c=$(OUTPUT_DIR_NAME)/$(PLATFORM)/%)

#### MAIN EXECUTABLE LIBRARIES ####
LIBS			+= -L$(OUTPUT_DIR_NAME)/$(PLATFORM)

ifeq ($(DEBUG),1)
LIBS += -ldnp3messagebusd
else
LIBS += -ldnp3messagebus
endif

LIBS += $(DNP3_LIBS) $(BOOST_LIBS)


#### TARGET DEFINITIONS ####
$(PLATFORM) : $(BUILD_DIRS) $(OUTPUT_DIRS) $(SO_OUT) $(MAIN_EXEC)

$(SO_OUT) : $(SOURCES_OUT)
	@echo DEBUG="$(DEBUG)"
	@echo PLATFORM="$(PLATFORM)"
	$(CLD) $(CLDFLAGS) -shared -o $@ $^ $(DNP3_LIBS) $(BOOST_LIBS) $(SYSLIBS) 

$(BUILD_DIR_NAME)/$(PLATFORM)/%.c.o : $(SOURCES_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $(INCLUDES) -c $<
	
$(BUILD_DIR_NAME)/$(PLATFORM)/%.cpp.o : $(SOURCES_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ $(INCLUDES) -c $<
	
$(MAIN_EXEC):
	$(CLD) $(CLDFLAGS) $< -o $@ $(LIBS) 
	
clean : $(BUILD_DIRS:%=%.clean) $(OUTPUT_DIRS:%=%.clean)

test: $(PLATFORM)
	LD_LIBRARY_PATH=$(OUTPUT_DIR_NAME)/$(PLATFORM) \
	$(MAIN_EXEC) 
	
%.dir : 
	@echo "Checking directory $*"
	@if [ ! -d $* ]; then \
		echo "Making directory $*"; \
		mkdir -p $* ; \
	fi;

%.dir.clean :
	rm -rf $*

