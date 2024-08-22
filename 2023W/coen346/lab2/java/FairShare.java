import java.util.*;

public class FairShare implements Algorithm {

    List<Task> q = null;
    int slice = 10;
    int next = 0;
    int totalBurst = 0;
    int totalBurstCalc = 0;
    int divider = 1;

    public FairShare(List<Task> queue) {
        q = queue;
    }

    @Override
    public void schedule() {
        System.out.println("\n-----Fair Share Scheduling-----\n");
        Collections.sort(q, Comparator.comparing(Task::getPriority)); // sort priority ascending order
        List<Integer> ls = new ArrayList<Integer>();
        List<Integer> dup = new ArrayList<Integer>();

        // for-loop goes through all tasks
        for (int i = 0; i < q.size(); i++){
            // check if array already contains the element
            if(!ls.contains(q.get(next).getPriority())) {
                ls.add(q.get(next).getPriority());  // add unique priority/id element into array
                dup.add(0);
            }else{
                dup.add(q.get(next).getPriority());  // add duplicate priority/id element into array
                divider += 1;
            }
        }
        // for-loop counting total amount of burst of all tasks
        for (int i = 0; i < q.size(); i++){
            totalBurst = totalBurst + q.get(i).getBurst();
            totalBurstCalc = totalBurst;        // used to calculate waiting time (WT = TAT - BT)
        }
        // while-loop until no more burst
        while (totalBurst > 0){
            int fairShareSlice = 0;
            if (dup.get(next) == 0){
                divider = 1;       // set divider to 1
            }
            if (ls.size() == 1){
                fairShareSlice = slice/q.size();   // divide by 1, fairShareSlice doesn't change
            } else{
                fairShareSlice = slice/ls.size();   // give quantum slice equally depending on how many user id (ls.size())
            }
            if(dup.contains(q.get(next).getPriority())) {
                fairShareSlice = fairShareSlice/divider;   // give quantum slice equally depending on how many user id (ls.size())
            }
            CPU.run(q.get(next), fairShareSlice);      // run with slice of 10
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
