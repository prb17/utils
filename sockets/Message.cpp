#include "../include/Message.h"
#include <sstream>

Message::Message() {}
Message::Message(EndPoint dest, EndPoint src) {
	source = src;
	destination = dest;
	name = "name";
	msg_type = UNITIALIZED;
	author = "author";
	date = "date";
	msg_body = "msg body";
	testRequester = EndPoint();
}

//getters
EndPoint Message::getSource() { return source; }
EndPoint Message::getDestination() { return destination; }
std::string Message::getName() { return name; }
Message::MESSAGE_TYPE Message::getMsgType() { return msg_type; }
std::string Message::getAuthor() { return author; }
std::string Message::getDate() { return date; }
std::string Message::getMsgBody() { return msg_body; }
EndPoint Message::getTestRequester() { return testRequester; }

//setters
void Message::setSource(EndPoint ep) { source = ep; }
void Message::setDestination(EndPoint ep) { destination = ep; }
void Message::setName(std::string str) { name = str; }
void Message::setMsgType(MESSAGE_TYPE mt) { msg_type = mt; }
void Message::setAuthor(std::string str) { author = str; }
void Message::setDate(std::string str) { date = str; }
void Message::setMsgBody(std::string str) { msg_body = str; }
void Message::setTestRequester(EndPoint ep) { testRequester = ep; }

std::string Message::toString() {
	std::stringstream ss;
	ss << source.toString() << "\n"
		<< destination.toString() << "\n"
		<< name << "\n"
		<< msg_type << "\n"
		<< author << "\n"
		<< date << "\n"
		<< msg_body << "\n"
		<< testRequester.toString() << "\n\n";
	return ss.str();
}

Message Message::fromString(const std::string& str) {
	Message msg;
	std::vector<std::string> splits = Utilities::StringHelper::split(str);
	msg.setSource(EndPoint::fromString(splits[0]));
	msg.setDestination(EndPoint::fromString(splits[1]));
	msg.setName(splits[2]);
	msg.setMsgType(static_cast<MESSAGE_TYPE>(std::stoi(splits[3])));
	msg.setAuthor(splits[4]);
	msg.setDate(splits[5]);
	msg.setMsgBody(splits[6]);
	msg.setTestRequester(EndPoint::fromString(splits[7]));
	return msg;
}