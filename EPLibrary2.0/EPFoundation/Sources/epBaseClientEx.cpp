/*! 
BaseClientEx for the EpLibrary
Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "stdafx.h"
#include "epBaseClientEx.h"
#include "epThread.h"

using namespace epl;

BaseClientEx::BaseClientEx(CString hostName, CString port)
{
	m_port=NULL;
	m_hostName=NULL;
	SetHostName(hostName);
	SetPort(port);
	m_connectSocket=NULL;
	m_result=0;
	m_ptr=0;
	m_isConnected=false;
}
BaseClientEx::~BaseClientEx()
{
	if(m_port)
		EP_DELETE[] m_port;
	if(m_hostName)
		EP_DELETE[] m_hostName;
	Disconnect();
}

bool BaseClientEx::SetHostName(CString hostName)
{
	LockObj lock(&m_generalLock);
	if(m_isConnected)
		return false;
	if(m_hostName==NULL)
		m_hostName=EP_NEW char[HOSTNAME_MAX_SIZE];

	memset(m_hostName,0,HOSTNAME_MAX_SIZE);

	if(hostName.GetLength()==0)
		memcpy(m_hostName,DEFAULT_HOSTNAME,sizeof(DEFAULT_HOSTNAME));
	else
	{
		wcstombs(m_hostName,hostName.GetString(),hostName.GetLength());
		m_hostName[hostName.GetLength()]='\0';
	}
	return true;

}
bool BaseClientEx::SetPort(CString port)
{
	LockObj lock(&m_generalLock);
	if(m_isConnected)
		return false;
	if(m_port==NULL)
		m_port=EP_NEW char[PORT_MAX_SIZE];

	memset(m_port,0,PORT_MAX_SIZE);

	if(port.GetLength()==0)
		memcpy(m_port,DEFAULT_PORT,sizeof(DEFAULT_PORT));
	else
	{
		wcstombs(m_port,port.GetString(),port.GetLength());
		m_port[port.GetLength()]='\0';
	}
	return true;

}

CString BaseClientEx::GetHostName()
{
	if(!m_hostName)
		return _T("");
	CString retString;
	TCHAR hostName[HOSTNAME_MAX_SIZE];
	System::MultiByteToWideChar(m_hostName,HOSTNAME_MAX_SIZE,hostName);
	retString=hostName;
	return retString;
}
CString BaseClientEx::GetPort()
{
	if(!m_port)
		return _T("");
	CString retString;
	TCHAR port[PORT_MAX_SIZE];
	System::MultiByteToWideChar(m_port,PORT_MAX_SIZE,port);
	retString=port;
	return retString;
}

int BaseClientEx::Send(const Packet &packet)
{
	LockObj lock(&m_sendLock);
	int writeLength=0;
	const char *packetData=packet.GetPacket();
	int length=packet.GetPacketByteSize();
	if(length>0)
	{
		int sentLength=send(m_connectSocket,(char*)&length,4,0);
		if(sentLength<=0)
			return false;
	}
	while(length>0)
	{
		int sentLength=send(m_connectSocket,packetData,length,0);
		writeLength+=sentLength;
		if(sentLength<=0)
		{
			Disconnect();
			return writeLength;
		}
		length-=sentLength;
		packetData+=sentLength;
	}
	return writeLength;
}

int BaseClientEx::receive(Packet &packet)
{
	int readLength=0;
	int length=packet.GetPacketByteSize();
	char *packetData=(char*)packet.GetPacket();
	while(length>0)
	{
		int recvLength=recv(m_connectSocket,packetData, length, 0);
		readLength+=recvLength;
		if(recvLength<=0)
			break;
		length-=recvLength;
		packetData+=recvLength;
	}
	return readLength;
}

bool BaseClientEx::Connect()
{
	LockObj lock(&m_generalLock);
	if(m_isConnected)
		return true;
	if(m_port==NULL)
	{
		m_port=EP_NEW char[PORT_MAX_SIZE];
		memcpy(m_port,DEFAULT_PORT,sizeof(DEFAULT_PORT));
	}

	if(m_hostName==NULL)
	{
		m_hostName=EP_NEW char[HOSTNAME_MAX_SIZE];
		memcpy(m_hostName,DEFAULT_HOSTNAME,sizeof(DEFAULT_HOSTNAME));
	}


	WSADATA wsaData;
	m_connectSocket = INVALID_SOCKET;
	struct addrinfo hints;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		MessageBox(NULL,_T("WSAStartup failed with error\n"),_T("Error"),MB_OK);
		return false;
	}

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(m_hostName, m_port, &hints, &m_result);
	if ( iResult != 0 ) {
		MessageBox(NULL,_T("getaddrinfo failed with error\n"),_T("Error"),MB_OK);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for(m_ptr=m_result; m_ptr != NULL ;m_ptr=m_ptr->ai_next) {

		// Create a SOCKET for connecting to server
		m_connectSocket = socket(m_ptr->ai_family, m_ptr->ai_socktype, 
			m_ptr->ai_protocol);
		if (m_connectSocket == INVALID_SOCKET) {
			MessageBox(NULL,_T("socket failed with error\n"),_T("Error"),MB_OK);
			Disconnect();
			return false;
		}

		// Connect to server.
		iResult = connect( m_connectSocket, m_ptr->ai_addr, (int)m_ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(m_connectSocket);
			m_connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	if (m_connectSocket == INVALID_SOCKET) {
		MessageBox(NULL,_T("Unable to connect to server!\n"),_T("Error"),MB_OK);
		Disconnect();
		return false;
	}
	m_isConnected=true;

	m_clientThreadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)BaseClientEx::ClientThread, this, 0, NULL);  
	if(m_clientThreadHandle)
		return true;
	return true;
}
unsigned long BaseClientEx::passPacket(void *param)
{
	Packet *recvPacket=( (PacketPassUnit*)param)->m_packet;
	BaseClientEx *worker=( (PacketPassUnit*)param)->m_this;
	worker->parsePacket(*recvPacket);
	recvPacket->Release();
	return 0;
}

DWORD BaseClientEx::ClientThread( LPVOID lpParam ) 
{
	BaseClientEx *pMainClass=(BaseClientEx*)lpParam;
	int iResult;
	/// Receive buffer
	Packet recvSizePacket(NULL,4);
	// Receive until the peer shuts down the connection
	do {
		int size =pMainClass->receive(recvSizePacket);
		if(size>0)
		{
			Packet *recvPacket=new Packet(NULL,size);
			iResult = pMainClass->receive(*recvPacket);

			if (iResult == size) {
				Thread::ThreadID threadID;
				PacketPassUnit passUnit;
				passUnit.m_packet=recvPacket;
				passUnit.m_this=pMainClass;
				::CreateThread(NULL,0,passPacket,&passUnit,Thread::THREAD_OPCODE_CREATE_START,(LPDWORD)&threadID);
			}
			else if (iResult == 0)
			{
				printf("Connection closing...\n"); // TODO:: LOG
				recvPacket->Release();
				break;
			}
			else  {
				printf("recv failed with error\n"); // TODO:: LOG
				recvPacket->Release();
				break;
			}
		}
		else
		{
			break;
		}

	} while (iResult > 0);
	return 0; 
}

bool BaseClientEx::IsConnected()
{
	return m_isConnected;
}


void BaseClientEx::Disconnect()
{
	LockObj lock(&m_generalLock);
	// No longer need server socket
	if(m_result)
		freeaddrinfo(m_result);

	if(m_isConnected)
	{
		// shutdown the connection since no more data will be sent
		int iResult = shutdown(m_connectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());// TODO LOG
		}
		closesocket(m_connectSocket);
	}
	m_isConnected=false;
	m_connectSocket = INVALID_SOCKET;
	m_result=NULL;
	WSACleanup();
}

