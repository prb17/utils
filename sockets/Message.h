#pragma once
#include "../include/Utilities.h"
#include<string>

///////////////////////////////////////////////////////////////////
  // EndPoint struct

struct EndPoint
{
	using Address = std::string;
	using Port = size_t;
	Address address;
	Port port;
	EndPoint(Address anAddress = "", Port aPort = 0);
	std::string toString();
	static EndPoint fromString(const std::string& str);
};

inline EndPoint::EndPoint(Address anAddress, Port aPort) : address(anAddress), port(aPort) {}

inline std::string EndPoint::toString()
{
	return address + ":" + Utilities::Converter<size_t>::toString(port);
}

inline EndPoint EndPoint::fromString(const std::string& str)
{
	EndPoint ep;
	size_t pos = str.find_first_of(':');
	if (pos == str.length())
		return ep;
	ep.address = str.substr(0, pos);
	std::string portStr = str.substr(pos + 1);
	ep.port = Utilities::Converter<size_t>::toValue(portStr);
	return ep;
}
///////////////////////////////////////////////////////////////////
// Message class
// - follows the style, but not the implementation details of
//   HTTP messages

class Message {
public:
	enum MESSAGE_TYPE {
		UNITIALIZED,
		TEST_REQUEST,
		WORKER_MESSAGE
	};

	Message();
	Message(EndPoint, EndPoint);

	//getters
	EndPoint getSource();
	EndPoint getDestination();
	std::string getName();
	MESSAGE_TYPE getMsgType();
	std::string getAuthor();
	std::string getDate();
	std::string getMsgBody();
	EndPoint getTestRequester();

	//setters
	void setSource(EndPoint);
	void setDestination(EndPoint);
	void setName(std::string);
	void setMsgType(MESSAGE_TYPE);
	void setAuthor(std::string);
	void setDate(std::string);
	void setMsgBody(std::string);
	void setTestRequester(EndPoint);

	std::string toString();
	static Message fromString(const std::string&);

	EndPoint source;
	EndPoint destination;
	std::string name;
	MESSAGE_TYPE msg_type;
	std::string author;
	std::string date;
	std::string msg_body;
	EndPoint testRequester;
};