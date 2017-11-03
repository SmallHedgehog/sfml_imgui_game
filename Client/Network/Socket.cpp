#include "Socket.h"


int Socket::nofSockets = 0;

/*
通过WSAStartup函数完成对WinSock服务的初始化
*/
void Socket::start()
{
	if (!nofSockets)
	{
		WSADATA info;
		if (WSAStartup(MAKEWORD(2, 0), &info))
		{
			throw "Windows Sockets Asynchronous Can't StartUp";
		}
	}
	++nofSockets;
}

void Socket::end()
{
	WSACleanup();
}

/*
对SOCKET进行封装，提供引用计数
*/
Socket::Socket() :
	_s(0)
{
	start();
	_s = socket(AF_INET, SOCK_STREAM, 0);

	if (_s == INVALID_SOCKET)
		throw "SOCKET: invalid socket!";

	// BOOL bReuseaddr = TRUE;
	// setsockopt(_s, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));

	// 引用计数
	refCounter = new int(1);
}

Socket::Socket(SOCKET s) :
	_s(s)
{
	start();

	// BOOL bReuseaddr = TRUE;
	// setsockopt(_s, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));

	// 引用计数
	refCounter = new int(1);
}

Socket::Socket(const Socket& _socket)
{
	refCounter = _socket.refCounter;
	(*refCounter)++;

	_s = _socket._s;

	nofSockets++;
}

Socket& Socket::operator=(Socket& _socket)
{
	// 引用计数加一
	(*_socket.refCounter)++;

	refCounter = _socket.refCounter;
	_s = _socket._s;

	nofSockets++;

	return *this;
}

Socket::~Socket()
{
	if (!--(*refCounter))
	{
		close();
		delete refCounter;
	}

	--nofSockets;
	if (!nofSockets)
		end();
}

const SOCKET& Socket::getSocket() const
{
	return _s;
}

void Socket::close()
{
	// 关闭套接字
	closesocket(_s);
}

/*
int recv(SOCKET, buffer, size, flags)函数从SOCKET套接字读取内容，当读取成功时，recv返回实际读取的字节数，
如果返回的是0，表示读取结束，而小于0则出现了错误；
（1）如果错误类型为EINTR，表示读的时候出现了中断；
*/
int Socket::read(void* buffer, unsigned int size)
{
	int left_bytes = size;
	int read_bytes = 0;
	char* buffer_ptr = static_cast<char*>(buffer);

	while (left_bytes > 0)
	{
		read_bytes = recv(_s, buffer_ptr, left_bytes, 0);
		if (read_bytes < 0)
		{
			int res = WSAGetLastError();
			if (res == WSAEINTR)
			{
				read_bytes = 0;
			}
			else
			{
				return -1;
			}
		}
		else if (read_bytes == 0)
			break;

		left_bytes -= read_bytes;
		buffer_ptr += read_bytes;
	}

	return (size - read_bytes);
}

int Socket::READ(void* buffer, unsigned int size)
{
	int read_bytes = 0;
	char* buffer_ptr = static_cast<char*>(buffer);

	read_bytes = recv(_s, buffer_ptr, size, 0);
	if (read_bytes < 0)
	{
		int res = WSAGetLastError();
		if (res == WSAEINTR)
		{
			read_bytes = 0;
		}
		else
		{
			return -1;
		}
	}

	return read_bytes;
}

/*
int send(SOCKET, buffer, size, flags)函数将buffer中的size字节内容写入文件描述符SOCKET，成功时返回写的字节数，失败返回-1，
在网络程序中，当我们向套接字文件描述符写时：
（1）send返回值大于0，表示写入了部分或者是全部数据。
（2）send返回值小于0，此时出现了错误，我们需要根据错误类型处理。
如果错误类型为EINTR，表示写的时候出现了中断；
如果错误类型为EPIPE，表示网络连接出现了问题（对方关闭了连接）
*/
int Socket::write(void* buffer, unsigned int size)
{
	int left_bytes = size;
	int send_bytes = 0;
	char* buffer_ptr = static_cast<char*>(buffer);

	while (left_bytes > 0)
	{
		send_bytes = send(_s, buffer_ptr, left_bytes, 0);
		if (send_bytes <= 0)
		{
			int res = WSAGetLastError();
			if (res == WSAEINTR)
			{
				send_bytes = 0;
			}
			else
			{
				return -1;
			}
		}
		left_bytes -= send_bytes;
		buffer_ptr += send_bytes;
	}
	return 0;
}
