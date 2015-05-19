#ifndef _FT_SOCKET_H_
#define _FT_SOCKET_H_

#define FT_TCP 0
#define FT_UDP 1

namespace ftSocket {

bool init();
void close();

class Server
{
public:
	Server();
	void listen(int port);
	void accept();
	void sendMsg();
	void recvMsg();
};

class Client
{
	char buffer[1024];
public:
	Client();
	void connect(const char *addr, int port);
	void sendMsg();
	void recvMsg();
};

}

#endif
