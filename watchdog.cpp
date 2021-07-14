/**
 * \mainpage CmpE 322 Watchdog Project
 * \author Onur Sefa ÖZÇIBIK 2017400222
 * \section idproject Project
 * The main idea of this project is creating a supervisor which checks the subprocesses and maintains them.
 * The control mechanism is sustained by the fork(), execl(), wait(), sleep() functions and inter process communication called named pipe
 * While the executor process runs the instructions, processes obeys the rules and watchdog keeps track of well-being of processes. Also, watchdog creates those processes and kills if it is necessary.
 *
 * \subsection idbonus bonus case
 * There is bonus case to handle the situation when the watchdog process dies abnormally or before the all other processes. So, in this case, watchdog before exiting must be sure about it's childs are dead. So, in the SIGTERM handler, it sends SIGTERM to the all processes that are currently not dead.
 *
 * \section idwatch watchdog.cpp
 * watchdog.cpp - This is the process creates a write pipe end and sends it's process id via this named pipe to the executor.
 * Afterwards, it creates child processes and transform them as process processes via fork() and execl() functions. In the infinite loop, waits a child to die. It checks whether the dead process is head process or not. With this information, whether it kills other all processes and creates another processes with different pids but same process names or reopens the died child.
 * When it receives SIGTERM, waits all the process processes to die and also kills them if it is necessary as stated in the bonus case.
 *
 * \section idprocess process.cpp
 * process.cpp - This process is the working process to do the "job" and created by watchdog. It receives signals from executor and reacts accordingly by writing to the ouput file.
 * When created, it sleeps in infinite loop to wait a signal. If the signal is SIGTERM, it dies gracefully.
 *
 *
 */

#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <csignal>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <map>


using namespace std;
/**
 * The amount of waiting time specified as 0.3 seconds
 */
struct timespec delta = {0 /*secs*/, 300000000 /*nanosecs*/}; //0.3 sec
/**
 * The filestream to write the operations that watchdog does
 */
ofstream file;
/**
 * Named pipe id which provides the communication between watchdog and executor
 */
int unnamedPipe;
/**
 * The pointer that points the pid id list to ensure accessibility from other functions than the main
 */
int * listPointer;
/**
 * The number of processes wanted to be run as process
 */
int n;

/**
 * Signal handler to terminate watchdog. Firstly, it ensure to all processes are dead.
 * After that, it waits all process processes to be dead. After it exits gracefully.
 * @param sig the integer value of the signal SIGTERM
 */
void termHendler(int sig){
    file << "Watchdog is terminating gracefully" ;
    file.close();
    for(int i=1; i<=n; i++){
        kill(listPointer[i], SIGTERM);
        nanosleep(&delta,&delta);
    }
    while(wait(NULL)>0){

    }
    close(unnamedPipe);
    exit(0);
}

/**
 * The main function that handles the creation of child processes and sustains them.
 * When created, it does necessary file operations and writes its id to the named pipe.
 * It waits a children termination, accordingly the of the dead process, it checks if it is head process. Accordingly, it kills and forks all child processes or the dead process only.
 * @param argc The integer value of the numbers of arguments taken by the watchdog
 * @param argv The argument array, which should be consist of the process name, number of processes to be run, process output directory, and watchdog output directory
 * @return the Return value is 0 in normal execution.
 */
int main(int argc, char* argv[]){
    signal(SIGTERM, termHendler);

    n = atoi(argv[1]);
    string watchdogPath = argv[3];
    
    int pidList[n+1];
    listPointer = pidList;

    pid_t pid;
    pid_t head;
    file.open(argv[2], ofstream::out|ofstream::trunc);
    file.close();
    file.open(watchdogPath);

    char * myfifo = (char*) "/tmp/myfifo";
    string s = "P0 " + to_string((int)getpid());
    char temp[30];
    strcpy(temp, s.c_str());
    unnamedPipe = open(myfifo, O_WRONLY);

    write(unnamedPipe, temp, 30);

    for(int i = 1; i <= n; i++){
        if(!(pid = fork())){
            s = "P" + to_string(i);
            strcpy(temp, s.c_str());
            execl("./process","process", argv[2], temp ,NULL);
            break;
        }
        else{
            file << "P" << i << " is started and it has a pid of " << pid << "\n" ;
            nanosleep(&delta, &delta);  // Deal with writing delays
            pidList[i] = pid;
            s = "P" + to_string(i) + " " + to_string(pid);
            strcpy(temp, s.c_str());
            write(unnamedPipe, temp, 30);
            if(i==1){
                head = pid;
            }
        }

    }



    while(1){
        int w = wait(NULL);
        if(w==head){
            file << "P1 is killed, all processes must be killed" << "\n" ;
            
            for(int i=2; i<=n; i++){
                kill(pidList[i], SIGTERM);
                nanosleep(&delta, &delta);  // Deal with writing delays
            }
            
            file << "Restarting all processes" << "\n";
            for(int i = 1; i <= n; i++){
                if(!(pid = fork())){
                    s = "P" + to_string(i);
                    strcpy(temp, s.c_str());
                    execl("./process","process", argv[2], temp ,NULL);
                    break;
                }
                else{
                    file << "P" << i << " is started and it has a pid of " << pid << "\n" ;
                    nanosleep(&delta, &delta);  // Deal with writing delays
                    pidList[i] = pid;
                    s = "P" + to_string(i) + " " + to_string(pid);
                    strcpy(temp, s.c_str());
                    write(unnamedPipe, temp, 30);
                    if(i==1){
                        head = pid;
                    }
                }

            }
        }else{
            for(int i=1; i<=n; i++){
                if(pidList[i]==w){
                    if(!(pid = fork())){
                        s = "P" + to_string(i);
                        strcpy(temp, s.c_str());
                        execl("./process","process", argv[2], temp ,NULL);
                        break;
                    }
                    else{
                        file << "P" << i << " is killed\nRestarting P" << i << "\n";
                        file << "P" << i << " is started and it has a pid of " << pid << "\n" ;
                        pidList[i] = pid;
                        s = "P" + to_string(i) + " " + to_string(pid);
                        strcpy(temp, s.c_str());
                        write(unnamedPipe, temp, 30);
                        if(i==1){
                            head = pid;
                        }
                    }
                }
            }
        }
    }



    close(unnamedPipe);

    return 0;
}
