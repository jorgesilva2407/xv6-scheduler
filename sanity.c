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
    for(int i = 0; i < 100; i++)
        for(int j = 0; j < 1000000; j++);
}

void
s_cpu(){
    
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 1000000; j++){
        }
        yield();
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
    printf(0,  "PID   |   Process Type   |   Ready(time)   |   Running(time)   |   Sleeping(time)\n");
    printf(0,  "------|------------------|-----------------|-------------------|---------------------\n");
}

void
print_process(int (*procs)[5], int max){
    for(int i = 0; i < max; i++)
        printf(0,  "%d | %s | %d | %d | %d\n", procs[i][0], myMap[procs[i][1]].value, procs[i][2], procs[i][3], procs[i][4]);
}

void
print_floor(int width) {
    for (int i = 0; i < width; i++) {
        printf(0,  "-");
    }
    printf(0,  "\n");
}

void
calc_means(int (*procs)[5], int (*means)[3], int max){
    int mod;
    for(int i = 0; i < max; i++){
        mod = i%NUMPROCSTYPES;
        means[mod][0] += procs[i][4];
        means[mod][1] += procs[i][2];
        means[mod][2] += procs[i][2] + procs[i][3] + procs[i][4];
    }
    float div = (max/NUMPROCSTYPES);
    for(int i = 0; i < NUMPROCSTYPES; i++){
        means[i][0] /= div;
        means[i][1] /= div;
        means[i][2] /= div;
    }
}

void
initialize(int matrix[3][3]){
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrix[i][j] = 0.0;
        }
    }
}

void
print_means(int (*procs)[5], int max) {
    int means[3][3];
    initialize(means);
    calc_means(procs, means, max);
    for(int i = 0; i < 3; i++){
        printf(0,  "%s:\n", myMap[i].value); 
        printf(0,  "Sleeping(mean) = %d Ready(mean) = %d Turnaround(mean) = %d\n", means[i][0], means[i][1], means[i][2]);
    }
}

void print_result(int (*procs)[5], int max){
    print_top();
    print_process(procs, max);
    print_floor(60);
    print_means(procs, max);
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
    int procs[NUMPROCSTYPES * n][5];

    int pid;
    int data[5];
    for (int i = 0; i < NUMPROCSTYPES * n; i++){
        pid = fork();
        
        if(pid != 0){
            continue;
        } 
        data[0] = pid;
        switch (pid % NUMPROCSTYPES)
        {
            case CPUBOUND:
                cpu_bound();
                data[1] = CPUBOUND;
                break;
            case SCPU:
                s_cpu();
                data[1] = SCPU;
                break;
            case IOBOUND:
                io_bound();
                data[1] = IOBOUND;
                break;
            default:
                break;
        }
        exit();

        int retime, rutime, stime = 0;
        wait2(&retime, &rutime, &stime);
        data[2] = retime; data[3] = rutime; data[4] = stime;
        set_procs(procs, data, i);
    }

    print_result(procs, NUMPROCSTYPES * n);
    exit();
}