package ca.concordia.coen346.server;

import java.io.IOException;
import java.net.Socket;

public class OSSimulator extends Thread{

    private final static int MAX_PROCESSES = 2;

    private Process[] clients = new Process[MAX_PROCESSES];

    private Buffer buffer = new Buffer();
    private int numClients = 0;


    private int currClient = 0;

    public OSSimulator (){

    }

    public int createProcess(Socket client) throws Exception {
        if(numClients == MAX_PROCESSES){
            return -1;
        }
        //get id and assign to that position
        clients[numClients++] = new Process(numClients-1, client, buffer);
        //add to queue
        System.out.println("Process created" + numClients);
        return 0;
    }

    public Process schedule(){

        return null;
    }

    public void run(){
        while(true){
            //Schedule
            //System.out.println("Scheduling");
            Process client = schedule();
            //System.out.println(client);
            if(client != null) {
                int result = client.run(1);
                if (result == -1) {
                    //delete process from queue and processes
                    clients[0] = null;
                }else{
                    System.out.println("Result: "+ result);
                }
            }
        }
    }
}
