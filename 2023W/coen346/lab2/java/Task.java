/**
 * Task to be scheduled by the scheduling alogrithm.
 *
 * Each task is represented by
 *
 *  String name - a task name, not necessarily unique
 *
 *  int tid - unique task identifier
 *
 *  int priority - the relative priority of a task where a higher number indicates
 *  higher relative priority.
 *
 *  int burst - the CPU burst of this task
 */

import java.util.concurrent.atomic.AtomicInteger;

public class Task
{
    // the representation of each task
    private String name;
    private int tid;
    private int priority;
    private int burst;
    private int remainBurst;

    /**
     * We use an atomic integer to assign each task a unique task id.
     */
    private static AtomicInteger tidAllocator = new AtomicInteger();

    public Task(String name, int priority, int burst) {
        this.name = name;
        this.priority = priority;
        this.burst = burst;
        this.remainBurst = burst;

        this.tid = tidAllocator.getAndIncrement();
    }

    /**
     * Appropriate getters
     */
    public String getName() {
        return name;
    }

    public int getTid() {
        return tid;
    }

    public int getPriority() {
        return priority;
    }

    public int getBurst() {
        return burst;
    }
    public int getRemainBurst() {
        return remainBurst;
    }

    /**
     * Appropriate setters
     */
    public int setPriority(int priority) {
        this.priority = priority;

        return priority;
    }
    
    public int setBurst(int burst) {
        this.burst = burst;

        return burst;
    }

    public int setRemainBurst(int remainBurst) {
        this.remainBurst = remainBurst;

        return remainBurst;
    }

    /**
     * We override equals() so we can use a
     * Task object in Java collection classes.
     */
    public boolean equals(Object other) {
        if (other == this)
            return true;

        if (!(other instanceof Task))
            return false;

        /**
         * Otherwise we are dealing with another Task.
         * two tasks are equal if they have the same tid.
         */
        Task rhs = (Task)other;
        return (this.tid == rhs.tid) ? true : false;
    }

    public String toString() {
        return
            "Name: " + name + "\n" + 
            "Tid: " + tid + "\n" + 
            "Priority or User ID: " + priority + "\n" +
            "Burst: " + burst + "\n"+
            "Remaining Burst: " + remainBurst + "\n";
    }

    public boolean isComplete() {
        if (getRemainBurst() > 0){
            return false;       // task no completed
        }
        return true;        // task completed
    }
}
