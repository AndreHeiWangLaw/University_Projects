import ca.concordia.processmanagement.ProcessManager;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class MainTest extends ProcessManager {
    static final int MIN_PID = 300;     // minimum range of pid value
    static final int MAX_PID = 500;     // maximum range of pid value
    static final int range = MAX_PID-MIN_PID;   // range of pid, 200

    @Test
    void test1() throws Exception {
        ProcessManager pm1 = new ProcessManager();
        pm1.createMap();
        int test1 = pm1.createProcess();
        pm1.terminateProcess(test1);
    }

    @Test
    void test2() throws Exception {
        ProcessManager pm2 = new ProcessManager();
        pm2.createMap();
        for (int i = 0; i <= range; i++){ // i <= range+1, expect an error
            pm2.createProcess();
        }
    }

    @Test
    void test3() throws Exception {
        ProcessManager pm3 = new ProcessManager();
        pm3.createMap();
        for (int i = 0; i <= range; i++){
            pm3.createProcess();
        }
        pm3.terminateProcess(MAX_PID);
        pm3.createProcess();
        //pm3.createProcess();    // uncomment this, we expect an error
    }
}