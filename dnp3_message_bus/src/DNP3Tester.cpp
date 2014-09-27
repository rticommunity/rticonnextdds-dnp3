/*******************************************************************************
 (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/

#include <iostream>

#include <DNP3/AsyncStackManager.h>
#include <APL/Log.h>
#include <APL/LogToStdio.h>
#include <APL/Loggable.h>
#include <APL/Lock.h>

#include <DNP3/APDUProxyStackConfig.h>
#include <DNP3/APDUProxyStack.h>
#include <DNP3/MasterStackConfig.h>

#include "MessageBus.h"

using namespace std;
using namespace apl;
using namespace apl::dnp;
using namespace boost;

namespace rti {

namespace dnp3 {

class TestAPDUListener: public APDUListener {
public:

	TestAPDUListener(
			const char* listName, APDUProxy* masterProxy,
			APDUProxy* slaveProxy) :
			name(listName),
					master(masterProxy),
					slave(slaveProxy) {
	}
	;
	~TestAPDUListener() {

	}
	;
	std::string& getName() {
		return name;
	}
	;
	void newRequestAPDU(
			APDUProxy* proxy, APDU& apdu) {
		cout << "TEST_APDU_LISTENER " << name << " : Received REQUEST : "
				<< apdu.ToString();

		slave->SendRequestAPDU(apdu);
	}
	;
	void newResponseAPDU(
			APDUProxy* proxy, APDU& apdu) {
		cout << "TEST_APDU_LISTENER " << name << " : Received RESPONSE : "
				<< apdu.ToString();

		master->SendResponseAPDU(apdu);

	}
	;
	void newUnsolResponseAPDU(
			APDUProxy* proxy, APDU& apdu) {
		cout << "TEST_APDU_LISTENER " << name << " : Received UNSOL.RESPONSE "
				<< apdu.ToString();

		master->SendUnsolResponseAPDU(apdu);

	}
	;

private:
	string name;
	APDUProxy* master;
	APDUProxy* slave;
};

}

}

void testSendUnsolAPDU(
		APDUProxyStack * slaveProxyStack, size_t fragSize, uint16_t masterAddr,
		uint16_t slaveAddr) {
	cout << "Sending an APDU created locally...\n";

	APDU apdu(fragSize);
	//the function code must be set immediately because
	//it determines the type of header that will be contained
	//by the APDU and SetControl() will fail if this is not set.
	enum FunctionCodes apduCode = FC_UNSOLICITED_RESPONSE;
	apdu.SetFunction(apduCode);

	bool apduFIR(true);
	bool apduFIN(true);
	bool apduCON(true);
	bool apduUNS(true);
	int apduSEQ(1);
	AppControlField apduCtrlField(apduFIR, apduFIN, apduCON, apduUNS, apduSEQ);
	apdu.SetControl(apduCtrlField);

	uint8_t apduIIN_LSB(0);
	uint8_t apduIIN_MSB(0);
	IINField apduIIN(apduIIN_LSB, apduIIN_MSB);

	apdu.SetIIN(apduIIN);

	APDUInfo apduInfo;
	apduInfo.dest = masterAddr;
	apduInfo.src = slaveAddr;
	apduInfo.master = false;

	enum CommandStatus result = slaveProxyStack->proxy->SendUnsolResponseAPDU(
			apdu);

	if (result != CS_SUCCESS) {
		cout << "Error sending APDU!\n";
	} else {
		cout << "Oh yeah!\n";
	}
}

#define DEBUG_LEVEL LEV_DEBUG

using namespace rti::dnp3;

int main(

		int argc,
		char** argv) {
	EventLog log;
	log.AddLogSubscriber(LogToStdio::Inst());
	const FilterLevel LOG_LEVEL = DEBUG_LEVEL;

	/* AsyncStackManager is the main entity of OpenDNP3
	 * which lets users create physical connections and
	 * DNP3 stacks (master and/or slaves) over this
	 * channels.
	 */
	AsyncStackManager mgr(log.GetLogger(LOG_LEVEL, "dnp"));

	/* Configuration for the physical (TCP) connections */
	millis_t physTimeout(1000);
	PhysLayerSettings physSettings(DEBUG_LEVEL, physTimeout);
	string tcpServerName("InputServer");
	string tcpClientName("OutputClient");
	string localIP("127.0.0.1");
	uint16_t localPort(4999);

	/* create a TCP server that will be used by the SlaveProxy
	 * to receive in-bound connections from masters.
	 */
	mgr.AddTCPServer(tcpServerName, physSettings, localIP, localPort);

	/* create a TCP client that will be used by the MasterProxy
	 * to connect to slaves.
	 */
	mgr.AddTCPClient(tcpClientName, physSettings, localIP, localPort - 1);

	/* Configuration for DNP3 proxy stacks. The DNP3 proxy stacks
	 * are created using DNP3 link layer's addresses which are
	 * automatically mapped by OpenDNP3 to the physical
	 * port selected for the stack. */
	uint16_t slaveAddr(1);
	uint16_t masterAddr(100);
	bool useConfirms(true);
	millis_t linkTimeout(1000);
	millis_t appTimeout(1000);
	size_t numRetry(3);
	size_t fragSize(2048);
	string slaveProxyName("SlaveProxy");
	string masterProxyName("MasterProxy");


	/* Create a proxy for a Slave. This proxy will act on
	 * behalf of a certain (slaveAddr) DNP3 slave and expect
	 * to communicate with a certain (masterAddr) master
	 */
	APDUProxyStackConfig slaveProxyConfig(slaveAddr, masterAddr, false,
			useConfirms,
			linkTimeout, appTimeout, numRetry, fragSize);
	APDUProxyStack* slaveProxyStack = mgr.AddProxyStack(tcpServerName,
			slaveProxyName,
			DEBUG_LEVEL, slaveProxyConfig);

	/* Create a proxy for a Master. Similarly to the one for
	 * the Slave, this proxy sends/receives APDUs on behalf
	 * of a certain (masterAddr) master to a certain (slaveAddr)
	 * slave.*/
	APDUProxyStackConfig masterProxyConfig(masterAddr, slaveAddr, true, false //masters cannot use confirms
			, linkTimeout, appTimeout, numRetry, fragSize);
	APDUProxyStack* masterProxyStack = mgr.AddProxyStack(tcpClientName,
			masterProxyName,
			DEBUG_LEVEL, masterProxyConfig);

	/* Create a listener that will be notified by the proxies
	 * of new APDUs received. This listener will act as a
	 * connector between the two entities and their respective
	 * proxies, forwarding each received message from one to
	 * the other.
	 */
	TestAPDUListener *listener = new TestAPDUListener("TestListener",
			slaveProxyStack->proxy,
			masterProxyStack->proxy);

	slaveProxyStack->proxy->AddAPDUListener(listener);
	masterProxyStack->proxy->AddAPDUListener(listener);

	char i = 0;
	cout << "Press any key to quit...\n";
	cin >> i;

	/* try to create an APDU message in code and send it to a master */
	testSendUnsolAPDU(slaveProxyStack, fragSize, masterAddr, slaveAddr);

	cout << "Exiting...\n";

	/* remove proxy stacks */
	mgr.RemoveStack(slaveProxyName);
	mgr.RemoveStack(masterProxyName);

	/* remove physical ports */
	mgr.RemovePort(tcpServerName);
	mgr.RemovePort(tcpClientName);

}

