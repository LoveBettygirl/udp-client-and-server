#pragma once
#include "afxsock.h"
class CUDPSocket : public CAsyncSocket
{
public:
	CUDPSocket();
	~CUDPSocket();
	virtual void OnReceive(int nErrorCode);
};

