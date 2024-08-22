package ca.concordia.processmanagement;
import java.util.Queue;
import java.util.LinkedList;

public class ProcessManager implements Process{
    Queue<ProcessControlBlock> q = new LinkedList<>();      // ready queue
    public PIDManager pidMan = new PIDManager();        //
    @Override
    public void createMap() throws Exception{
        try{
            pidMan.allocateMap();       // call function allocateMap from PIDManager
        }
        catch (Exception e) {
            throw new Exception("Unable to create Map");   // error message
        }
    }


    @Override
    public int createProcess() throws Exception {
        try {
            int pid = pidMan.allocatePid();     // call function allocatePid from PIDManager
            boolean status = false;     // false = 0, it means "available"
            ProcessControlBlock ctrlBlock = new ProcessControlBlock(pid, status);
            q.add(ctrlBlock);   // add process control block into queue
			System.out.println(pid);
            return pid;
        }
        catch(Exception e){
            throw new Exception("Unable to create the process");   // error message
        }
    }

    @Override
    public void terminateProcess(int pid) throws Exception {
        try {
            pidMan.releasePid(pid);     // call function releasePid from PIDManager
            boolean status = true;      // true = 1, it means "in use"
            ProcessControlBlock ctrlBlock = new ProcessControlBlock(pid, status);
            q.remove(ctrlBlock);    // remove process control block into queue
        }
        catch(Exception e){
            throw new Exception("Process doesn't exist");   // error message
        }
    }
}
