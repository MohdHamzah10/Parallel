#include <stdio.h>
#include <omp.h>
int main(){
	omp_set_num_threads(4);
	int i,tid;
	#pragma omp parallel private(tid)
	{
	tid=omp_get_thread_num();
	for(i=0;i<100;i++){
		printf("%d: Hello World\n",tid);
	}
	}
	return 0;
}
