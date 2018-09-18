#include <stdio.h>



int main(void) {

	float x;



	printf("Put in Float\n");
	
	if (1 != scanf("%f", &x)){

		printf("Wrong \n\n");
		return -1;
	}

	else printf("Is %f, Good Work \n\n",x);



	return 0;
}