#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>

using namespace std;

/**
 * The name of the process given by the watchdog process
 */
string process;
/**
 * The directory of the output file
 */
string directory;
/**
 * Filestream object to write done operations by the process
 */
fstream file;
/**
 * Signal handler function to react SIGINT signal
 * @param sig The integer value of the given signal
 */
void intHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " received signal " << sig << "\n";
    file.close();
}
/**
 * Signal handler function to react SIGHUP signal
 * @param sig The integer value of the given signal
 */
void hupHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " received signal " << sig << "\n";
    file.close();
}
/**
 * Signal handler function to react SIGILL signal
 * @param sig The integer value of the given signal
 */
void illHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " received signal " << sig << "\n";
    file.close();
}
/**
 * Signal handler function to react SIGTRAP signal
 * @param sig The integer value of the given signal
 */
void trapHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " received signal " << sig << "\n";
    file.close();
}
/**
 * Signal handler function to react SIGFPE signal
 * @param sig The integer value of the given signal
 */
void fpeHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " received signal " << sig << "\n";
    file.close();
}
/**
 * Signal handler function to react SIGSEGV signal
 * @param sig The integer value of the given signal
 */
void segvHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " received signal " << sig << "\n";
    file.close();
}
/**
 * Signal handler function to react SIGTERM signal
 * Afterwards, it exits() with the 15 code
 * @param sig The integer value of the given signal
 */
void termHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " received signal " << sig <<", terminating gracefully" << "\n";
    file.close();
    exit(sig);
}
/**
 * Signal handler function to react SIGXCPU signal
 * @param sig The integer value of the given signal
 */
void xcpuHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " received signal " << sig << "\n";
    file.close();
}
/**
 * The main function that operates the process objectives. When started and remarked it in the output file it sleeps and waits any signal that will be handled later on.
 * @param argc The number of arguments
 * @param argv Argument array which consists of process, the output file directory, and the name of the process
 * @return Returns 0 in normal termination.
 */
int main(int argc, char* argv[]){
    process = argv[2];
    directory = argv[1];
    file.open(directory, std::ios_base::app);

    file << argv[2] << " is waiting for a signal" << "\n";
    file.close();

    signal(SIGINT, intHandler);
    signal(SIGHUP, hupHandler);
    signal(SIGILL, illHandler);
    signal(SIGTRAP, trapHandler);
    signal(SIGFPE, fpeHandler);
    signal(SIGSEGV, segvHandler);
    signal(SIGTERM, termHandler);
    signal(SIGXCPU, xcpuHandler);
    

    while(1){
        sleep(1);
    }



    return 0;
    
}