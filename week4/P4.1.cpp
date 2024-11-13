#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Fib(int n){
	if(n<=0){
		printf("Invalid. Enter a positive number\n");
		return;
	}
	long long fib[n];
	fib[0]=0;
	if(n>1){
	fib[1]=1;
}
	
	#pragma omp for
	for(int i=2;i<n;i++){
		fib[i]=fib[i-1]+fib[i-2];
}
	printf("The first %d fibonacci numbers are: \n",n);
	for(int i=0;i<n;i++){
		printf("%lld ",fib[i]);
	}
	printf("\n");
}
int main(){
	int n;
	printf("Enter the number of elements in the series: ");
	scanf("%d",&n);
	Fib(n);
	return 0;
	

}
