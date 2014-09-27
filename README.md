<!--
 (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 -->

rticonnextdds-dnp3
==================
DNP3 Adapter for RTI Connext DDS Routing Service

To build the DNP3 Adapter for RTI Connext DDS Routing Service

1. Build required dependencies: [dnp3++][] library
2. Build the `dnp3_message_bus` library
3. Build the `rs_adapter` library

You can build either the debug version `(DEBUG=1)` or the release version 
`(DEBUG=0)`. To build both versions, build with `DEBUG=1` and then with 
`DEBUG=0` (default).

Once built, you can run the the included
[simple test example][rs_adapter test] to verify the 
DNP3 Adapter for RTI Connext DDS Routing Service. You can also use this 
example as basis for creating your own routing service configuration.

You can optionally [package][] the
DNP3 Adapter for RTI Connext DDS Routing Service binaries for distribution.

Each of the steps are detailed below.

System requirements
-------------------
The DNP3 Adapter for RTI Connext DDS Routing Service and the steps below to 
build and run it have been validated on the following platforms:

- Ubuntu 12.04 LTS 32-bit and 64-bit

Please let us know (send us a pull request or email) if you validate on a new 
platform, so that so that we can expand this list and keep it updated for 
future users.


Dependencies
------------
### Required: RTI Connext DDS Core Libraries and Routing Service

RTI Connext DDS Software can be downloaded from the RTI Community portal 
downloads page:
- [RTI Community > Downloads](http://community.rti.com/content/page/downloads)



A Live ISO with all the RTI Software pre-installed can be downloaded from the
from the RTI Community portal downloads page:
- [RTI Community > Downloads > Live ISO](http://community.rti.com/downloads/live-cds/download-live-cd)


### Required: dnp3++ (patched dnp3)
[dnp3++]: #dnp3
<a id="dnp3"></a> 

Obtain the [patched dnp3](https://github.com/rajive/dnp3) source

		mkdir rticonnextdds-dnp3-workspace
		cd rticonnextdds-dnp3-workspace/
		
		git clone git@github.com:rajive/dnp3.git
		# git checkout rticonnextdds-dnp3  # OPTIONAL: should already be here
		# NOTE: tested against the 'dnp3++' tag 

Install rake to build dnp3
    
		(ubuntu)  sudo apt-get install -y rake libtool
		(rhel)    sudo yum install  rubygem-rake
    
    
Build the patched dnp3 source

		./dnp3/build_me.sh   
	
For the rest of this document, set
		DNP3_HOME=/path/to/rticonnextdds-dnp3-workspace/dnp3
	
	
You should end up with the following layout in rticonnextdds-dnp3-workspace:

		$ ls ${DNP3_HOME}/..
		dnp3/ dnp3-tools/
	 

### Optional

[tmux](http://tmux.sourceforge.net/) - terminal multiplexer 


dnp3_message_bus/
-----------------

### make

make parameters
- `DEBUG` = 1 for debug version; 0 otherwise (default)
- `PLATFORM` = The RTI Connext DDS platform to build for
- `DNP3_HOME` = path to the [dnp3 library][dnp3++]

make targets
- `${PLATFORM}` : (default) build the adapter shared library for ${PLATFORM}
- `test` : run the tester (Use ^C to terminate it)
- `clean` : delete the output directory
		
make output
- `../lib/${PLATFORM}/{libdnp3messagebusd.so,MessageBusTester}`


### Build

Example:

		$ DEBUG=1 \
		PLATFORM=i86Linux3.xgcc4.6.3 \
		DNP3_HOME=../../dnp3 \
		make
	
		$ ls ../lib/i86Linux3.xgcc4.6.3/
		libdnp3messagebusd.so  MessageBusTester
	
	
### Test

Example:

		$ DEBUG=1 \
		DNP3_HOME=../../dnp3 \
		PLATFORM=i86Linux3.xgcc4.6.3 \
		make test
	
		makefile:11: Set DEBUG=1 to build the debug version!
		Checking directory objs
		Checking directory objs/i86Linux3.xgcc4.6.3
		Checking directory ../lib
		Checking directory ../lib/i86Linux3.xgcc4.6.3
		LD_LIBRARY_PATH=../lib/i86Linux3.xgcc4.6.3 \
			../lib/i86Linux3.xgcc4.6.3/MessageBusTester 
		2014-Sep-23 01:09:47.542 - INFO - MsgBus - New listener registered: 0xbf83ed38
		2014-Sep-23 01:09:47.550 - INFO - MsgBus.SlaveProxy - DNP3-AsyncStackManager created.
		2014-Sep-23 01:09:47.551 - INFO - MsgBus.SlaveProxy - Creating physical ports.
		2014-Sep-23 01:09:47.552 - INFO - MsgBus.SlaveProxy - Created TCP-server port: TCPServer (listening on: 127.0.0.1:4999)
		2014-Sep-23 01:09:47.552 - INFO - MsgBus.SlaveProxy - Physical ports created.
		2014-Sep-23 01:09:47.552 - INFO - MsgBus - New DNP3 proxy created : SlaveProxy (id=0)
		2014-Sep-23 01:09:47.561 - DEBUG - MsgBus.SlaveProxy.DNP3.TCPServer - Linking stack to port w/ route  Local: 50 Remote: 80
		2014-Sep-23 01:09:47.562 - DEBUG - MsgBus.SlaveProxy.DNP3.TCPServer - Start()
		2014-Sep-23 01:09:47.563 - DEBUG - MsgBus.SlaveProxy.DNP3.TCPServer - MonitorStateInit(PLS_CLOSED) -> MonitorStateOpening(PLS_OPENING) : Open: 0 Opening: 0 Reading: 0 Writing: 0 Closing: 0
		2014-Sep-23 01:09:47.563 - INFO - MsgBus.SlaveProxy.Connection-0 - APDUProxy created.
		2014-Sep-23 01:09:47.563 - INFO - MsgBus.SlaveProxy - New connection created: Connection-0
		2014-Sep-23 01:09:47.564 - INTERPRET - MsgBus.SlaveProxy.DNP3.Connection-0-APDUProxy - Adding new listener MessageBus
		2014-Sep-23 01:09:47.564 - INFO - MsgBus - New connection created on proxy (id=0)
		2014-Sep-23 01:09:47.565 - INFO - MsgBus.MasterProxy - DNP3-AsyncStackManager created.
		2014-Sep-23 01:09:47.565 - INFO - MsgBus.MasterProxy - Creating physical ports.
		2014-Sep-23 01:09:47.566 - INFO - MsgBus.MasterProxy - Created TCP-client port: TCPClient (server: 127.0.0.1:4998)
		2014-Sep-23 01:09:47.566 - INFO - MsgBus.MasterProxy - Physical ports created.
		2014-Sep-23 01:09:47.566 - INFO - MsgBus - New DNP3 proxy created : MasterProxy (id=1)
		2014-Sep-23 01:09:47.567 - INTERPRET - MsgBus.SlaveProxy.DNP3.Connection-0-APDUProxy - NotificationThread - begin iteration
	
		^C to terminate
	

rs_adapter/
-----------

### make
[rs_adapter make]: #rs_adapter_make
<a id="rs_adapter_make"></a> 

make parameters

- `DEBUG` = 1 for debug version; 0 otherwise (default)
- `PLATFORM` = The RTI Connext DDS platform to build for
- `DNP3_HOME` = path to the [dnp3 library][dnp3++]
- `NDDSHOME` = path to the RTI Connext DDS libraries
- `ROUTER_HOME` = path to the RTI Connext DDS Routing Service

make targets

- `build` : (default) build the adapter shared library for ${PLATFORM}
- `test` : run a simple test (see below)
- `clean` : delete the output directory
- `package` : create a dir containing a binary package for distribution
- `packageList` : list the contents of the package dir
- `packageClean` : clean the package dir

		
make output
- `../lib/${PLATFORM}/libdnp3adapterd.so`


### Build

Example

		$ DEBUG=1 \
		PLATFORM=i86Linux3.xgcc4.6.3 \
		DNP3_HOME=../../dnp3 \
		NDDSHOME=~/RTI/ndds.5.1.0 \
		ROUTER_HOME=~/RTI/RTI_Routing_Service_5.1.0 \
		make
	
		$ ls ../lib/i86Linux3.xgcc4.6.3/
		libdnp3adapterd.so  libdnp3messagebusd.so  MessageBusTester


### Test
[rs_adapter test]: #rs_adapter_test
<a id="rs_adapter_test"></a> 

A simple loop-back test is included for the RTI Connext DDS DNP3 adapter.

Running the test requires specifying one additional parameter, in addition to 
the ones for [building the adapter][rs_adapter make]:

- `DDS_DOMAIN` = the DDS domain to use for running the simple test (default = 42)
- `LAUNCH_WITH_TMUX` = 1 if tmux should be used to for the program windows; 
0 if X windows (terminals) should be used (default) 
	
The simplest way to invoke the test is via the make test target, as follows.

0. OPTIONAL: In a terminal window, size it, and then start `tmux` server

        tmux
      
1. Start a loopback test

		$ LAUNCH_WITH_TMUX=1 \
		DDS_DOMAIN=42 \
		DEBUG=1 \
		PLATFORM=i86Linux3.xgcc4.6.3 \
		DNP3_HOME=../../dnp3 \
		NDDSHOME=~/RTI/ndds.5.1.0 \
		ROUTER_HOME=~/RTI/RTI_Routing_Service_5.1.0 \
		make test

2. Exercise the test
	In the pane titled `Master102` type in the following command:
	
		>issue bo 1 lon  
		Waiting for response... Result: CS_SUCCESS 
		
	If you see the response above (CS_SUCCESS), that means round-trip DNP3 -> DDS ->      DNP3 communication is working!

4. Terminate the test
    - if LAUNCH_WITH_TMUX=1, kill the tmux window (kill-window)
    - else, kill the XWindows terminal


### Package
[package]: #package
<a id="package"></a> 

Create package for distribution:

		$ DEBUG=1 \
		PLATFORM=i86Linux3.xgcc4.6.3 \
		DNP3_HOME=../../dnp3 \
		NDDSHOME=~/RTI/ndds.5.1.0 \
		ROUTER_HOME=~/RTI/RTI_Routing_Service_5.1.0 \
		make package
		
		$ ls ../build
		eclipse/ rticonnextdds-dnp3/  rticonnextdds-dnp3_verYYYYMMDD-HHMM.tgz


Run the packaged example:

        $ cd ../build/rticonnextdds-dnp3/rs_adapter/
        
		$ LAUNCH_WITH_TMUX=1 \
		DDS_DOMAIN=42 \
		DEBUG=1 \
		PLATFORM=i86Linux3.xgcc4.6.3 \
		DNP3_HOME=~/Code/rticonnextdds-dnp3-workspace/dnp3 \
		NDDSHOME=~/RTI/ndds.5.1.0 \
		ROUTER_HOME=~/RTI/RTI_Routing_Service_5.1.0 \
		./resource/dnp3/simple-test/run_simple-test.sh 


Users Manual
============

For more details on how to exercise the simple test and configure the
DNP3 Adapter for RTI Connext DDS Routing Service, please refer to 

- [rs_adapter/USERS_MANUAL.md](rs_adapter/USERS_MANUAL.md)
