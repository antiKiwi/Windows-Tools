#include <stdio.h>
#include <string.h>
#include <Windows.h>

int main(int argc, char* argv[])
{
	char* pass = "Password";

	while (strcmp(pass, argv[1]))
	{
		printf("The password is not equal!\n");
		Sleep(2500);
	}
	
	printf("\n** Oops What heppend ? IAT Hooking Done!! **\n");
	
	system("pause");
	return 0;
}