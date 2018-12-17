//This program is designed to do a simple decryption algorithim on a text file (Need Encryptor.C)

#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#define XOR 15 //Value to XOR Read-byte by
#define Offset 17 //Offest to offset value in ascii table

int main(void){

	char ch;
	FILE * fptr, *fptr2;
	const char* Read_Name = "Crypted.txt"; //Name of file to decrypt
	const char* Write_Name = "DeCrypt.txt"; //Name of file to write to


	fptr = fopen(Read_Name, "r"); // Error handling

	if (fptr == NULL) {
	printf("Error Opening Read File\n\n"); // Error handling
	return -1;
	}

	fptr2 = fopen(Write_Name, "w"); // Error handling

	if (fptr2 == NULL) {
	printf("Error Opening Write File\n\n"); // Error handling
	return -1;
	}


	while ((ch = fgetc(fptr)) != EOF) { //Read and decrypt

	ch = ((ch ^ XOR) - Offset);
	fputc(ch, fptr2);               //Send to output file
	}


	fclose(fptr); //Close and free files
	fclose(fptr2);

	printf("Decryption Successful\n\n");

	return 0;

}