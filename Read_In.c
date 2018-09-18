#include <stdio.h>



int main(void) {

	int x;



	printf("Put in Int\n");
	
	if (1 != scanf("%d", &x)){

		printf("Wrong \n\n");
		return -1;
	}

	else printf("Is %d, Good Work \n\n",x);



	return 0;
}