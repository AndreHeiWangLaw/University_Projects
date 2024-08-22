// Andre Hei Wang Law
// 4017 5600

package ca.concordia.coen346.personcrossingriver;

public class Person implements Runnable {
    private int loc;
    private static int onBoardCounter = 0;
    private static final Object sync = new Object();
    private final int id;

    // constructor
    public Person(int id, int location) {
        this.id = id;       // person id
        this.loc = location;    // 0 for left bank and 1 for right bank
    }

    // Must not return until it safe for the person to cross the river in the given direction
    // (no more than 3 people on board + no one will step off the pier when boat is opposite bank)
    public void ArriveAtBoat(int loc) {
        synchronized(sync) {    // use the lock object to synchronize the code block
            while (onBoardCounter > 2) {    // enter while-loop when there are more than 2 people
                try {
                    System.out.println("Mr." + id + " is waiting for the boat to return at location " + loc);
                    sync.wait();    // wait for the lock to be notified
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            onBoardCounter++;    // increment the number of people on the boat

            System.out.println("\nMr." + id + " is boarding the boat at location " + loc);
            System.out.println("Number of People on board: " + onBoardCounter);
        }
    }

    // returns location where the person gets off the boat
    public int BoardBoatAndCrossRiver(int loc) throws InterruptedException {
        Thread.sleep(100); // simulate crossing the river, 100ms
        return 1 - loc;    // return the opposite location of the current location
    }

    // called when person has finished crossing the river
    public void GetOffOfBoat(int loc) {
        synchronized(sync) {    // use the lock object to synchronize the code block
            onBoardCounter--;    // decrement the number of people on the boat
            sync.notifyAll();   // notify all waiting threads that the lock is available

            System.out.println("\nMr." + id  + " is leaving the boat at location " + loc);
            System.out.println("Number of People on board: " + onBoardCounter);
        }
    }

    public void run() {
        ArriveAtBoat(loc); // call method with the current location
        int newLocation = 0;
        try {
            newLocation = BoardBoatAndCrossRiver(loc); // call method with the current location
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        GetOffOfBoat(newLocation);  // call method with the current location
    }
}
