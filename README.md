# OS_Process_Scheduler

The following code is a operating system simulator in which processes are scheduled according to some strategy as per decided by the user and the processes follow *Poisson Process* for arrival, of which, rate is again determined by the user. Duration of each process is predetermined, which comes from the exponential distribution of some rate determined by the user itself. Please NOTE, in practicality such time is not known to the Operating System, but it is required by some of the scheduling strategies theoretically known and being implemented here. The current implementation implements a universal clock in the code itself and every time dependent input output is considered to be the multiple of implemented unit time. This is like one CPU cycle in hardwares. 

### Scheduling Strategies :-
1. First in First Out
2. Round Robin
3. Shortest Job First
4. Shortest Remaining Time First
5. MultiLevel Feedback Queue

## Input :-
Each simulation requires the following information from the user at the start of the execution:-  
1. Total Simulation Period (*int*)
2. Arrival Rate of Processes (per unit time)
3. Mean duration of Processes
4. Time Slices (If the sheduling strategy needs so) (*int*)

## Output :-
Our code prints information about the processes which are completed within the specified period of simulation, which contains start_time, arrival_time, finish_time. At the end of simultion, a comprehensive performance summary is provided which tells user about **fractional utilization of CPU**, **average turnaround time** and **average response time**. Our code also plots the whole scheduling graph (**Gantt Chart**) after the completion of simulation.

## Dependencies :-
Simulator code is in C++14 and the graph plotting is being implemented in JAVA. To plot the Gantt Charts, we use Java library *jfreechart*, for which required jar files are already in the folder 'chartJarFiles'  
To run the code, just execute **simulate.bat** if you are on windows system, else execute **simulate.sh**