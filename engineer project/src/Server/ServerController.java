package Server;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Author
 * asd
 */

public class ServerController {

    private ServerSocket serverSocketForJava;
    private ServerSocket serverSocketForHardware;
    private Socket hardwareSocket;

    /**
     *
     * @param ip
     * @param portForJava - this is the port for java
     * @param portForHardware
     * @throws IOException
     */
    public ServerController(String ip, int portForJava, int portForHardware) throws IOException {
        InetAddress bindAddress = InetAddress.getByName(ip);
        this.serverSocketForJava = new ServerSocket(portForJava, 50, bindAddress);
        this.serverSocketForHardware = new ServerSocket(portForHardware, 50, bindAddress);
        System.out.println("Server active!");
        System.out.println("Running on port for Java: " + portForJava);
        System.out.println("Running on port for Hardware: " + portForHardware);
        System.out.println("Running on IP: " + ip);
        acceptHardwareClient();
        startServerForJavaClients();
    }

    private void acceptHardwareClient() {
        new Thread(() -> {
            while (true) {
                try {
                    // Accept a new hardware client connection
                    Socket newHardwareSocket = serverSocketForHardware.accept();

                    // Close the current hardware socket if it is already connected
                    if (hardwareSocket != null && !hardwareSocket.isClosed()) {
                        hardwareSocket.close();
                        System.out.println("Replaced previous hardware client connection.");
                    }

                    // Assign the new hardware client connection to the hardwareSocket variable
                    hardwareSocket = newHardwareSocket;
                    System.out.println("New hardware client connected.");
                } catch (IOException e) {
                    System.out.println("Failed to connect hardware client: " + e.getMessage());
                }
            }
        }).start();
    }

    private void startServerForJavaClients() {
        try {
            while (!serverSocketForJava.isClosed()) {
                Socket clientSocket = serverSocketForJava.accept();
                System.out.println("Java client connected.");
                ClientHandler clientHandler = new ClientHandler(clientSocket, hardwareSocket, this);
                new Thread(clientHandler).start();
            }
        } catch (IOException e) {
            System.out.println("Server stopped or error occurred: " + e.getMessage());
        }
    }

    public void notifyHardwareClientDisconnected() {
        System.out.println("Hardware client disconnected.");
    }

    public void stopServer() {
        try {
            if (!serverSocketForJava.isClosed()) {
                serverSocketForJava.close();
            }
            if (!serverSocketForHardware.isClosed()) {
                serverSocketForHardware.close();
            }
        } catch (IOException e) {
            System.out.println(e.toString());
        }
    }
}
