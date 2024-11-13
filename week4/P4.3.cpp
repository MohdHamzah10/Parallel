#include <stdio.h>
#include <omp.h>
int main(){
	#pragma omp parallel
	{
		for(int i=0;i<10;i++){
			printf("%d from %d\n",i,omp_get_thread_num());
		}
	}
	return 0;
}
