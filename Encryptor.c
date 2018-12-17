//This program is designed to do a simple encryption algorithim on a text file (Need Encryption_Reciever.C)

#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#define XOR 15 //Value to XOR Read-byte by
#define Offset 17 //Offest to offset value in ascii table

int main(void) {

	FILE * fptr, *fptr2;
	const char* Read_Name = "Crypt.txt";    //Name of file to be encrypted
	const char* Write_Name = "Crypted.txt"; //Name of encrypted file
	char ch;


	fptr = fopen(Read_Name, "r"); // Error Handling

	if (fptr == NULL) {
		printf("Error Opening Read File\n\n"); // Error Handling
		return -1;
	}

	fptr2 = fopen(Write_Name, "w"); // Error Handling

	if (fptr2 == NULL) {
		printf("Error Opening Write File\n\n"); // Error Handling
		return -1;
	}


	while ((ch = fgetc(fptr)) != EOF) { //Read and encrypt

		ch = (ch + Offset) ^ (XOR);
		fputc(ch, fptr2);              //Send to output file
	}


	fclose(fptr); //Close and free files
	fclose(fptr2);

	printf("Encryption Successful\n\n");

	return 0;

}
