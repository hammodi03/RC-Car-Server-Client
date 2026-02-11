package Server;

import java.io.IOException;

public class MainProgram {
    public static void main(String[] args) throws IOException {
        ServerController serverController = new ServerController("192.168.127.246", 2240, 2250);
    }
}

