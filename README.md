Dining Philosophers Problem

Overview

The Dining Philosophers Problem is a classic synchronization challenge in computer science, illustrating how processes share limited resources without conflicts. Philosophers seated around a table need forks to eat, but only one fork is placed between each pair. Solving this problem requires preventing deadlocks, avoiding starvation, and managing concurrency efficiently.

This project is part of the 1337 School Curriculum, where I implemented the problem in C using two distinct synchronization methods.

Project Goals

Understand and implement synchronization primitives.

Learn how to avoid deadlocks, race conditions, and resource starvation.

Explore different concurrency models using threads and processes.

Implementation Approaches

1. Threads and Mutexes

Threads: Each philosopher is represented by a thread.

Mutexes: Forks are protected by mutexes to ensure mutual exclusion.

Key Steps:

Initialization:

Parse input arguments and validate them.

Initialize mutexes for forks and shared resources.

Thread Creation:

Create a thread for each philosopher.

Assign threads unique IDs and relevant fork references.

Philosopher Routine:

Philosophers perform actions: take forks, eat, sleep, and think.

Use mutex locks to ensure mutual exclusion while accessing forks.

Monitoring:

Monitor philosopher states for possible starvation or death.

Terminate simulation upon completion.

Resource Cleanup:

Destroy all mutexes and free allocated memory.

2. Processes and Semaphores (Bonus Implementation)

Processes: Each philosopher runs as a separate process.

Semaphores: Semaphore variables manage forks and avoid deadlocks.

Key Steps:

Initialization:

Initialize semaphores for forks and shared data protection.

Process Creation:

Create a process for each philosopher using fork().

Philosopher Routine:

Each process runs the philosopher's lifecycle independently.

Use semaphores to coordinate access to forks.

Monitoring and Cleanup:

Monitor processes for termination signals.

Ensure proper resource cleanup upon process termination.

Key Features

Deadlock Avoidance: Resource allocation strategies ensure philosophers don’t wait indefinitely.

Fairness: Every philosopher gets equal chances to eat.

Clean Resource Management: Proper cleanup of threads, processes, mutexes, and semaphores.

What I Learned

Thread vs. Process Management: Creating and managing concurrent execution flows.

Mutexes vs. Semaphores: Choosing appropriate synchronization primitives based on system constraints.

Deadlock Prevention: Using strategies like resource ordering and mutex hierarchy.

Problem-Solving Skills: Decomposing complex challenges into manageable tasks.

How to Run the Project

Prerequisites

GCC Compiler

POSIX Thread Library (for the threads version)

Compilation & Execution

Clone the repository:

git clone https://github.com/hamzamraizik/philosophers.git
cd philosophers

Compile the project:

make

Run the program:

./philosophers <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]

Clean up:

make clean

Project Structure

src/: Contains the implementation files.

includes/: Header files.

Makefile: Automates build and clean processes.

Resources

GitHub Repository

Medium Article - Full Explanation

Feel free to explore, learn, and contribute!

