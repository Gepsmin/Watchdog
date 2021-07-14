# Watchdog Project 

### Description

The project is to be designed in a flexible manner. That means, you will have N processes
and a watchdog, but those N processes will be created by running the same executable file,
process.c/process.cpp. During the evaluation, we may take N to be 3 or maybe even 120. So,
do not develop your code depending on a specific value of N.  
You are expected to have three C/C++ programs; process, watchdog, and executor with the
properties specified below. Note that, executor program is provided to you, and you should not
alter anything on the original program

### executor.cpp

1. File Properties:
• The name of the file is executor:cpp. (This file is already provided to you. Do not
modify.)
2. Operations:
• Program reads instructions from instructions.txt.
• Creates a named pipe to be shared with watchdog program.
• Reads "\# \<PID\>" tuples from the read end of the pipe (P1 3564, P2 3565 etc.).
• Detects whether it is a signal or wait command.
• Sends the intended signal to the given process.
• When instructions are completed, first it kills the watchdog process, then all of the
other running processes by sending the SIGTERM signal.

### process.cpp

1. File Properties:
• The name of the file must be process:c=process:cpp.
2. Process definition:
• Assume you are asked to create a system with N processes. Name the processes as
P 1; P 2; P 3; P 4; ... ; PN.
• All processes should work concurrently.
• Process P 1 should be considered as the head process (not parent).
• For the sake of the scenario, we assume the head is such a critical process that, if it
fails, all other processes should be terminated, P 1 be re-created, and then all remaining
processes P 2; P 3; ..., and PN should be created following P 1.
3. Operations:
• Each process is forked from the watchdog process (which is described in the following
sections).
• Each process should display a message (as explained below) indicating that it has
started and then go to sleep until it receives a signal.
• Each process must handle the signals given in Table 1.
• Processes handle signals by only printing the value of the corresponding signal.
• Specific to SIGTERM signal, the process should terminate after printing the required
message.
• The instructions file has the signals and wait commands listed line by line. A sample
instruction file is shown in Table 2. For instance, if the instruction is "SIGHUP P3",
P3 receives a SIGHUP signal. If the instruction is "wait 1", all the processes should
sleep for 1 second.

![table1](https://user-images.githubusercontent.com/12373950/125690664-1da8cfcf-fca4-4f15-9866-b69649402c8a.PNG)

### watchdog.cpp

1. File properties:
• The name of the file must be watchdog:c=watchdog:cpp.
• Main method should be in the watchdog file.
2. Operations:
• The watchdog program opens the named pipe.
• The watchdog process writes its ID to the named pipe as P0 <PID>.
• The watchdog should be the parent process, so each process should be created using
fork() from the watchdog.
• The watchdog process writes the ID of each newly forked processes to the named pipe
as P# <PID>.
• The watchdog process should initiate each process from the process executable using
exec().
• If num of process = 10, your program should create 10 distinct processes (in addition
to the watchdog)..
• The number of processes in the program should be preserved, and be equal to the
num of process all the time.
• Watchdog sleeps until a process terminates.
• If a process terminates, watchdog should detect and restart it. Do not forget to send
"P# <PID>" tuple for the restarted process since the PID value has changed.
• If the head process receives SIGTERM signal, all of the other processes should also
be killed and restarted by the watchdog. The processes should be terminated in
increasing order of the process numbers and this order should be visible in the output.
3• Watchdog kills processes via the SIGTERM signal.
• The whole project should be terminated in a safe state. That is, executor kills all the
child processes and the watchdog when it reaches the end of instructions.txt file.
• Bonus Case: What happens if the watchdog dies? (You do not have to handle this
case.)

![table2](https://user-images.githubusercontent.com/12373950/125690780-a789328c-c01e-4d85-9038-7ad13742f1c1.PNG)

![table3-4](https://user-images.githubusercontent.com/12373950/125690816-7bf787de-49e9-4115-9bf1-30b47c05e407.PNG)

### Input-Output

```
gcc process.c -o process
gcc watchdog.c -o watchdog
./watchdog 10 process_output watchdog_output
```