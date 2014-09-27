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

#include <stdlib.h>

#include "MessageBusImpl.h"
#include <DNP3/AsyncStackManager.h>

namespace rti {
namespace dnp3 {

using namespace apl::dnp;

Connection::Connection(
		string& connName,
		RTIDNP3_ConnectionId_t connId,
		RTIDNP3_ConnectionConfig *connConfig,
		Proxy *connProxy, apl::Logger *logger) :
		apl::Loggable(logger),
		name(connName),
		id(connId),
				config(*connConfig),
				proxy(connProxy),
				apduProxy(NULL),
				apdu(NULL),
				apduProxyName("") {

	ostringstream apduProxyName;

	apduProxyName << "Connection-" << connId << "-APDUProxy";

	this->apduProxyName = apduProxyName.str();

	this->config.remoteAddr = connConfig->remoteAddr;
	this->config.fragSize = connConfig->fragSize;
	this->config.linkTimeout = connConfig->linkTimeout;
	this->config.numRetry = connConfig->numRetry;
	this->config.useConfirms = connConfig->useConfirms;
	this->config.appTimeout = connConfig->appTimeout;
	this->config.portName = strdup(connConfig->portName);

	RTIDNP3_LOG_LEV_CONVERT_TO_DNP3(this->proxy->GetConfig()->logLevel,
				this->logLevel);

	const char* type = (this->proxy->GetConfig()->isMaster)?"master":"slave";

	this->CreateAPDUProxy();

	this->apdu = new APDU(this->config.fragSize);

}

Connection::~Connection() {
	this->DeleteAPDUProxy();

	delete this->apdu;
	this->apdu = NULL;

//	free(this->config.portName);
}

bool Connection::CreateAPDUProxy() {
	boost::uint16_t localAddr = this->proxy->GetConfig()->localAddr;
	bool isMaster = this->proxy->GetConfig()->isMaster;

	string portName(this->config.portName);
	boost::uint16_t remoteAddr = this->config.remoteAddr;
	bool useConfirms = this->config.useConfirms;
	apl::millis_t linkTimeout = this->config.linkTimeout;
	apl::millis_t appTimeout = this->config.appTimeout;
	size_t numRetry = this->config.numRetry;
	size_t fragSize = this->config.fragSize;

	APDUProxyStackConfig proxyStackConfig(localAddr, remoteAddr, isMaster,
			useConfirms, linkTimeout, appTimeout, numRetry, fragSize);

	proxyStackConfig.dump();

	AsyncStackManager *asyncSM = this->proxy->GetStackManager();
	APDUProxyStack *stack = asyncSM->AddProxyStack(portName,
			this->apduProxyName,
			this->logLevel, proxyStackConfig);


	if (stack != NULL) {
		this->apduProxy = stack->proxy;

		LOG_BLOCK(apl::LEV_INFO, "APDUProxy created.");

		return true;
	} else {

		LOG_BLOCK(apl::LEV_ERROR, "Cannot create APDUProxy.");

		return false;
	}
}

bool Connection::DeleteAPDUProxy() {
	AsyncStackManager *asyncSM = this->proxy->GetStackManager();
	asyncSM->RemoveStack(this->apduProxyName);
	this->apduProxy = NULL;

	LOG_BLOCK(apl::LEV_INFO, "APDUProxy deleted.");

	return true;
}

bool Connection::operator==(
		const Connection& other) {

	return this->id == other.id;
}

bool Connection::operator<(
		const Connection& other) {

	return this->id < other.id;

}

}
}
