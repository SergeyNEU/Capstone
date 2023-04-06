#include <iostream>
#include <sstream>
#include <cstdlib>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

int sdp_get_rfcomm_channel(const std::string &bt_addr_str, const std::string &uuid_str)
{
    bdaddr_t bt_addr;
    str2ba(bt_addr_str.c_str(), &bt_addr);

    uuid_t service_uuid;
    uint32_t uuid_int = strtol(uuid_str.c_str(), NULL, 16);
    sdp_uuid128_create(&service_uuid, &uuid_int);

    bdaddr_t any_addr = {{0, 0, 0, 0, 0, 0}};
    sdp_session_t *session = sdp_connect(&bt_addr, &any_addr, SDP_RETRY_IF_BUSY);
    if (!session)
    {
        std::cerr << "Failed to connect to SDP server." << std::endl;
        return -1;
    }

    sdp_list_t *search_list = sdp_list_append(NULL, &service_uuid);
    uint32_t range = 0xFFFF;
    sdp_list_t *attrid_list = sdp_list_append(NULL, &range);
    sdp_list_t *response_list = NULL;
    sdp_service_search_attr_req(session, search_list, SDP_ATTR_REQ_RANGE, attrid_list, &response_list);

    int channel = -1;

    for (sdp_list_t *r = response_list; r; r = r->next)
    {
        sdp_record_t *record = (sdp_record_t *)r->data;
        sdp_list_t *proto_list;

        if (sdp_get_access_protos(record, &proto_list) == 0)
        {
            for (sdp_list_t *p = proto_list; p; p = p->next)
            {
                sdp_list_t *proto_desc = (sdp_list_t *)p->data;

                if (proto_desc != NULL && proto_desc->data != NULL && RFCOMM_UUID == ((sdp_data_t *)proto_desc->data)->val.uuid.value.uuid16)
                {
                    sdp_data_t *channel_data = sdp_data_get(record, 0x09);
                    if (channel_data)
                    {
                        channel = channel_data->val.int8;
                    }
                }
            }
            sdp_list_free(proto_list, (sdp_free_func_t)sdp_data_free);
        }
        if (channel != -1)
        {
            break;
        }
    }

    sdp_list_free(response_list, 0);
    sdp_list_free(search_list, 0);
    sdp_list_free(attrid_list, 0);
    sdp_close(session);

    if (channel == -1)
    {
        std::cerr << "Failed to find RFCOMM channel for specified UUID." << std::endl;
    }

    return channel;
}

bool transfer_file_over_bluetooth_ftp(const std::string &bt_addr, const std::string &uuid_str, const std::string &file_path)
{
    int ftp_channel = sdp_get_rfcomm_channel(bt_addr, uuid_str);
    //int ftp_channel = 10; // Replace with the correct channel number
    if (ftp_channel < 0)
    {
        std::cerr << "Failed to find RFCOMM channel for specified UUID." << std::endl;
        return false;
    }

    std::stringstream command;
    command << "obexftp --nopath --noconn --uuid none --bluetooth " << bt_addr << " --channel " << ftp_channel << " -p " << file_path;
    int result = std::system(command.str().c_str());

    return result == 0;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: ./bluetooth_file_transfer <bt_addr> <file_path>" << std::endl;
        return 1;
    }
    const std::string bt_addr = argv[1];
    const std::string file_path = argv[2];
    const std::string uuid_str = "f6d79071f92f4cd998b934a1ce6dae9e";
    if (transfer_file_over_bluetooth_ftp(bt_addr, uuid_str, file_path))
    {
        std::cout << "File transfer completed successfully." << std::endl;
    }
    else
    {
        std::cout << "File transfer failed." << std::endl;
    }

    return 0;
}
