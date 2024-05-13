#pragma once 

enum MessageTypes
{
	MT_START,
	MT_CLOSE,
	MT_DATA,
	MT_GET
};


struct MessageHeader
{
	int messageType;
	int size;
	int addr;
};


struct Message
{
	MessageHeader header = { 0 };
	std::string data;

	Message() = default;

	Message(MessageTypes messageType, int addr = -1, const std::string& data = "")
		:data{ data }
	{
		header = { messageType, int(data.length()), addr};
	}
};
