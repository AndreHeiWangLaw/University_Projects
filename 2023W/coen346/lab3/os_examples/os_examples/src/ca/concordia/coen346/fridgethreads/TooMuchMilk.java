// Andre Hei Wang Law
// 4017 5600

package ca.concordia.coen346.fridgethreads;

public class TooMuchMilk {

    public static void main(String[] args){
        Fridge f = new Fridge();
        Person p1 = new Person(f, "p1");
        Person p2 = new Person(f, "p2");


        p1.start();
        p2.start();
    }
}
