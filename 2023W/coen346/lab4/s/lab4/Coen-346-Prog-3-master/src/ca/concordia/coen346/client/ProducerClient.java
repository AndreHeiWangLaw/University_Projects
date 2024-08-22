package ca.concordia.coen346.client;

import ca.concordia.coen346.server.Process;

import java.io.*;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.SocketImpl;
import java.net.UnknownHostException;

public class ProducerClient {

    public static void main(String[] args){
        int id;
        try(Socket socket = new Socket("localhost", 8000)){
            System.out.println("Producer connected");
            OutputStream output = socket.getOutputStream();
            PrintWriter writer = new PrintWriter(output, true);

            InputStream input = socket.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(input));
            //get Id
            String fromServer = reader.readLine();
            System.out.println(fromServer);
            id = Integer.parseInt(fromServer);

            //wait for signal to start
            fromServer = reader.readLine();
            System.out.println(fromServer);
            int i = id;
            while(i<500) {
                System.out.println(fromServer);
                if (fromServer.equals("RUN")) {
                    writer.println(Process.NUM_ITEMS);
                    System.out.println("Sent request");
                    int numItems = Integer.parseInt(reader.readLine());
                    System.out.println("num:" + numItems);
                    fromServer = reader.readLine();
                }
                i++;
            }
            if (fromServer.equals("RUN")) {
                writer.println(Process.TERMINATE);
            }

        }catch (UnknownHostException ex) {

            System.out.println("Server not found: " + ex.getMessage());

        } catch (IOException ex) {

            System.out.println("I/O error! " + ex.getMessage());
        }

    }
}
