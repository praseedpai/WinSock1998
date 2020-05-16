/////////////////////////////////////////////// 
//
// Socket Client Application
//
//
//
//
//
/////////////////////////////////////
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
/////////////////////////////
//
// Helper Function To Start Socket
// 
BOOL StartSocket();
DWORD WINAPI MessageBoxThread(
  LPVOID lpParameter   
);
///////////////////////////
//
//  
//
//
char ServerName[255];   
WORD PortNumber;        
struct sockaddr_in Server; 
struct hostent     *HostPointer; 
unsigned int addr;   
SOCKET ConnectSock; 
char Buffer[128];

HANDLE g_hv;
char bfr[255];
/////////////////////////////////////
//
//
//
//

int main(int argc, char* argv[])
{

	if ( argc != 4 )
	{
      fprintf(stdout,"Usage: Server <ServerName ><portid> <message> \n");  
      return 1;
	}

	if ( !StartSocket() )
	{
      fprintf(stdout,"Failed To Initialize Socket Library\n");
	  return 1;
	}

	strcpy(ServerName,argv[1] );
	PortNumber = atoi(argv[2]);


   
	if (isalpha(ServerName[0])) {   
		HostPointer= gethostbyname(ServerName);
	}
	else  { /* Convert nnn.nnn address to a usable one */
		addr = inet_addr(ServerName);
		HostPointer = gethostbyaddr((char *)&addr,4,AF_INET);
	}
	if (HostPointer == NULL ) {
		fprintf(stderr,"Client: Cannot resolve address [%s]: Error %d\n",
			ServerName,WSAGetLastError());
		WSACleanup();
		exit(1);
	}

	printf("*");
	
    printf(" **");
    memset(&Server,0,sizeof(Server));
	printf(" ***");
    memcpy(&(Server.sin_addr),HostPointer->h_addr,
		HostPointer->h_length);

    

	Server.sin_family = HostPointer->h_addrtype;
	Server.sin_port = htons(PortNumber);
	printf("***");   

	
	ConnectSock = socket(AF_INET,SOCK_STREAM,0); 
	if (ConnectSock <0 ) {
		fprintf(stderr,"Client: Error Opening socket: Error %d\n",
			WSAGetLastError());
		WSACleanup();
		return -1;
	}


	if (connect(ConnectSock,
		(struct sockaddr*)&Server,
		sizeof(Server))
		== SOCKET_ERROR) {
		fprintf(stderr,"connect() failed: %d\n",WSAGetLastError());
		WSACleanup();
		return -1;
	}
    DWORD Thid;

    g_hv = CreateEvent(0,FALSE,FALSE,0);


    CreateThread(NULL,0,MessageBoxThread,NULL,
		0,&Thid);
   
    while (1) {
		gets(Buffer);

		if (stricmp(Buffer,"END") == 0 )
		{

               break;

		}
          int RetVal = send(ConnectSock,Buffer,strlen(Buffer),0);

	   if (RetVal == SOCKET_ERROR) {
	 	fprintf(stderr,"send() failed: error %d\n",WSAGetLastError());
		WSACleanup();
		return -1;
	   }

          
          memset(bfr,0,255);
	  RetVal
		 = recv(ConnectSock,bfr,sizeof(bfr),0);
  	  if ( RetVal == 0) 
		 continue; 

          SetEvent(g_hv);          

	  }

	 closesocket(ConnectSock);
	 WSACleanup();




	return 0;
}


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



DWORD WINAPI MessageBoxThread(
  LPVOID lpParameter   
)
{

while (1)
{
  WaitForSingleObject(g_hv , INFINITE);
  MessageBox(GetFocus(),"HELLO " , bfr , MB_OK ); 
   
	

}


}