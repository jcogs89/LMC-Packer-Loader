#include <iostream>
#include <fstream>
using namespace std;
//using System;
//using System.Net;
//using System.Net.Sockets;
//using System.Text;

void Tlistnr() {
    int portNum = 8000;
    
	bool done = false;

	var listener = new TcpListener(IPAddress.Any, portNum);

	listener.Start();

	while (!done)
	{
		Console.Write("Waiting for connection...");
		TcpClient client = listener.AcceptTcpClient();

		Console.WriteLine("Connection accepted.");
		NetworkStream ns = client.GetStream();

		//byte[] byteTime = Encoding.ASCII.GetBytes(DateTime.Now.ToString());
		byte[] byteData[100];

		try
		{
		//    ns.Write(byteTime, 0, byteTime.Length);
			ns.Write(byteData, 0, 100)
			outdata.open("mal.exe");
			outdata << bytedata << endl;
			outdata.close();
		
		
			ns.Close();
			client.Close();
		}
		catch (Exception e)
		{
			Console.WriteLine(e.ToString());
		}
	}	

	listener.Stop();

	return 0;
}

//int main() {
//	Tlistnr()
//	
//	// Temporary... file will eventually be pulled in from some .NET framework with networking while running as a service to recieve packed data.
//		ifstream infile;
//		char data[100]; 
//		
//		infile.open("hello_world.txt", ios::in);
//		
//		cout << "Reading from the file" << endl;
//		infile >> data;
//		
//		// write data to screen.
//		cout << data << endl;
//		
//		// close opened input file.
//		infile.close();
//	
//	return 0;
//}