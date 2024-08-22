package ca.concordia.coen346.pingpongthreads;

public class PingPong {

    public static void main(String[] args){
        Ping ping = new Ping();
        Pong pong = new Pong();
        ping.start();
        pong.start();
        try {
            ping.join(5000);
            pong.join(5000);
            if (pong.isAlive()) pong.interrupt();
            if (ping.isAlive()) ping.interrupt();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }


    }
}
