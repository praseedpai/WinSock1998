// Server.cpp : Defines the entry point for the console application.
//
//
//
//
//
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

/////////////////////////////
//
//
// 

BOOL StartSocket();

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


	while (1)
	{
		memset(Buffer , 0 ,sizeof(Buffer));

		int Len = sizeof( RemoteAddress );
		InComingSocket = accept(ListnerSocket,
			(struct sockaddr*)&RemoteAddress, &Len);
		if (InComingSocket == INVALID_SOCKET) 
		{
			fprintf(stderr,"accept error %d\n",WSAGetLastError());
			WSACleanup();
			return -1;
		}

		int RetVal = recv(InComingSocket,
			Buffer,sizeof (Buffer),0 );

		if (RetVal == SOCKET_ERROR) {
			fprintf(stderr,
				"recv() failed: error %d\n",WSAGetLastError());
			closesocket(InComingSocket);
			continue;
		}

		if (RetVal == 0) {
			printf("Client closed connection\n");
			closesocket(InComingSocket);
			continue;
		}


		printf("===================================\n");
        printf("String in the Buffer\n\n" );
        printf("%s\n" ,Buffer); 
		printf("===================================\n");
		
		closesocket(InComingSocket);

    }

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


