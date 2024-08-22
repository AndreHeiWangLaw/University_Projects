package ca.concordia.processmanagement;

public class ProcessControlBlock {
    public int PID;     // pid process
    public boolean processStatus;   // status of availability(false) or "in use" (true)

    public ProcessControlBlock(int pid, boolean status) {
        PID = pid;
        processStatus = status;
    }
}
