#include <stdio.h>
#include <omp.h>
int main(){
	omp_set_num_threads(4);
	int i;
	#pragma omp parallel for
	for(i=0;i<100;i++){
		printf("%d: Hello World\n",omp_get_thread_num());
	}
	return 0;
}
