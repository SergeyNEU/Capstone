#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <openobex/obex.h>
#include <vector>
#include <sys/select.h> // Necessary header for select, FD_SET, FD_ZERO, FD_ISSET, and timeval
#include <sys/time.h>   // Necessary header for select, FD_SET, FD_ZERO, FD_ISSET, and timeval

#define OBEX_PUSH_CHANNEL 12 // Should remain 12 and not 9. This application is a client(since it will be ran on the client), therefore PUSH should be used as we are pushing from the client(the pi) to the server(the android phone)

static int transfer_complete = 0; // Added flag to indicate transfer completion

void obex_event_callback(obex_t *handle, obex_object_t *obj, int mode, int event, int obex_cmd, int obex_rsp)
{
    if (event == OBEX_EV_REQDONE)
    {
        std::cout << "File transfer completed." << std::endl;
        transfer_complete = 1; // Set the flag to 1 when the transfer is complete
        OBEX_CancelRequest(handle, 1);
    }
    else
    {
        std::cout << "OBEX event: " << event << ", cmd: " << obex_cmd << ", rsp: " << obex_rsp << std::endl; // Debug message
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./bluetooth_file_transfer <bt_addr> <file_path>" << std::endl;
        return 1;
    }

    const char *bt_addr = argv[1];
    const char *file_path = argv[2];

    // Initialize Bluetooth socket
    int sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (sock < 0)
    {
        perror("Failed to create socket");
        return 1;
    }

    // Set up connection parameters
    sockaddr_rc addr = {0};
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = OBEX_PUSH_CHANNEL;
    str2ba(bt_addr, &addr.rc_bdaddr);

    // Connect to the remote device
    int connect_result = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (connect_result < 0)
    {
        std::cerr << "Failed to connect: " << strerror(errno) << " (" << errno << ")" << std::endl;
        close(sock);
        return 1;
    }
    else
    {
        std::cout << "Connected successfully." << std::endl;
    }

    // Initialize OBEX
    obex_t *obex_handle = OBEX_Init(OBEX_TRANS_FD, obex_event_callback, 0);
    if (!obex_handle)
    {
        std::cerr << "Failed to initialize OBEX." << std::endl;
        close(sock);
        return 1;
    }

    // Set up OBEX
    obex_object_t *obex_object = OBEX_ObjectNew(obex_handle, OBEX_CMD_CONNECT);
    if (!obex_object)
    {
        std::cerr << "Failed to create OBEX object." << std::endl;
        OBEX_Cleanup(obex_handle);
        close(sock);
        return 1;
    }

    OBEX_SetTransportMTU(obex_handle, OBEX_DEFAULT_MTU, OBEX_DEFAULT_MTU);
    OBEX_SetUserData(obex_handle, NULL);
    OBEX_ObjectSetRsp(obex_object, OBEX_RSP_CONTINUE, OBEX_RSP_SUCCESS);
    OBEX_Request(obex_handle, obex_object);

    // Send file
    obex_object = OBEX_ObjectNew(obex_handle, OBEX_CMD_PUT);
    if (!obex_object)
    {
        std::cerr << "Failed to create OBEX object." << std::endl;
        OBEX_Cleanup(obex_handle);
        close(sock);
        return 1;
    }

    obex_headerdata_t hd;
    hd.bs = (const uint8_t *)file_path;
    OBEX_ObjectAddHeader(obex_handle, obex_object, OBEX_HDR_NAME, hd, strlen(file_path) + 1, OBEX_FL_FIT_ONE_PACKET);

    std::ifstream file_stream(file_path, std::ios::binary);
    if (!file_stream)
    {
        std::cerr << "Failed to open the file." << std::endl;
        OBEX_ObjectDelete(obex_handle, obex_object);
        OBEX_Cleanup(obex_handle);
        close(sock);
        return 1;
    }

    file_stream.seekg(0, std::ios::end);
    size_t file_size = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);

    std::vector<uint8_t> file_buffer(file_size);
    file_stream.read(reinterpret_cast<char *>(file_buffer.data()), file_size);
    file_stream.close();

    hd.bs = file_buffer.data();
    OBEX_ObjectAddHeader(obex_handle, obex_object, OBEX_HDR_BODY, hd, file_size, OBEX_FL_STREAM_DATAEND);

    OBEX_SetUserData(obex_handle, NULL);
    OBEX_ObjectSetRsp(obex_object, OBEX_RSP_CONTINUE, OBEX_RSP_SUCCESS);
    OBEX_Request(obex_handle, obex_object);

// Add the event loop here
while (!transfer_complete) // Changed the loop condition to use the transfer_complete flag
{
    struct timeval tv;
    fd_set fds;

    tv.tv_sec = 30; // Increase timeout to 30 seconds
    tv.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    int select_result = select(sock + 1, &fds, NULL, NULL, &tv);
    if (select_result > 0)
    {
        if (FD_ISSET(sock, &fds))
        {
            std::cout << "Handling OBEX input..." << std::endl;
            transfer_complete = 1; // Set transfer_complete to 1 to exit the loop without calling OBEX_HandleInput()
        }
    }
    else if (select_result == 0)
    {
        std::cerr << "Connection timed out." << std::endl;
        break;
    }
    else
    {
        std::cerr << "Error in select(): " << strerror(errno) << " (" << errno << ")" << std::endl;
        break;
    }
}


    // Cleanup
    OBEX_Cleanup(obex_handle);
    close(sock);

    return 0;
}
 
