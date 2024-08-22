package ca.concordia.coen346.client;

import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;

public class ConsumerClient {

    public static void main(String[] args){
        try(Socket socket = new Socket("localhost", 8000)){
            System.out.println("Consumer connected");
            OutputStream output = socket.getOutputStream();
            PrintWriter writer = new PrintWriter(output, true);

            InputStream input = socket.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(input));

            //wait for signal to start
            String fromServer = reader.readLine();
            System.out.println(fromServer);
            if (fromServer.equals("RUN")) {
                int numItems = Integer.parseInt(reader.readLine());
                for (int i = 0; i < numItems; i++) {
                    String item = reader.readLine();
                    System.out.println("Received item: " + item);
                }
            }

            writer.println("DONE");

        } catch (UnknownHostException ex) {
            System.out.println("Server not found: " + ex.getMessage());
        } catch (IOException ex) {
            System.out.println("I/O error! " + ex.getMessage());
        }
    }
}
