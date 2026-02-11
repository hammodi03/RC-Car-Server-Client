
extern int conn_socket;
extern int conn_result;

void socket_client_task(void *pvParameters);
void connectServer();
int readCmd();
void send_status_to_server(uint8_t status);