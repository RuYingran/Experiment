#include<iostream>
#include <string.h>
#include <stdio.h>
#include <string>
#include"sm3_SIMD.h"
using namespace std;

int main()
{
	printf("please enter the message:\n");
	std::string input_str;
	std::cin >> input_str;
	int dlen = input_str.size();

	unsigned char* message = (unsigned char*)input_str.c_str();
	unsigned char res[32];

	printf("Message:\n%s\n", message);

	sm3(message, dlen, res);
	printf("Hash:\n");
	for (int i = 0; i < 32; i++)
	{
		printf("%02x", res[i]);
		if (((i + 1) % 4) == 0) printf(" ");
	}
	printf("\n");
}

