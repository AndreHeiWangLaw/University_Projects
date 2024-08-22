package ca.concordia.coen346.server;

import java.io.*;
import java.net.Socket;

public class Process {

    public final static String NUM_ITEMS = "getNumItems";
    public final static String GET_ITEM = "getItemInPos";
    public final static String NEXT_ITEM_POS = "getNextItemPos";
    public final static String TERMINATE = "terminate";

    private int processId;

    private Buffer buffer;
    private BufferedReader reader;
    private PrintWriter writer;

    public Process(int id, Socket socket, Buffer buffer) throws Exception{
        this.reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.writer = new PrintWriter(socket.getOutputStream(), true);
        this.buffer = buffer;
        this.processId = id;
        writer.println(processId);
    }

    public int run(int times){
        System.out.println("process running");

        for(int i = 0; i < times; i++) {
            try{
                writer.println("RUN");
                System.out.println("sent");
                String instruction = reader.readLine();
                System.out.println(instruction);
                if(instruction.equals(NUM_ITEMS)){
                    getNumberofItems();
                }
                else if(instruction.equals(GET_ITEM)){
                    //read position from client
                    int position=0;
                    getItem(position);
                }
                else if(instruction.equals(NEXT_ITEM_POS)){
                    getNextItemPosition();
                }
                else if(instruction.equals(TERMINATE)){
                    return -1;
                }
            }catch(IOException e){
                return -1;
            }

        }
        return 0;
    }

    public void getNumberofItems() throws IOException {
        System.out.println("Number of items");
        int numItems = buffer.readCount();
        writer.println(numItems);
    }

    public void getNextItemPosition(){

    }

    public void getItem(int position){
        int item = buffer.getNextItem(position);
        //send to client
    }
}
