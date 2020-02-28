/*
 * Helpers.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: cbai
 */
#include <stdexcept>
#include <string>
#include <iostream>
using namespace std;

int intinput()
{
	char c;
	while (1)
	{
		cin >>c;
		try
		{
			return stoi(""+c);
		}
		catch (...)
		{
			printf("non numeric input\n>> ");
		}
	}
	return -1;
}
