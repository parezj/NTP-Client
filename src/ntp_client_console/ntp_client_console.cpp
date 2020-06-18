/**************************************
* Author:   Jakub Parez
* File:     ntp_client_console.cpp
* Project:  NTP client
* Company:  CTU/VIN
* Date:     2020/4/29 11:25
* License:  MIT
***************************************/

#include <iostream>
#include "ntp_client.h" 

#pragma warning(disable:4996)                       // disable CRT secure warnings

#define IP          "195.113.144.201"               // NTP server ip <tik.cesnet.cz>
#define INTERVAL    1000                            // interval of NTP server query

using namespace CTU::VIN::NTP_client;

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);       // console ctrl handler prototype

HNTP client;                                        // NTP client object opaque pointer

int main(int argc, char** argv)                     // C style example
{
    enum Status s;                                  // status enum as a result from query
    struct Result* result = nullptr;                // result struct with all data
    char status_str[100];                           // status string from enum container 
    char result_str[400];                           // result print info char container
    char ip[20];                                    // IP char container

    if (argc > 1)                                   // if there exists argument,
        strncpy(ip, argv[1], 20);                   // copy it into IP array
    else
        strcpy(ip, IP);                             // otherwise use predefined IP value

    SetConsoleCtrlHandler(ConsoleHandler, true);    // assign console window onclose event
    client = Client__create();                      // finally create NTP client

    while (true)
    {
        try
        {
            s = Client__query_and_sync(client, ip, &result);    // main NTP server query

            Client__get_status_str(s, status_str);              // convert status enum to str
            printf("\nStatus: %s\n", status_str);               // print status string

            if (s == Status::OK) {                              // if status ok, print info
                Client__format_info_str(result, result_str);    // format info into string
                Client__free_result(result);                    // delete dynamic object
                printf("%s", result_str);                       // print all important info
            }
            else {                                              // if status not ok, quit
                Client__free_result(result);                    // delete dynamic object
                break;
            }           
            Sleep(INTERVAL);                                    // sleep for specified time
        }
        catch (const std::exception& exc)                       // catch any exception
        {
            printf("%s", exc.what());                           // print exception details
            break;                                              // quit
        }
    }
    getchar();                                                  // finally wait for user input
}

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType)                    // console ctrl event handler
{
    if (CTRL_CLOSE_EVENT == dwCtrlType)                         // close window event fired
    {
        if (client != NULL)                                     // if client exists,
        {
            Client__close(client);                              // close / clean-up
        }
    }
    return TRUE;
}