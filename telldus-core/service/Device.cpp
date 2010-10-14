#include "Device.h"
#include "Settings.h"

class Device::PrivateData {
public:
	std::wstring model;
	std::wstring name;
	ParameterMap parameterList;
	Protocol *protocol;
	std::wstring protocolName;
	int preferredControllerId;
};

Device::Device(int id)
	:Mutex()
{
	d = new PrivateData;
	d->protocol = 0;
	d->preferredControllerId = 0;
	//n�r n�got uppdateras, spara ocks� till registret
	//obs, kunna hantera om alla v�rden inte �r satta
}

Device::~Device(void) {
	delete d->protocol;
	delete d;
}

/**
* Get-/Set-methods
*/
std::wstring Device::getModel(){
	return d->model;
}

void Device::setModel(const std::wstring &model){
	d->model = model;
}

std::wstring Device::getName(){
	return d->name;
}

void Device::setName(const std::wstring &name){
	d->name = name;
}

std::wstring Device::getParameter(const std::wstring &key){
	return d->parameterList[key];
}

void Device::setParameter(const std::wstring &key, const std::wstring &value){
	d->parameterList[key] = value;
}

int Device::getPreferredControllerId(){
	return d->preferredControllerId;
}

void Device::setPreferredControllerId(int controllerId){
	d->preferredControllerId = controllerId;
}

std::wstring Device::getProtocolName(){
	return d->protocolName;
}

void Device::setProtocolName(const std::wstring &protocolName){
	d->protocolName = protocolName;
}

/**
* End Get-/Set
*/

int Device::doAction(int action,  const std::wstring &data, Controller *controller) {
	Protocol *p = this->retrieveProtocol();
	if(p){
		std::string stringData(data.begin(), data.end());	//conversion needed
		std::string code = p->getStringForMethod(action, stringData, controller);
		controller->send(code);
	}
	return 0;
}

Protocol* Device::retrieveProtocol() {
	if (d->protocol) {
		return d->protocol;
	}
	
	d->protocol = Protocol::getProtocolInstance(d->protocolName);
	if(d->protocol){
		d->protocol->setModel(d->model);
		d->protocol->setParameters(d->parameterList);
		return d->protocol;
	}

	return 0;
}