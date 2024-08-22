package ca.concordia.processmanagement;

public interface Process {
    void createMap() throws Exception;      // create the map
    int createProcess() throws Exception;       // create a process by creating its process control block
    void terminateProcess(int pid) throws Exception;        // end process of specified pid
}
