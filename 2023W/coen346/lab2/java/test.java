import org.testng.annotations.Test;

public class test extends Driver {

    //===========================================================//
    //              Below, uncomment a main to test a            //
    //              scheduler with a specific .txt file.         //
    //===========================================================//

    // First Come First Served Test
    @Test
    void testFCFS() throws Exception {
        main(new String[]{"FCFS", "book.txt"});
//        main(new String[]{"FCFS", "schedule.txt"});
//        main(new String[]{"FCFS", "rr-schedule.txt"});
    }

    // Shortest Job First Test
    @Test
    void testSJF() throws Exception {
//        main(new String[]{"SJF", "book.txt"});
//        main(new String[]{"SJF", "schedule.txt"});
//        main(new String[]{"SJF", "rr-schedule.txt"});
    }

    // Priority Scheduling Test
    @Test
    void testPri() throws Exception {
//        main(new String[]{"PRI", "book.txt"});
//        main(new String[]{"PRI", "schedule.txt"});
//        main(new String[]{"PRI", "rr-schedule.txt"});
    }

    // Round Robin Test
    @Test
    void testRR() throws Exception {
//        main(new String[]{"RR", "book.txt"});
//        main(new String[]{"RR", "schedule.txt"});
//        main(new String[]{"RR", "rr-schedule.txt"});
    }

    // Fair Share Test
    @Test
    void testFS() throws Exception {
//        main(new String[]{"FS", "book.txt"});
//        main(new String[]{"FS", "schedule.txt"});
//        main(new String[]{"FS", "rr-schedule.txt"});
    }
}
