import ca.concordia.coen346.server.Buffer;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        Buffer buffer = new Buffer();

        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 1000; i++) {
                buffer.produce(i);
            }
        });
        Thread t2 = new Thread(() -> {
           for (int i = 0; i < 1000; i++) {
               buffer.consume();
           }
        });
        t1.start();
        t2.start();
        t1.join();
        t2.join();
        System.out.println("Count " + buffer.readCount());
    }
}