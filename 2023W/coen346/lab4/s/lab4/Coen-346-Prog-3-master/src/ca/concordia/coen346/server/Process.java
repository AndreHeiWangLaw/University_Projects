package ca.concordia.coen346.server;

import java.io.*;
import java.net.Socket;

public class Process {
    public final static String NUM_ITEMS = "getNumItems";
    public final static String GET_ITEM = "getItemInPos";
    public final static String NEXT_ITEM_POS = "getNextItemPos";
    public final static String PRODUCE_ITEM = "produceItem";
    public final static String CONSUME_ITEM = "consumeItem";
    public final static String TERMINATE = "terminate";

    private int processId;
    static final int MIN_PID = 300;
    static final int MAX_PID = 500;

    static int[] pidArray;

    private Buffer buffer;
    private BufferedReader reader;
    private PrintWriter writer;

    public Process(int id, Socket socket, Buffer buffer) throws Exception {
        allocateMap();
        this.reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.writer = new PrintWriter(socket.getOutputStream(), true);
        this.buffer = buffer;
        this.processId = allocatePid();
        writer.println(processId);
        System.out.println(processId);
    }

    public int run(int times) {
        System.out.println("process running");

        for (int i = 0; i < times; i++) {
            try {
                writer.println("RUN");
                System.out.println("sent");
                String instruction = reader.readLine();
                System.out.println(instruction);
                if (instruction.equals(NUM_ITEMS)) {
                    getNumberofItems();
                } else if (instruction.equals(GET_ITEM)) {
                    int position = Integer.parseInt(reader.readLine());
                    getItem(position);
                } else if (instruction.equals(NEXT_ITEM_POS)) {
                    getNextItemPosition();
                } else if (instruction.equals(PRODUCE_ITEM)) {
                    int item = Integer.parseInt(reader.readLine());
                    produceItem(item);
                } else if (instruction.equals(CONSUME_ITEM)) {
                    consumeItem();
                } else if (instruction.equals(TERMINATE)) {
                    return -1;
                }
            } catch (IOException e) {
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

    public void getNextItemPosition() {
        // implementation to be added
    }

    public void getItem(int position) {
        int item = buffer.getNextItem(position);
        writer.println(item);
    }

    public void produceItem(int item) {
        buffer.insertItem(item, 1);
    }

    public void consumeItem() {
        buffer.getNextPosition();
    }

    public void allocateMap() throws Exception {
        pidArray = new int[MAX_PID - MIN_PID];
        if (pidArray == null) {
            System.out.println("Unsuccessful");
        }
        for (int i = 0; i < pidArray.length; i++) {
            pidArray[i] = 0;
        }
        System.out.println("Success");
    }

    static int allocatePid() throws Exception {
        if (pidArray == null) {
            System.out.println("PID manager not initialized");
            return -1;
        }
        int pidNum = -1;
        for (int i = 0; i < pidArray.length; i++) {
            if (pidArray[i] == 0) {
                pidArray[i] = 1;
                pidNum = i + MIN_PID;
                break;
            }
        }
        if (pidNum == -1) {
            System.out.println("Unable to allocate PID");
            return -1;
        }
        System.out.println("Allocate PID :" + pidNum);
        return pidNum;
    }

}
