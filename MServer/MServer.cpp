// Server.cpp : Defines the entry point for the console application.
//
//
//
//
//
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

////////////////////////////////
//
//
//
typedef struct
{
 SOCKET Sh;
 
}CLIENT_DATA;

/////////////////////////////
//
//
// 

BOOL StartSocket();
DWORD WINAPI ListenThreadProc( LPVOID lpParameter);
DWORD WINAPI ClientThreadProc( LPVOID lpParam   );

/////////////////////////////
//
//
//
int ProtocolPort = 3500; 
struct sockaddr_in LocalAddress;
struct sockaddr_in RemoteAddress;
SOCKET ListnerSocket;
SOCKET InComingSocket;
char   Buffer[1024];
CRITICAL_SECTION m_CriticalSection;

///////////////////////////////////////
//
//




int main(int argc, char* argv[])
{


	if ( argc != 2 )
	{
      fprintf(stdout,"Usage: Server <portid> \n");  
      return 1;
	}
	if ( !StartSocket() )
	{
      fprintf(stdout,"Failed To Initialize Socket Library\n");
	  return 1;
	}

    ProtocolPort = atoi(argv[1]); 

    LocalAddress.sin_family = AF_INET;
	LocalAddress.sin_addr.s_addr = INADDR_ANY;
	LocalAddress.sin_port = htons(ProtocolPort);

	if ( ( ListnerSocket = socket(AF_INET, SOCK_STREAM,0)) ==
		 INVALID_SOCKET ) 
	{
    	fprintf(stdout,"call to socket failed with error %d\n",
			WSAGetLastError());
		WSACleanup();
		return -1;
	}

	if (bind(ListnerSocket,(struct sockaddr*)&LocalAddress,
		sizeof(LocalAddress) ) 
		== SOCKET_ERROR) {
		fprintf(stderr,"Socket Error %d\n",WSAGetLastError());
		WSACleanup();
		return -1;
	}


	if (listen(ListnerSocket,5) == SOCKET_ERROR) 
	{
	 fprintf(stderr,"Socket Error %d\n",WSAGetLastError());
	 WSACleanup();
	 return -1;
	}

	InitializeCriticalSection(&m_CriticalSection);
    DWORD  Thid;
	CreateThread(NULL,0,ListenThreadProc,NULL,
		0,&Thid);

	while(1)
		;


	
   	WSACleanup();
	return 0;
}


/////////////////////////////////
//
// Initialise Winsock Libraries 
// 
//
//
BOOL StartSocket()
{
  WORD Ver;
  WSADATA wsd;
  Ver = MAKEWORD( 2, 2 );
  if (WSAStartup(Ver,&wsd) == SOCKET_ERROR) 
  {
	WSACleanup();
	return FALSE;
  }

  return TRUE;
}


/////////////////////////////////
//
//
//
//
DWORD WINAPI ListenThreadProc(
  LPVOID lpParameter   
)
{

while (1)
{
	int Len = sizeof( RemoteAddress );
	InComingSocket = accept(ListnerSocket,
	(struct sockaddr*)&RemoteAddress, &Len);
	if (InComingSocket == INVALID_SOCKET) 
	{
		fprintf(stderr,"accept error %d\n",WSAGetLastError());
		WSACleanup();
		return -1;
	}

	CLIENT_DATA ClientData;
	DWORD ThreadId;
	ClientData.Sh = InComingSocket;
	::CreateThread(NULL,0,ClientThreadProc,(LPVOID)&ClientData,
		0,&ThreadId);

}


}

/////////////////////////////////////
//
//
//
//
//
//
DWORD WINAPI ClientThreadProc(
  LPVOID lpParam   
)
{
 CLIENT_DATA CData;
 memcpy(&CData,lpParam,sizeof(CLIENT_DATA));
 char bfr[255];

 while (1)
 {
	 memset(bfr,0,255);
	 int RetVal
		 = recv(CData.Sh ,bfr,sizeof(bfr),0);

	 if ( RetVal == 0) 
		 return 0; 

	 EnterCriticalSection(&m_CriticalSection);
	 printf("==========================\n");
	 printf(bfr);
     printf("\n==========================\n");
	 LeaveCriticalSection(&m_CriticalSection);

}

}