#include <iostream>
#include "ntp_client.h" 

using namespace CTU::VIN::NTP_client;

#define INTERVAL 1000

int main()
{
    result_t* result = nullptr;
    char status_str[100];
    HNTP client = Client__create();

    while (true)
    {
        status_t status = Client__query_and_sync(client, "195.113.144.201", &result); // tik.cesnet.cz

        Client__get_status_str(status, status_str);
        printf("\nStatus: %s\n", status_str);
        if (status == Status::OK)
            Client__print_info(result);
        else
            break;

        Sleep(INTERVAL);
    }

    Client__close(client);
    delete(result);
    getchar();
}
