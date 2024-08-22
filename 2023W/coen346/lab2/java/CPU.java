/**
 * "Virtual" CPU
 *
 * This virtual CPU also maintains system time.
 *
 * @author Greg Gagne - March 2016
 */
 
public class CPU
{
    private static int waitingTime = 0;
    private static int finishTime = 0;


    public static void run(Task task, int slice) {
        int remainingTime = task.getRemainBurst(); // get remaining time for the task
        int quantumTime = slice; // remaining time for the time slice

        // while-loop until no more burst or until no more slice
        while (remainingTime > 0 && quantumTime > 0) {
            remainingTime--;
            quantumTime--;
            waitingTime++;
        }

        task.setRemainBurst(remainingTime); // set remaining burst to remaining time

        task.setBurst(slice-quantumTime);   // set current burst


        if (task.getBurst() == 0){
            // the task is already finished
        } else{
            System.out.println("Will run " + task);

            // check if task is finished
            if (task.isComplete()) {
                System.out.println("-----------------------");
                System.out.println("Task " + task.getName() + " finished.");
                System.out.println("-----------------------\n");
                finishTime = finishTime + waitingTime;
            }
        }
    }

    public static int getFinishTime(Task task) {
        return finishTime;
    }
}
