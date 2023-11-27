#include "types.h"
#include "user.h"
#include "param.h"

#define MAP_SIZE 3

enum proc_types{
    CPUBOUND,
    SCPU,
    IOBOUND
};

struct IntCharMap {
    int key;
    const char* value;
};

struct IntCharMap myMap[MAP_SIZE] = {
        {CPUBOUND, "CPU-Bound"},
        {SCPU, "S-CPU"},
        {IOBOUND, "IO-Bound"}
};

void
cpu_bound(){
    for(int i = 0; i < 100; i++){
		for(int j = 0; j < 1000000; j++){
			asm("nop");
		}
	}
        
}

void
s_cpu(){
    
    for(int i = 0; i < 100; i++){
		asm("nop");
        for(int j = 0; j < 1000000; j++){
			double x = i * j;
			x = x;
			if (j % 10000 == 0)
				yield();
        }
        
    }

}

void
io_bound(){
    for(int i = 0; i < 100; i++){
        sleep(1);
    }
}

void
print_top() {
    printf(0,  "PID   |   Process Type   |   Waiting(time)   |   Running(time)   |   Sleeping(time)   |   TurnAround(time)\n");
    printf(0,  "------|------------------|-------------------|-------------------|--------------------|---------------------\n");
}

void
print_process(int N, int param, int procs[N][param]){
	for(int i = 0; i < N; i ++){
		printf(0,  "%d | %s | %d | %d | %d | %d\n", procs[i][0], myMap[procs[i][0]%NUMQUEUES].value, procs[i][1], procs[i][2], procs[i][3], procs[i][4]);
	}
    	
}

void
print_floor(int width) {
    for (int i = 0; i < width; i++) {
        printf(0,  "-");
    }
    printf(0,  "\n");
}

void
calc_means(int N, int procs[N][5], int means[3][3]){
    int mod;
    for(int i = 0; i < N; i++){
        mod = i%NUMPROCSTYPES;
        means[mod][0] += procs[i][1];
        means[mod][1] += procs[i][2];
        means[mod][2] += procs[i][3];
    }
    int div = (N/NUMPROCSTYPES);
    for(int i = 0; i < NUMPROCSTYPES; i++){
        means[i][0] /= div;
        means[i][1] /= div;
        means[i][2] /= div;
    }
}

void
initialize(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 0;
        }
    }
}

void
print_means(int means[3][3]) {
    
    printf(1, "\n\nCPU bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", means[0][0], means[0][1], means[0][2], (means[0][0] + means[0][1] + means[0][2]));
	printf(1, "CPU-S bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", means[1][0], means[1][1], means[1][2], (means[1][0] + means[1][1] + means[1][2]));
	printf(1, "I/O bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", means[2][0], means[2][1], means[2][2], (means[2][0] + means[2][1] + means[2][2]));
}

void
get_procs(int N, int param, int procs[N][param]){
	for (int i = 0; i < N; i++) {
        int retime, rutime, stime;
		int pid = wait2(&retime, &rutime, &stime);
		procs[i][0] = pid;
		procs[i][1] = retime;
		procs[i][2] = rutime;
		procs[i][3] = stime;
		procs[i][4] = retime + rutime + stime;
	}
}

void print_results(int n){
	int N = n * NUMPROCSTYPES;

	int means[NUMPROCSTYPES][NUMPROCSTYPES];
	initialize(NUMPROCSTYPES, NUMPROCSTYPES, means);

	int procs[N][5];
	get_procs(N, 5, procs);
	print_top();
	print_process(N, 5, procs);
	print_floor(110);

	calc_means(N, procs, means);
	print_means(means);

}
void
set_procs(int (*procs)[5], int data[5], int line) {
    for (int i = 0; i < 5; i++) {
        procs[line][i] = data[i];
    }
}

int char2int(char* num){
    int result = 0;
    int i = 0;

    while (num[i] != '\0') {
        result = result * 10 + (num[i] - '0');
        i++;
    }

    return result;
}

int 
main(int argc, char *argv[]){
    if(argc < 2){
        printf(0,  "Error: Missing argument in program call.");
        printf(0,  "Usage: sanity <int>");
        exit();
    }

    int n = char2int(argv[1]);

    int pid;
    for (int i = 0; i < NUMPROCSTYPES * n; i++){
        pid = fork();
        
        if(pid != 0){
            continue;
        } 
        int cpid = getpid();
        switch (cpid % NUMPROCSTYPES)
        {
            case CPUBOUND:
                cpu_bound();
                break;
            case SCPU:
                s_cpu();
                break;
            case IOBOUND:
                io_bound();
                break;
            default:
                break;
        }
		exit();
		continue;
    }
	

	print_results(n);
    exit();
    exit();
}