// Andre Hei Wang Law
// 4017 5600

package ca.concordia.coen346.personcrossingriver;

import java.util.ArrayList;
import java.util.List;

public class Main {

    public static void main(String[] args) {

        int totalPeople = 20;   // define the total number of people
        int startingLocation = 1;   // specify the starting location of the people, 0 for left bank and 1 for right bank

        List<Thread> threads = new ArrayList<>();   // create an ArrayList to store Thread objects

        // loop through the total number of people and create Thread objects for each Person
        for (int i = 0; i < totalPeople; i++) {
            Thread t = new Thread(new Person(i, startingLocation));
            threads.add(t);
            t.start();
        }

        // loop through the ArrayList and join each Thread object to the main thread
        for (Thread t : threads) {
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println("Everyone crossed the river");
    }
}