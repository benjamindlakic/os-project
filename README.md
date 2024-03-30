# OS

Members:  
          
          Benjamin Dlakic
          Tarik Karahodzic

Files:     
                    
          shell.c
          system_calls.c
          gists.txt (test file)
          
          
Answers:  
          
          Q1 -  If we have a single-core, uniprocessor system that supports multiprogramming, how many processes can be in a running state in such a system, at any given time?

            - Only one process will be in a running state, despite that the system supports multiprogramming. Why is that? It’s because a processor can execute only one process at a time on a single core. Other processes can be in a ready or waiting state, waiting for their turn to be executed.


          Q2 -  Explain why system calls are needed for a shared memory method of inter-process communication (IPC). If there are multiple threads in one process, are the system calls needed for sharing memory between those threads?
            
            - IPC refers to the mechanism and techniques used by processes in a computer system to communicate and share data with each other. System calls are needed for sharing memory IPC because they act as the go-between for processes. Even though we said that system calls are needed for sharing memory, they are not required for threads within a process to share memory.

          Q3 -  Consider the following sample code from a simple shell program. Now, suppose the shell wishes to redirect the output of the command not to STDOUT but to a file “foo.txt”. Show how you would modify the above code to achieve this output redirection.

            command = read_from_user();
            int fd = open("foo. txt", O_WRONLY | O_CREAT, 0644);
            if (fd == -1) {
                  perror ("open");
                  exit(1);
            ｝

            int dup_result = dup2(fd, STDOUT_FILENO);
            if(dup_result == -1) {
                  perror ("dup?");
                  exit(1);
            ｝
            close (fd)

            int rc = fork();
            if(rc == 0) {
                  exec (command)
            } else {
                  wait();
            close (STDOUT_FILENO);

Outline:       

          For this assignment we created a shell program with wc, grep, df, cmatrix, ls, pwd, date functionalities, We also have a separate C code that implements system calls fork(), wait(), exec().

          We were exploring the concept of system calls in C programming. System calls allow programs to interact with the operating system and perform various tasks such as creating processes, executing other programs, and waiting for process completion.

Instructions:   

          Firstly we need to save the shell.c file on a desired location. After opening terminal and changing directory to the
          location where the file is stored, we can compile it using the following command:
                gcc -Wall -o shell shell.c 
          After the program is compiled, we can run it using the command:
                ./shell

          The same process applies for any other file. 
                  gcc -Wall -o system_call system_call.c

                  ./system_call
            

Challenges:     

          We faced a challenge while implementing all the functions to shell. The biggest one was CMATRIX and it was really hard for us to understand how to do it, why is it a function and how to actually implement it. Lack of resources was really disappointing.
          In the end, we did many functions, had difficult time with it, but with some research it was done.

Sources:        
              
          Stack Overflow
          geeksforgeeks.org
          W3schools
          Medium
          github
          brennan.io

