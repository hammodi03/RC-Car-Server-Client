package Server;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.Socket;

public class ClientHandler implements Runnable {
    private Socket clientSocket;
    private Socket hardwareSocket;
    private ObjectInputStream ois;
    private DataOutputStream os;
    private DataInputStream is; // Input stream for hardware status messages
    private ServerController controller;
    private DataOutputStream clientOs;

    public ClientHandler(Socket clientSocket, Socket hardwareSocket, ServerController controller) {
        this.clientSocket = clientSocket;
        this.hardwareSocket = hardwareSocket;
        this.controller = controller;
        try {
            this.ois = new ObjectInputStream(clientSocket.getInputStream());
            this.clientOs = new DataOutputStream(clientSocket.getOutputStream());
            initializeStreams();
        } catch (IOException e) {
            System.err.println("Error initializing streams: " + e.getMessage());
        }
    }

    private void initializeStreams() {
        if (hardwareSocket != null && hardwareSocket.isConnected() && !hardwareSocket.isClosed()) {
            try {
                this.os = new DataOutputStream(hardwareSocket.getOutputStream());
                this.is = new DataInputStream(hardwareSocket.getInputStream());
            } catch (IOException e) {
                System.err.println("Error initializing streams: " + e.getMessage());
                this.os = null;
                this.is = null;
            }
        } else {
            System.err.println("Hardware socket is not connected or is closed.");
            this.os = null;
            this.is = null;
        }
    }

    public void sendCommandToHardware(Object command) throws IOException {
        if (os == null) {
            initializeStreams();
            if (os == null) {
                System.err.println("Output stream to hardware is still not available.");
                return;
            }
        }
        try {
            int commandToSend = Integer.parseInt((String) command);
            os.writeInt(commandToSend);
            os.flush();
        } catch (IOException e) {
            System.err.println("Failed to send command to hardware: " + e.getMessage());
        }
    }


    private void listenForHardwareStatus() {
        new Thread(() -> {
            while (hardwareSocket != null && !hardwareSocket.isClosed()) {
                try {
                    int status = is.readUnsignedByte();
                    System.out.println("Status received from hardware: " + status);
                    sendStatusToClient(status);
                } catch (IOException e) {
                    System.err.println("Error receiving status from hardware: " + e.getMessage());
                    break;
                }
            }
        }).start();
    }

    private void sendStatusToClient(int status) {
        try {
            clientOs.writeInt(status);
            clientOs.flush();
        } catch (IOException e) {
            System.err.println("Error sending status to client: " + e.getMessage());
        }
    }

    @Override
    public void run() {
        try {
            while (!clientSocket.isClosed()) {
                listenForHardwareStatus();
                if (hardwareSocket.isClosed()) {
                    controller.notifyHardwareClientDisconnected();
                    break;
                }
                Object command = ois.readObject();
                System.out.println("Command received from client: " + command);
                sendCommandToHardware(command);

            }
        } catch (IOException | ClassNotFoundException e) {
            System.err.println("Error handling client connection: " + e.getMessage());
        }
    }

    private void closeResources() {
        try {
            if (ois != null) {
                ois.close();
            }
            if (os != null) {
                os.close();
            }
            if (is != null) {
                is.close();
            }
            if (clientSocket != null) {
                clientSocket.close();
            }
            if (hardwareSocket != null) {
                hardwareSocket.close();
            }
        } catch (IOException e) {
            System.err.println("Error closing resources: " + e.getMessage());
        }
    }
}
