#include "../include/server.h"

int main(void) {
    ServerState server_state = {0};
    ServerInitResult init_result;
    init_result = initialize_server(&server_state);
    
    if (init_result != SERVER_INIT_SUCCESS) {
        fprintf(stderr, "Server initialization failed with code %d\n", init_result);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);
    while (server_state.keep_running) {
        int client_fd = accept_connection(server_state.server_fd, SERVER_TIMEOUT_SEC);
        if (client_fd < 0) {
            // In test mode or timeout, I might want to break
            if (server_state.is_test_mode) {
                break;
            }
            continue;
        }
        if (handle_client(client_fd) < 0) {
            close(client_fd);
            continue;
        }

        close(client_fd);
    }
    close_server(&server_state);
    printf("Server shutdown complete.\n");
    return 0;
}
