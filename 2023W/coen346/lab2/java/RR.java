import java.util.List;

public class RR implements Algorithm {

    List<Task> q = null;
    int next = 0;
    int slice = 10;
    int totalBurst = 0;
    int totalBurstCalc = 0;

    public RR(List<Task> queue) {
        q = queue;
    }

    @Override
    public void schedule() {
        System.out.println("\n-----Round Robin Scheduling-----\n");

        // for-loop counting total amount of burst of all tasks
        for (int i = 0; i < q.size(); i++){
            totalBurst = totalBurst + q.get(i).getBurst();
            totalBurstCalc = totalBurst;        // used to calculate waiting time (WT = TAT - BT)
        }

        // while-loop until no more burst
        while (totalBurst > 0){
            CPU.run(q.get(next), slice);      // run with slice of 10
            totalBurst = totalBurst - q.get(next).getBurst();      // decrease totalBurst Counter
            pickNextTask(); // go to next task
        }
    }

    @Override
    public Task pickNextTask() {
        if (next == q.size()-1){
            next = 0;   // 'reset' to first index
        } else{
            next += 1;  // increase index
        }
        return null;
    }

    // turnaround - burst
    public void waitingTime(){
        float totalTurnAround = 0;  // total turnaround time
        float totalWaitTime = 0;    // total wait time
        float avgWaitTime = 0;      // average wait time
        float arrivalTime = 0;      // all tasks arrived at time 0

        for (int i = 0; i < q.size(); i++){

            totalTurnAround = CPU.getFinishTime(q.get(i)) - arrivalTime;  // total turnaround time (TAT = CT - AT)
        }
        totalWaitTime = totalTurnAround - totalBurstCalc;    // total waiting time (WT = TAT - BT)

        avgWaitTime = totalWaitTime/q.size();   // divide total wait time to number of task
        System.out.println("The average waiting time is: " + avgWaitTime);  // output

    }

    // exit time - arrival time
    public void avgTurn(){
        float totalTurnAround = 0;  // total turnaround time
        float avgTurnAround = 0;    // average turnaround time
        float arrivalTime = 0;      // all tasks arrived at time 0

        for (int i = 0; i < q.size(); i++){
            totalTurnAround = CPU.getFinishTime(q.get(i)) - arrivalTime;  // total turnaround time (TAT = CT - AT)
        }

        avgTurnAround = totalTurnAround/q.size();       // average turnaround time

        System.out.println("The average turn around time is: " + avgTurnAround);    // output
    }
}
