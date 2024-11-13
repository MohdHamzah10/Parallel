#include <stdio.h>
#include <omp.h>

int IsPerfect(int num){
	int sum=0;
	for(int i=1;i<=num/2;i++){
		if(num%i==0){
			sum+=i;
		}
	}
	return sum==num;
}
int main(){
	int num[20],i;
	printf("enter 20 numbers: \n");
	for(i=0;i<20;i++){
		scanf("%d",&num[i]);
	}
	#pragma omp parallel for
	for(i=0;i<20;i++){
		if(IsPerfect(num[i])){
			printf("%d is a perfect number\n",num[i]);
		}
		else{
			printf("%d is not a perfect number\n",num[i]);
		}
	}
	return 0;
}
