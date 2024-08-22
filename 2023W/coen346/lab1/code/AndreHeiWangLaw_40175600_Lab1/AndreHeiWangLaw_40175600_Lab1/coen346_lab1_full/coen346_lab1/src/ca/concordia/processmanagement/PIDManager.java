package ca.concordia.processmanagement;

import java.util.BitSet;

public class PIDManager {
    static final int MIN_PID = 300;     // minimum range of pid value
    static final int MAX_PID = 500;     // maximum range of pid value
    static final int range = MAX_PID-MIN_PID;   // range of pid

    BitSet bitSetPid;       // obj using BitSet

    // method 1: allocateMap
    void allocateMap() throws Exception{
        try {
            bitSetPid = new BitSet(range);    // create obj, range from 0-199
            bitSetPid.clear();      // set all bit to false (by default it is already false)
        }
        catch(Exception e) {
            throw new Exception("Unsuccessful in creating a data structure");   // error message
        }
    }

    // method 2: allocatePid
    int allocatePid() throws Exception{
        try {
            int index = bitSetPid.nextClearBit(MIN_PID);      // fnc, get first FALSE bit
            if (index >= MIN_PID && index <= MAX_PID) {
                bitSetPid.flip(index);
                return index;   // return index in which this process id is available
            } throw new Exception("Out of Range");
        }
        catch(Exception e){
            throw new Exception("Action unsuccessful, unable to allocate a pid");   // error message
        }
    }

    // method 3: releasePid
    void releasePid(int pid) throws Exception {
        boolean bit = bitSetPid.get(pid);     // fnc, get bit value of pid

        if(bit) {     // check if pid is indeed in use (true = 1)
            bitSetPid.flip(pid);      // fnc, complement of its current value
        }
        else {
            System.out.println("Action unsuccessful, pid already in released state");   // error message
        }
    }
}
