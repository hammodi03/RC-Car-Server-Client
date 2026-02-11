package Client;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;

public class Gui extends JFrame {
    private ConnectionManager connectionManager;

    public Gui(ConnectionManager manager) {
        this.connectionManager = manager;
        setTitle("Control Panel");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(300, 300);

        // Adjusting layout to accommodate one more button
        setLayout(new GridLayout(4, 3)); // Adjusting rows to 4

        JButton btnUp = new JButton("↑");
        JButton btnDown = new JButton("↓");
        JButton btnLeft = new JButton("←");
        JButton btnRight = new JButton("→");
        JButton btnStop = new JButton("Stop"); // The new Stop button

        add(new JLabel(""));
        add(btnUp);
        add(new JLabel(""));
        add(btnLeft);
        add(new JLabel("")); // This could be a label or adjusted as needed
        add(btnRight);
        add(new JLabel(""));
        add(btnDown);
        add(new JLabel(""));
        add(new JLabel("")); // Extra label for alignment
        add(btnStop); // Adding the Stop button
        add(new JLabel("")); // Extra label for alignment

        btnUp.addActionListener(e -> sendCommand("1"));
        btnDown.addActionListener(e -> sendCommand("2"));
        btnLeft.addActionListener(e -> sendCommand("3"));
        btnRight.addActionListener(e -> sendCommand("4"));
        btnStop.addActionListener(e -> sendCommand("5")); // Action listener for Stop

        setVisible(true);
    }

    private void sendCommand(String command) {
        try {
            System.out.println("Button: " + command + " pressed");
            connectionManager.send(command);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Failed to send command: " + e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
}