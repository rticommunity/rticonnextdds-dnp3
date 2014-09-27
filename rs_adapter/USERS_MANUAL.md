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

DNP3 Adapter for RTI Connext DDS Routing Service - User's Manual
================================================================

1. [Pre-requisites][]
2. [Loop-back Scenario][]
3. [DNP3 Simulator][]
   1. [Interacting with the Slave][]
   2. [Interacting with the Master][]
4. [Configuration of the DNP3MessageBus][]
5. [Configuration of Routing Service Adapter][]
6. [DNP3MessageBus Configuration Syntax][]
7. [Example Master Proxy Configuration][]
8. [Example Slave Proxy Configuration][]


Pre-requisites
--------------
[Pre-requisites]: #Pre-requisites
<a id="Pre-requisites"></a> 

This guide assumes familiarity with:

- [RTI Connext DDS Routing Service](http://community.rti.com/documentation)
- [RTI Connext DDS Core Concepts](http://community.rti.com/documentation)


Loop-back Scenario
------------------
[Loop-back Scenario]: #Loop-back_Scenario
<a id="Loop-back_Scenario"></a> 

The included

		resource/dnp3/simple-test/run_simple-test.sh

exercises a simple DNP3 to DDS loop-back scenario. 

 This scenario demonstrates a basic proxy configuration of
 DNP3 communication over DDS. 
 
 A DNP3 Master (address 102) communicates with a DNP3 Slave
 (address 11) and their messages are proxied by a Routing
 Service instance (rs1), which converts them to/from a DDS
 topic named DNP3Bus on domain $DNP3_DOMAIN.
 
 In order to run the whole scenario locally, messages coming
 from DNP3 are actually published on a DDS topic named 
 DNP3BusReplica on domain $DNP3_DOMAIN.
 
 A second Routing Service instance (rs2) duplicates all
 messages from that topic to DNP3Bus, domain $DNP3_DOMAIN.
 
The following is a diagram of the communication path:

		-------------------------------------------------------------
		|      DNP3           |          RS1         |     RS2      |
		-------------------------------------------------------------
		|                     |                      |              |
		|     Slave 11        |      Master 102<<------->-------    |
		| (127.0.0.1:4900)<<------------->|          |         |    |
		|                     |                      |         |    |
		|                     |                      |         |    |
		|                     |                      |         |    |
		|    Master 102              Slave 11 <<----------->----    |
		|        |<------------->>(127.0.0.1:4902)   |              |
		|                     |                      |              |
		|                     |                      |              |
		-------------------------------------------------------------


Commands originate on a DNP3 MASTER *device 102*, get converted to a DDS topic 
called *DNP3BusReplica*, get looped back via another routing service on a DDS 
topic called *DNP3Bus*, and get converted back to commands for a 
DNP3 SLAVE *device 11*. 

		DNP3 Slave 11   <-- DDS Topic DNP3Bus
									^
						rs1         | rs2
									|
		DNP3 Master 102 --> DDS Topic DNP3Bus Replica
		

Similarly, responses originate on a DNP3 SLAVE *device 11*, 
get converted to a DDS topic called *DNP3BusReplica*, get 
looped back via another routing service on a DDS topic called *DNP3Bus*, and
get converted back to responses for a DNP3 MASTER *device102*.

		DNP3 Master 102 <-- DDS Topic DNP3Bus
									^
						rs1         | rs2
									|
		DNP3 Slave 11   --> DDS Topic DNP3Bus Replica
		
		
 The start script will create 8 panes (tmux) or tabs (XWindows):
 
 1. Routing Service "rs1"
 2. Routing Service "rs2"
 3. DNP3 Master "Master102"
 4. Master102's log
 5. DNP3 Slave "Slave11"
 6. Slave11's log
 7. RTI DDS Spy, Domain: $(DNP3_DOMAIN), Topic: DNP3Bus
 8. Command Shell 
   
You can look at "rs1"'s tab to observe how messages are
received by the adapter's StreamReaders and StreamWriters and
routed between the two domains.
 
The DNP3 entities' logs will show that end-to-end communication
at the DNP3 application layer occur transparently with respect
to the intermediate proxying.
 
RTI DDS Spy will show you all APDUs published on the 
DNP3Bus topic letting you observe the request/response 
interaction between master and slave.

The DNP3 entities are simulated using the TestSet application
provided by OpenDNP3. The configuration of the two instances 
used to simulate Slave11 and Master102 are

- dnp3_master_102_proxied.xml

         Creates Master102 and connects to Slave11 using a TCP
         client socket to 127.0.0.1:4902
 
- dnp3_slave_11.xml

         Creates Slave11 and listens for incoming connections
         on 127.0.0.1:4900
 
The DNP3 Routing Service adapter is configured by file
simple-test-DNP3.conf.

This configuration file specifies proxies for DNP3 entities
that will be created by the DNP3MessageBus and connections
to remote DNP3 entities, which will specify communication
channels that the bus will use to send APDUs.



DNP3 Simulator
--------------
[DNP3 Simulator]: #DNP3_Simulator
<a id="DNP3_Simulator"></a> 

When running the "run-simple-test.sh" demo, we use a simulated dnp3 master and 
a simulated dnp3 slave. The panes in which these simulators are launched 
are labeled (tmux or XWindows).

- Master102
- Slave11

These programs both have a text interface which we can use to interact with them.

Both simulators use a prompt which is a greater-than symbol, like this: ">"

Some commands we can use at the simulators are as follows.


### Interacting with the Slave
<a id="Interacting_with_the_Slave"></a> 
[Interacting with the Slave]: #Interacting_with_the_Slave

In the slave, changes to the inputs are first "queued", then "flushed". This 
allows for changing the state of several inputs in a single fragment.

In the following example, we will do the following:

- first, we will display the current status of the datapoints in the slave

- next, we will queue binary input index 5 to have the following:

            value: 1 (i.e. ON)
            quality: "on-line", and "locally-overridden"

- next, we will queue the binary input index 10 to have the following:

           value 1 (i.e. ON)
           quality: "on-line"

- next, we queue the counter input index 1 to have the following:

           value 1
           quality: "on-line"

- next, we will flush the queued changes

* finally, we will display the current status of the datapoints to see 
changes


Do this at the prompt:

		> show
		> queue bi 5 1 OL
		> queue bi 10 1 O
		> queue c 1 1 O
		> flush
		> show



### Interacting with the Master
[Interacting with the Master]: #Interacting_with_the_Master
<a id="Interacting_with_the_Master"></a> 

In the master, there is no "queue" command. Instead, there is an "issue" 
command which is the important one.

In the following example, we will do the following:

- first, we will send a command to the slave to operate a valid binary 
output:

           index: 1  (i.e. within the valid range)
           operation: "latch-on"

- next, we will send a command to the slave to operate an invalid binary 
output:

           index 2   (i.e. outside of the valid range)
           operation: "latch-off"

- finally, we will display the current status of the slave to see the changes


Do this at the prompt:

		> issue bo 1 lon
		  ( you should see a "success" message )
		> issue bo 2 loff
		  ( you should see an error message)




Configuration of the DNP3MessageBus
-----------------------------------
[Configuration of the DNP3MessageBus]: #Configuration_of_the_DNP3MessageBus
<a id="Configuration_of_the_DNP3MessageBus"></a> 

The **DNP3MessageBus** is a helper layer on top of the 
patched OpenDNP3 library, responsible for managing DNP3 
endpoints. The routing service adapter uses a singleton 
instance of the DNP3MessageBus.
 
 - The DNP3MessageBus is configured by means of external
   configuration files which specify proxies for DNP3 entities
   connections to actual remote DNP3 entities.
   The DNP3MessageBus will create proxies for each of the
   specified entities and open connections to each
   remote entity using the specified configuration.
   
 - Proxies are assigned a name, used to reference them in the
   rest of configuration file entries.
   
 - Connections are identified by the name of the proxy and
   the numeric address of the remote DNP3 entity
   
 - The order in which configuration entries are presented in
   the file is irrelevant and they can be specified in any
   order.
   
 - You will have to create a proxy for each DNP3 entity you
   want to "send messages as" and a connection to every DNP3
   entity you want to "send messages to".
   
 - The DNP3MessageBus instance is a singleton within the DNP3
   routing service adapter. You can specify the same configuration file for
   multiple domain routes; the bus will ignore and skip
   configurations for existing proxies or connections.
   
 - Refer to section [DNP3MessageBus Configuration Syntax][]
for additional documentation.
   

Configuration of Routing Service Adapter
----------------------------------------
[Configuration of Routing Service Adapter]: #Configuration_of_Routing_Service_Adapter
<a id="Configuration_of_the_DNP3MessageBus"></a> 

 - The DNP3 Adapter must be loaded using the following snippet
   in your RTI Connext DDS routing service configuration:
 
   E.g.
 
		<adapter_library name="adapters">
	       <adapter_plugin name="DNP3Adapter">
	           <dll>dnp3adapter</dll>
	           <create_function>
	               DNP3Adapter_create
	           </create_function>
	       </adapter_plugin>
		</adapter_library>
   
   If you want to use the debug version of the adapter, change
   the `<dll>` entry to `dnp3adapterd`
   
 - The DNP3 adapter uses DDS data-types that are defined in 
   the file 
   
		resource/idl/SimpleAPDU.idl
		
   You can also find an XML version of this data-type in the same location.
   You will need to include the XML type definition in the Routing Service
   configuration. This will allow you to register the type
   used to represent DNP3 APDUs with each DDS and DNP3
   connection you will configure in your domain routes.
   
   This type is registered by including the following in any
   `<connection_1>, <connection_2>, <participant_1>, 
   <participant_2>` element:
   
       <registered_type name="APDU"
             type_name="com::rti::rs::adapter::dnp3::SimpleAPDU" />
             
   This registers type `com::rti::rs::adapter::dnp3::SimpleAPDU`
   with name "APDU" in the enclosing connection, allowing to 
   reference it as "APDU" in any of the contained routes.
   
   SimpleAPDU is a simplified data type, which uses a byte 
   buffer to store a raw APDU and adds source, type and 
   destination info, which can be used to route APDUs
   appropriately at the DDS level.
   
 - A connection to a DDS domain is easily configured by 
   specifying the ID of the domain used for the connection.
   
   E.g.

		<participant_1>
	       <domain_id>46</domain_id>
	       <registered_type name="APDU"
	          type_name="com::rti::rs::adapter::dnp3::SimpleAPDU"/>
		</participant_1> 
	      
 - A DNP3 connection requires an external configuration file 
   used to configure the **DNP3MessageBus**.
   
   E.g.

		<connection_2 plugin_name="adapters::DNP3Adapter">
	       <registered_type name="APDU"
	          type_name="com::rti::rs::adapter::dnp3::SimpleAPDU"/>
	       <property>
	           <value>
	               <element>
	                   <name>dnp3config</name>
	                   <value>path/to/myDNP3.conf</value>
	               </element>
	           </value>
	       </property>
		</connection_2>
   
   The same file can be used by multiple connections; the 
   DNP3MessageBus is a singleton; it will
   skip the creation of any existing proxy or connection.
   
   The main advantage of this is that a configuration file may
   specify additional physical ports or connections for an
   existing proxy.
   
   E.g. (DNP3MsgBus configuration)

           proxy-0 = "MyExistingProxy"
           
           MyExistingProxy-phys-port-0 = "AdditionalTCPServer"
           
           MyExistingProxy-connection-0 = "103"
           
           AdditionalTCPServer-address = "127.0.0.1"
           AdditionalTCPServer-port = "4903"
           ....
           MyExistingProxy-103-phys-port = "AdditionalTCPServer"
           ...

 - Output routes to DNP3 do not require additional 
   configuration parameters.
   
   E.g.
    
           <output>
               <registered_type_name>APDU</registered_type_name>
               <stream_name>Master102-to-dnp3</stream_name>
               <creation_mode>IMMEDIATE</creation_mode>
           </output>
   
 - Input routes from DNP3 can be configured using a simple 
   filter that selects which of the APDUs received by the
   DNP3MessageBus will be propagated to the route's reader.
   The filtering support is currently very limited and it is
   possible to select messages based on the following 
   parameters:
   
   - src  : numeric address of an APDU's sender 
   
   - dest : numeric address of an APDU's recipient 
   
   - type : type of APDU 
        (one of: request, response, unsol-response)
            
   The filter syntax is :
   
         FILTER := FILTER_ENTRY [';' FILTER]
         FILTER_ENTRY := SRC | DEST | TYPE 
         SRC := 'src' '=' (NUM | ANY)
         DEST := 'dest' '=' (NUM | ANY)
         TYPE := 'type' '=' (TYPE_VAL | ANY)
         TYPE_VAL := 'request' | 'response' | 'unsol-response'
         ANY := '-1'
         NUM := ('0'|'1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9') [NUM]
     
   White spaces are ignored, but parsing is case-sensitive.
   
   E.g.
   
		<input connection="2">
	       <stream_name>
	           Slave11-to-Master102-Responses
	       </stream_name>
	       <registered_type_name>APDU</registered_type_name>
	       <property>
	           <value>
	               <element>
	                   <name>dnp3filter</name>
	                   <value>
	                   src = 11; dest = 102; type = response
	                   </value>
	               </element>
	           </value>
	       </property>
	       <creation_mode>IMMEDIATE</creation_mode>
		</input>
   
 - Input routes from DDS will probably make use of a content
   filtered topic to select only messages containing APDUs 
   that are pertinent to that route.
   
   E.g.
   
		<dds_input participant="1">
	       <topic_name>DNP3Bus</topic_name>
	       <content_filter>
	           <expression>
	               src.address = 11 AND dest.address = 102 AND
	               type = APDU_TYPE_RESPONSE
	           </expression>
	       </content_filter>
	       <registered_type_name>APDU</registered_type_name>
	       <creation_mode>IMMEDIATE</creation_mode>
		</dds_input>
   
 - Output routes to DDS need only to specify the DDS topic
   where APDUs will be published.
   
   E.g.
   
		<dds_output>
	       <topic_name>DNP3Bus</topic_name>
	       <registered_type_name>APDU</registered_type_name>
	       <creation_mode>IMMEDIATE</creation_mode>
		</dds_output>


   
DNP3MessageBus Configuration Syntax
-----------------------------------
[DNP3MessageBus Configuration Syntax]: #DNP3MessageBus_Configuration_Syntax
<a id="DNP3MessageBus_Configuration_Syntax"></a> 

	<NAME>      : name of a DNP3 Proxy, an ASCII string
	<PORT>      : name of a DNP3 Proxy's physical port, an ASCII string
	<ID>        : an unsigned integer, starting from 0 and increasing with each new entry
	<DNP3ADDR>  : address of the remote DNP3 entity, an integer
	<BOOL>      : one of {0,1}
	<UINT>      : an unsigned integer
	<STRING>    : an ASCII string
	<ADDRESS>   : an IP address
	<LOGLEVEL>  : one of {debug,info,warning,error}
	<PORTTYPE>  : one of {serial,client,server}
	<FLOWTYPE>  : one of {none,xonxoff,hardware}
	<PARITY>    : one of {none,odd,even}


- Declaration of a DNP3 Proxy 

		proxy-<ID> = "<NAME>"


- Configuration of a DNP3 Proxy

		<NAME>-is-master = "<BOOL>"
		<NAME>-address = "<UINT>"
		<NAME>-log-level = "<LOGLEVEL>"
	
	
- Declaration of physical ports on DNP3 Proxy

		<NAME>-phys-port-<ID> = "<PORT>"
	
	
- Declaration of connection to remote DNP3 entities

		<NAME>-connection-<ID> = "<UINT>"

	
- (Common) Configuration of a physical port

		<PORT>-type = "<PORTTYPE>"
		<PORT>-timeout = "<UINT>"

		
- Configuration of a serial physical port

		<PORT>-device = "<STRING>"
		<PORT>-baud = "<UINT>"
		<PORT>-data-bits = "<UINT>"
		<PORT>-stop-bits = "<UINT>"
		<PORT>-flow-type = "<FLOWTYPE>"
		<PORT>-parity = "<PARITY>"
	
	
- Configuration of a TCP (client and server) physical port

		<PORT>-address = "<ADDRESS>"
		<PORT>-port = "<UINT>"
	

- Configuration of a connection to a remote DNP3 entity 

		<NAME>-<DNP3ADDR>-phys-port = "<PORT>"
		<NAME>-<DNP3ADDR>-timeout-app = "<UINT>"
		<NAME>-<DNP3ADDR>-timeout-link = "<UINT>"
		<NAME>-<DNP3ADDR>-frag-size = "<UINT>"
		<NAME>-<DNP3ADDR>-retries = "<UINT>" 
		<NAME>-<DNP3ADDR>-use-confirms = "<BOOL>"
	


Example Master Proxy Configuration
----------------------------------
[Example Master Proxy Configuration]: #Example_Master_Proxy_Configuration
<a id="Example_Master_Proxy_Configuration"></a> 

		/* This is the first proxy specified in
		   the configuration file */
		proxy-0 = "Master102"
		
		//Proxy Configuration
		Master102-is-master = "1"
		Master102-address = "102"
		Master102-log-level = "warning"
		
		//Declaration of physical ports
		Master102-phys-port-0 = "Master102TCPClient1"
		
		//Declaration of connection to remote slaves
		Master102-connection-0 = "11"
		
		//TCP Client configuration
		Master102TCPClient1-timeout = "1000"
		Master102TCPClient1-type = "client"
		Master102TCPClient1-address = "127.0.0.1"
		Master102TCPClient1-port = "4900"
		
		//Configuration of connection to Slave11
		Master102-11-phys-port = "Master102TCPClient1"
		Master102-11-timeout-app = "10000"
		Master102-11-timeout-link = "1000"
		Master102-11-frag-size = "2048"
		Master102-11-retries = "3" 
		Master102-11-use-confirms = "1"


Example Slave Proxy Configuration
---------------------------------
[Example Slave Proxy Configuration]: #Example_Slave_Proxy_Configuration
<a id="Example_Slave_Proxy_Configuration"></a> 

		/* This is the second proxy specified in
		   the configuration file */
		proxy-1 = "Slave11"
		
		//Proxy Configuration
		Slave11-is-master = "0"
		Slave11-address = "11"
		Slave11-log-level = "warning"
		
		//Declaration of physical ports
		Slave11-phys-port-0 = "Slave11TCPServer102"
		
		//Declaration of connection to remote endpoints
		Slave11-connection-0 = "102"
		
		//TCP Server configuration
		Slave11TCPServer102-timeout = "1000"
		Slave11TCPServer102-type = "server"
		Slave11TCPServer102-address = "127.0.0.1"
		Slave11TCPServer102-port = "4902"
		
		//Configuration of connection to Master102
		Slave11-102-phys-port = "Slave11TCPServer102"
		Slave11-102-timeout-app = "10000"
		Slave11-102-timeout-link = "1000"
		Slave11-102-frag-size = "2048"
		Slave11-102-retries = "3"
		Slave11-102-use-confirms = "1"
