#include <bits/stdc++.h>
using namespace std;

class process{

    private:

        int pid;
        int duration;
        int runningTime;
        int birthTime;
        bool active;
        bool isCompleted;
        int startTime;
        int completionTime;

    public:
        
        process(int id, int ctime, int btime, bool a = false):
            pid(id), duration(ctime), birthTime(btime), active(a){ isCompleted = false; runningTime = 0; startTime = -1; completionTime=-1;}

        int GetPid(){return pid;}

        int GetDuration(){return duration;}

        int GetBirthTime(){return birthTime;}

        int getCompletionTime(){return completionTime;}

        int getStartTime(){return startTime;}

        void Start(int stime){
            active = true;
            startTime = stime;
        }

        void End(int etime){
            active = false;
            completionTime = etime;
            isCompleted = true;

            // Print brief details about the terminated process;
            printf("process %d has terminated\n", pid);
            printf("Here is the brief about the process\n");
            printf("Time of Birth               : %d\n", birthTime);
            printf("Total Running time required : %d\n", duration);
            printf("Time of initialization      : %d\n", startTime);
            printf("Time of completion          : %d\n", completionTime);
            printf("---------------------------------------\n\n");
        }

        bool CheckIfComplete(){
            if(runningTime >= duration)return true;
            else return false;
        }

        void Run(){
            runningTime += 1;
        }

        int getRunningTime(){
        	return runningTime;
        }

        int getRemainingTime(){
        	return duration - runningTime;
        }
};

int global_time = 0;
double arrival_rate = 0;
double completion_mean_time = 0;
int pid_index = 0;
int total_simulation_time = 0;
int chart_array[1000000];
vector<process*> all_processes;
int levels = 0;
int level_of_execution[1000000];

default_random_engine generator;
uniform_real_distribution<double> distribution(0.0,1.0);

void initializeChartArray(){
	for (int i=0;i<1000000;i++){
		chart_array[i] = -1; // -1 will correspond to no process being scheduled at that point of time (time unit)
		level_of_execution[i] = 0; // By Default all processes will execute at level 0 at any given point of time (time unit)
	}
}

void preprocessProcessDataForGanttChart(){
	// saves required data in "processData.txt"
	// number of levels . . . number of processes executed
	// each continuous process execution with corresponding level of execution
	unordered_set<int> uniquePIDs;
	for(int i=0;i<total_simulation_time;i++){
		if(chart_array[i]!=-1) uniquePIDs.insert(chart_array[i]);
	}
	fstream fileHandler;
	fileHandler.open("processData.txt", ios::out);
	fileHandler << levels << " " << pid_index << " " << total_simulation_time << endl;
	int first=-1, last=-1, level=-1, pid =-1;
	for(int i=0;i<total_simulation_time;i++){
		if(i==0){
			first = i;
			pid = chart_array[i];
			level = level_of_execution[i];
			last = i;
		}
		else{
			if(chart_array[i]==chart_array[i-1] && level_of_execution[i]==level_of_execution[i-1]){
				last = i;
			}
			else{
				if(pid!=-1){
					fileHandler << pid << " " << first << " " << last << " " << level << endl;
				}
				first = i;
				pid = chart_array[i];
				level = level_of_execution[i];
				last = i;
			}
		}
	}
	if(pid!=-1){
		fileHandler << pid << " " << first << " " << last << " " << level << endl;
	}
	fileHandler.close();
}

double generateRandomInterArrivalTime(){
    double random = distribution(generator); //between 0 and 1 (Uniform Distribution)
    double t = log(1-random)/(-1.0*arrival_rate);  //converting to exponential  
    return t;
}

double generateRandomDuration(){
    double random = distribution(generator); //between 0 and 1 (Uniform Distribution)

    double t = log(1-random)*(-1.0*completion_mean_time);  //converting to exponential
    return t;	
}

double getUtilization(){
	int wasteCycles = 0;
	for(int i=0;i<total_simulation_time;i++){
		if(chart_array[i]==-1){
			wasteCycles++;
		}
	}
	return (double)(1.0 - (wasteCycles*1.0)/total_simulation_time);
}

double getAvgTurnaroundTime(){
	double avg = 0;
	double processCount = 0;
	for(int i=0;i<pid_index;i++){
		if(all_processes[i]->getCompletionTime() != -1){
			avg = (avg*processCount + (all_processes[i]->getCompletionTime() - all_processes[i]->GetBirthTime()))/(processCount+1);
			processCount++;
		}
	}
	return avg;
}

double getAvgResponseTime(){
	double avg = 0;
	double processCount = 0;
	for(int i=0;i<pid_index;i++){
		if(all_processes[i]->getStartTime() != -1){
			avg = (avg*processCount + (all_processes[i]->getStartTime() - all_processes[i]->GetBirthTime()))/(processCount+1);
			processCount++;
		}
	}
	return avg;
}

void printPerformanceReport(){
	double utilization = getUtilization();
	double avgTurnaroundTime = getAvgTurnaroundTime();
	double avgResponseTime = getAvgResponseTime();
	cout << "Here is the comprehensive performance report" << endl;
	cout << "Fractional Utilization of CPU : " << utilization << endl;
	cout << "Average Turnaround Time       : " << avgTurnaroundTime << endl;
	cout << "Average Response Time         : " << avgResponseTime << endl;
	cout << "---------------------------------------" << endl << endl;

}

void startSimulation(int typeOfSimulation){
	initializeChartArray();
	double lastProcessBirthTime = generateRandomInterArrivalTime();
	double nextArrivalTime = lastProcessBirthTime;
	int currentExecutingPID = -1; // Initialized with -1 as no execution is taking place;
	if(typeOfSimulation==1){
		//First come first service
		cout << "---------------------------------------" << endl << endl;
		queue<int> pidQueue;
		while(global_time <= total_simulation_time){
			while(global_time==ceil(nextArrivalTime)){ // loop assuming there might be two processes coming at same time
				all_processes.push_back(new process(pid_index, ceil(generateRandomDuration()), global_time));
				pidQueue.push(pid_index);
				pid_index++;
				lastProcessBirthTime = nextArrivalTime;
				nextArrivalTime = nextArrivalTime + generateRandomInterArrivalTime();
			}
			if(currentExecutingPID==-1){
				if(pidQueue.empty()==false){
					currentExecutingPID = pidQueue.front();
					all_processes[currentExecutingPID]->Start(global_time);
					global_time--; // nullify the increment which will be done later, it will be handled in next execution
				}
			}
			else{
				if(all_processes[currentExecutingPID]->CheckIfComplete()){
					all_processes[currentExecutingPID]->End(global_time);
					pidQueue.pop();
					currentExecutingPID = -1;
					global_time--;
				}
				else{
					all_processes[currentExecutingPID]->Run();
					chart_array[global_time] = currentExecutingPID;
				}
			}
			global_time++;
		}
	}
	else if(typeOfSimulation==2){
		// Round Robin
		// Round Robin slice is taken to be same as completion_mean_time (5)
		cout << "Please enter the value of time slice (NOTE : It should be multiple of clock, hence integer)" << endl;
		int timeSliceDuration;
		cin >> timeSliceDuration;
		cout << "---------------------------------------" << endl << endl;
		int timeSliceEnd = timeSliceDuration;
		queue<int> pidQueue;
		while(global_time <= total_simulation_time){
			while(global_time==ceil(nextArrivalTime)){ // loop assuming there might be two processes coming at same time
				int dur = ceil(generateRandomDuration());
				all_processes.push_back(new process(pid_index, dur, global_time));
				pidQueue.push(pid_index);
				pid_index++;
				lastProcessBirthTime = nextArrivalTime;
				nextArrivalTime = nextArrivalTime + generateRandomInterArrivalTime();
			}
			if(currentExecutingPID==-1){
				if(pidQueue.empty()==false){
					currentExecutingPID = pidQueue.front();
					if(all_processes[currentExecutingPID]->getRunningTime()==0) all_processes[currentExecutingPID]->Start(global_time);
					timeSliceEnd = global_time + timeSliceDuration;
					global_time--; // nullify the increment which will be done later, it will be handled in next execution
				}
			}
			else{
				if(all_processes[currentExecutingPID]->CheckIfComplete()){
					all_processes[currentExecutingPID]->End(global_time);
					pidQueue.pop();
					currentExecutingPID = -1;
					global_time--;
				}
				else{
					if(timeSliceEnd==global_time){
						pidQueue.push(pidQueue.front());
						pidQueue.pop();
						currentExecutingPID = -1;
						global_time--;
					}
					else{
						all_processes[currentExecutingPID]->Run();
						chart_array[global_time] = currentExecutingPID;
					}
				}
			}
			global_time++;
		}
	}
	else if(typeOfSimulation==3){
		// Shortest Job first
		cout << "---------------------------------------" << endl << endl;
		priority_queue< pair<int, int>, vector< pair<int, int> >, greater<pair<int, int>> > pidQueue;
		while(global_time <= total_simulation_time){
			while(global_time==ceil(nextArrivalTime)){ // loop assuming there might be two processes coming at same time
				int dur = ceil(generateRandomDuration());
				all_processes.push_back(new process(pid_index, dur, global_time));
				pidQueue.push({dur, pid_index});
				pid_index++;
				lastProcessBirthTime = nextArrivalTime;
				nextArrivalTime = nextArrivalTime + generateRandomInterArrivalTime();
			}
			if(currentExecutingPID==-1){
				if(pidQueue.empty()==false){
					currentExecutingPID = pidQueue.top().second;
					pidQueue.pop();
					all_processes[currentExecutingPID]->Start(global_time);
					global_time--; // nullify the increment which will be done later, it will be handled in next execution
				}
			}
			else{
				if(all_processes[currentExecutingPID]->CheckIfComplete()){
					all_processes[currentExecutingPID]->End(global_time);
					currentExecutingPID = -1;
					global_time--;
				}
				else{
					all_processes[currentExecutingPID]->Run();
					chart_array[global_time] = currentExecutingPID;
				}
			}
			global_time++;
		}
	}
	else if(typeOfSimulation==4){
		// Shortest Remaining Time First
		cout << "---------------------------------------" << endl << endl;
		priority_queue< pair<int, int>, vector< pair<int, int> >, greater<pair<int, int>> > pidQueue;
		while(global_time <= total_simulation_time){
			while(global_time==ceil(nextArrivalTime)){ // loop assuming there might be two processes coming at same time
				if(pidQueue.empty()==false){
					pair<int, int> temp = pidQueue.top();
					pidQueue.pop();
					pidQueue.push({all_processes[temp.second]->getRemainingTime(), temp.second});
				}
				int dur = ceil(generateRandomDuration());
				all_processes.push_back(new process(pid_index, dur, global_time));
				pidQueue.push({dur, pid_index});
				pid_index++;
				currentExecutingPID = pidQueue.top().second;
				lastProcessBirthTime = nextArrivalTime;
				nextArrivalTime = nextArrivalTime + generateRandomInterArrivalTime();
			}
			if(currentExecutingPID==-1){
				if(pidQueue.empty()==false){
					currentExecutingPID = pidQueue.top().second;
					if(all_processes[currentExecutingPID]->getRunningTime()==0) all_processes[currentExecutingPID]->Start(global_time);
					global_time--; // nullify the increment which will be done later, it will be handled in next execution
				}
			}
			else{
				if(all_processes[currentExecutingPID]->CheckIfComplete()){
					all_processes[currentExecutingPID]->End(global_time);
					pidQueue.pop();
					currentExecutingPID = -1;
					global_time--;
				}
				else{
					if(all_processes[currentExecutingPID]->getRunningTime()==0) all_processes[currentExecutingPID]->Start(global_time);
					all_processes[currentExecutingPID]->Run();
					chart_array[global_time] = currentExecutingPID;
				}
			}
			global_time++;
		}
	}
	else{
		// Multilevel Feedback Queue (3 levels)
		cout << "Please enter the value of time slices (NOTE : It should be multiple of clock, hence integer)" << endl;
		levels = 3;
		int timeSliceDuration[levels];
		for(int i=0;i<levels;i++) cin >> timeSliceDuration[i];

		int timeSliceEnd[levels];
		for(int i=0;i<levels;i++) timeSliceEnd[i] = timeSliceDuration[i];

		cout << "Please enter the renewTime after which all lower priority processes will be shifted to higher priority queue" << endl;
		int renewTime;
		cin >> renewTime;
		cout << "---------------------------------------" << endl << endl;

		int nextRenewTime = renewTime;

		queue<int> pidQueue[levels];
		int currentExecutionLevel = -1;
		while(global_time <= total_simulation_time){
			while(global_time==ceil(nextArrivalTime)){ // loop assuming there might be two processes coming at same time
				int dur = ceil(generateRandomDuration());
				// cout << "** " << pid_index << " " << global_time << " " << dur << endl;
				all_processes.push_back(new process(pid_index, dur, global_time));
				pidQueue[0].push(pid_index); // Insert the process in topMost Queue
				pid_index++;
				lastProcessBirthTime = nextArrivalTime;
				nextArrivalTime = nextArrivalTime + generateRandomInterArrivalTime();
			}
			if(global_time==nextRenewTime){
				for(int i=1;i<levels;i++){
					while(pidQueue[i].empty()==false){
						pidQueue[0].push(pidQueue[i].front());
						pidQueue[i].pop();
					}
				}
				currentExecutingPID = -1;
				currentExecutionLevel = -1;
				nextRenewTime = nextRenewTime + renewTime;
			}
			if(currentExecutingPID==-1){
				for(int i=0;i<levels;i++){
					if(pidQueue[i].empty()==false){
						currentExecutingPID = pidQueue[i].front();
						currentExecutionLevel = i;
						if(all_processes[currentExecutingPID]->getRunningTime()==0) all_processes[currentExecutingPID]->Start(global_time);
						timeSliceEnd[i] = global_time + timeSliceDuration[i];
						global_time--; // nullify the increment which will be done later, it will be handled in next execution
						break;
					}
				}
			}
			else{
				if(all_processes[currentExecutingPID]->CheckIfComplete()){
					all_processes[currentExecutingPID]->End(global_time);
					pidQueue[currentExecutionLevel].pop();
					currentExecutingPID = -1;
					currentExecutionLevel = -1;
					global_time--;
				}
				else{
					if(timeSliceEnd[currentExecutionLevel]==global_time){
						int proposedExecutionLevel = (currentExecutionLevel+1<levels)?currentExecutionLevel+1:currentExecutionLevel;
						pidQueue[proposedExecutionLevel].push(pidQueue[currentExecutionLevel].front());
						pidQueue[currentExecutionLevel].pop();
						currentExecutingPID = -1;
						currentExecutionLevel = -1;
						global_time--;
					}
					else{
						all_processes[currentExecutingPID]->Run();
						chart_array[global_time] = currentExecutingPID;
						level_of_execution[global_time] = currentExecutionLevel;
					}
				}
			}
			global_time++;
		}
	}
}

int main(){
	cout << "Are you ready to start simulation? Press 1 if yes, else 0 to terminate" << endl;
	int input;
	cin >> input;
	if(input==1){
		cout << "Please tell us the which simulation are you heading towards" << endl;
		cout << "Press 1 for FIFO" << endl;
		cout << "Press 2 for Round Robin" << endl;
		cout << "Press 3 for SJF" << endl;
		cout << "Press 4 for SRTF" << endl;
		cout << "Press 5 for MLFQ" << endl;
		int simInput;
		cin >> simInput;
		if(simInput >=1 && simInput <=5){
			cout << "Please enter the simulation period" << endl;
			cin >> total_simulation_time;
			cout << "Please tell us the rate of process arrival [Remember : Mean interarrival time will be reciprocal of it]" << endl;
			
			// "NOTE : Please ensure that you enter a number lesser than 1"
			// "for a good simulation otherwise a lot of processes will be there in each time unit cycle"
			
			cin >> arrival_rate;

			cout << "Please tell us the average duration of each process [exponential distribution]" << endl;

			cin >> completion_mean_time;

			startSimulation(simInput);
			preprocessProcessDataForGanttChart();
			printPerformanceReport();
		}
	}
	cout << "Thanks for using our scheduling simulator" << endl;
	return 0;
}