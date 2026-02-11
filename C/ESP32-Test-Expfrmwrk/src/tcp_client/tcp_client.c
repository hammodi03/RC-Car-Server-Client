#include <string.h>
#include "lwip/sockets.h"
#include "esp_log.h"
#include "init/init.h"

#define Server_IP "192.168.127.246"
#define Server_Port 2250

int conn_socket = 0;
int conn_result = 0;

void connectServer() {
    vTaskDelay(pdMS_TO_TICKS(500));

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(Server_Port);
    server_address.sin_addr.s_addr = inet_addr(Server_IP);

    conn_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_socket < 0) {
        ESP_LOGI("TCP_CLIENT", "Failed to create socket, errno: %d", errno);
        return;
    }

    conn_result = connect(conn_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    ESP_LOGI("TCP_CLIENT", "Attempting to connect to %s:%d", Server_IP, Server_Port);
    if (conn_result < 0) {
        ESP_LOGI("TCP_CLIENT", "Failed to connect to server, errno: %d", errno);
        close(conn_socket);
        return;
    }
    conn_established = true;

}

int readCmd() {
    int received_data;
    int bytes_received = recv(conn_socket, &received_data, sizeof(received_data), 0);

    if (bytes_received == sizeof(received_data)) {
        ESP_LOGI("TCP_CLIENT", "Command received from server: %d", received_data);
        return received_data;
    } else if (bytes_received <= 0) {
        conn_established = false;
        close(conn_socket);
        ESP_LOGI("TCP_CLIENT", "Connection lost.");
    }
    return 0;
}

void send_status_to_server(uint8_t status) {
    if (conn_established) {
        int bytes_sent = send(conn_socket, &status, sizeof(status), 0);
        if (bytes_sent < 0) {
            ESP_LOGI("TCP_CLIENT", "Failed to send status, errno: %d", errno);
            conn_established = false;
            close(conn_socket);
        } else {
            ESP_LOGI("TCP_CLIENT", "Status byte sent to server: %d", status);
        }
    } else {
        ESP_LOGI("TCP_CLIENT", "No connection to server, cannot send status");
    }
}





