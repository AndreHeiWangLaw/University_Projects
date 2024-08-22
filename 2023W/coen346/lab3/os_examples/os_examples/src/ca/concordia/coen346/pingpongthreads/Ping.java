package ca.concordia.coen346.pingpongthreads;

public class Ping extends Thread{

    public void run(){
        while(true) {
            System.out.println("ping");
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
