package Client;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ConnectionManager {
    private String ip;
    private int port;
    private Socket socket;
    private ObjectOutputStream oos;
    private DataInputStream dis;

    public ConnectionManager(String ip, int port) {
        this.ip = ip;
        this.port = port;
        start();
    }

    private void start() {
        if (connect()) {
            System.out.println("Connected to server. Ready to send commands.");
            Gui gui = new Gui(this);
            startReceivingMessages();
        } else {
            System.out.println("Failed to connect to the server.");
        }
    }

    private boolean connect() {
        try {
            this.socket = new Socket(ip, port);
            this.oos = new ObjectOutputStream(socket.getOutputStream());
            this.dis = new DataInputStream(socket.getInputStream());
            System.out.println("CONNECTED");
            return true;
        } catch (IOException e) {
            System.out.println("Connection error: " + e.getMessage());
            return false;
        }
    }

    public void send(Object command) throws IOException {
        if (oos != null) {
            oos.writeObject(command);
            oos.flush();
        } else {
            System.out.println("Error while sending the command");
        }
    }

    private void startReceivingMessages() {
        new Thread(() -> {
            try {
                while (!socket.isClosed()) {
                    int message = dis.readInt(); // Assuming the server sends integers
                    if (message == 1) {
                        System.out.println("Message received from server: moving forward");
                    }
                    if (message == 2) {
                        System.out.println("Message received from server: moving backwards");
                    }
                    if (message == 3) {
                        System.out.println("Message received from server: turning left");
                    }
                    if (message == 4) {
                        System.out.println("Message received from server: turning right");
                    }
                    if (message == 5) {
                        System.out.println("Message received from server: motors turned off");
                    }
                    //System.out.println("Message received from server: " + message);
                }
            } catch (IOException e) {
                System.err.println("Error receiving message from server: " + e.getMessage());
            }
        }).start();
    }

    public void close() throws IOException {
        if (socket != null && !socket.isClosed()) {
            if (oos != null) oos.close();
            if (dis != null) dis.close();
            socket.close();
        }
    }
}
