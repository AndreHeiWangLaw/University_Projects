import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class SJF implements Algorithm {

    List<Task> q = null;
    int next = 0;

    public SJF(List<Task> queue) {
        q = queue;
    }

    @Override
    public void schedule() {
        System.out.println("\n-----SJF Scheduling -----\n");

        Collections.sort(q, Comparator.comparing(Task::getBurst)); // sort burst ascending order

        // for-loop goes through all tasks
        for (int i = 0; i < q.size(); i++){
            CPU.run(q.get(next), q.get(next).getBurst());   // run with slice equal to burst (meaning it runs the entire burst)
            pickNextTask(); // go to next task
        }
    }

    @Override
    public Task pickNextTask() {
        next += 1;  // increase index
        return null;
    }

    // turnaround - burst
    public void waitingTime(){
        float waitTime = 0;         // current wait time
        float turnAround = 0;       // current turnaround time
        float totalWaitTime = 0;    // total wait time
        float avgWaitTime = 0;      // average wait time

        for (int i = 0; i < q.size(); i++){
            turnAround = q.get(i).getBurst() + turnAround;      // turnaround time
            waitTime = turnAround - q.get(i).getBurst();     // wait time = turnaround time - burst
            totalWaitTime = waitTime + totalWaitTime;     // add current wait time to total wait time
        }
        avgWaitTime = totalWaitTime/q.size();   // divide total wait time to number of task
        System.out.println("The average waiting time is: " + avgWaitTime);  // output

    }

    // exit time - arrival time
    public void avgTurn(){
        float turnAround = 0;       // current turnaround time
        float totalTurnAround = 0;  // total turnaround time
        float avgTurnAround = 0;    // average turnaround time
        float arrivalTime = 0;      // all tasks arrived at time 0

        for (int i = 0; i < q.size(); i++){
            turnAround = (q.get(i).getBurst() - arrivalTime)  + turnAround;     // exit time - arrival time + itself
            totalTurnAround = totalTurnAround + turnAround;     // total turnaround time
        }
        avgTurnAround = totalTurnAround/q.size();       // average turnaround time
        System.out.println("The average turn around time is: " + avgTurnAround);    // output
    }
}
