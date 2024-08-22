package ca.concordia.coen346.server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ProcessServer {

    private static OSSimulator simulator;
    public static void main(String[] args){
        simulator = new OSSimulator();
        simulator.start();
        try {
            ServerSocket socket = new ServerSocket(8000);

            while(true) {
                System.out.println("Server waiting for connections");
                Socket client = socket.accept();
                if (simulator.createProcess(client) == -1) {
                    client.sendUrgentData(-1);
                    client.close();
                }
            }

        } catch (IOException e) {
            throw new RuntimeException(e);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
