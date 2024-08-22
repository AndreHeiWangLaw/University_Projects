package ca.concordia.coen346.pingpongthreads;

public class Pong extends Thread{

    public void run() {
        while(true) {
            System.out.println("pong");
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
