/**************************************
* Author:   Jakub Parez
* File:     ntp_client.h
* Project:  NTP client
* Company:  CTU/VIN
* Date:     2020/4/19 10:30
* License:  MIT
***************************************/

#pragma once

// targetver
#if defined(_MSC_VER)			// Microsoft compiler
#include <WinSDKVer.h>
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0601		// Windows 7 and higher
#include <SDKDDKVer.h>
#endif

#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define EXPORT		__declspec(dllexport)
#define IMPORT		__declspec(dllimport)
//#define Interface __interface
//#define CDECL		__cdecl
#define PACKED_STRUCT(name) __pragma(pack(push, 1)) struct name __pragma(pack(pop))
#else
#error Unknown / unsupported platform
#endif

//Ws2_32.lib
//Mswsock.lib
//AdvApi32.lib

#include <chrono>
#include <assert.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <iostream>
#include <string>


#define Interface struct

#define NTP_PACKET_LEN		48
#define UDP_PORT			123
#define RX_TIMEOUT			5000
#define JITTER_WINDOW		100

#define STR_MAX_SIZE		100
#define NTP_TS_DELTA		2208988800ull


//[module(name = "ntp_client")];

/*
Below is a description of the NTP / SNTP Version 4 message format,
which follows the IPand UDP headers.This format is identical to
that described in RFC - 1305, with the exception of the contents of the
reference identifier field.The header fields are defined as follows :

1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+ -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| LI | VN | Mode | Stratum | Poll | Precision |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Root Delay |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Root Dispersion |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Reference Identifier |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                   Reference Timestamp(64)                    |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                   Originate Timestamp(64)                    |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                    Receive Timestamp(64)                     |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                    Transmit Timestamp(64)                    |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Key Identifier(optional) (32) |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                                                               |
|                 Message Digest(optional) (128)               |
|                                                               |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Reference Timestamp : This is the time at which the local clock was
last set or corrected, in 64 - bit timestamp format.

Originate Timestamp : This is the time at which the request departed
the client for the server, in 64 - bit timestamp format.

Receive Timestamp : This is the time at which the request arrived at
the server, in 64 - bit timestamp format.

Transmit Timestamp : This is the time at which the reply departed the
server for the client, in 64 - bit timestamp format.


 * NTP uses two fixed point formats.  The first (l_fp) is the "long"
 * format and is 64 bits long with the decimal between bits 31 and 32.
 * This is used for time stamps in the NTP packet header (in network
 * byte order) and for internal computations of offsets (in local host
 * byte order). We use the same structure for both signed and unsigned
 * values, which is a big hack but saves rewriting all the operators
 * twice. Just to confuse this, we also sometimes just carry the
 * fractional part in calculations, in both signed and unsigned forms.
 * Anyway, an l_fp looks like:
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                         Integral Part                         |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                         Fractional Part                       |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * REF http://www.eecis.udel.edu/~mills/database/rfc/rfc2030.txt

	offset = [(T2 - T1) + (T3 - T4)] / 2
	delay = (T4 - T1) - (T3 - T2)

 */


namespace CTU
{
    namespace VIN
    {
        namespace NTP_client
        {
			typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time_point_t;

			class Result
			{
			public:
				time_point_t time;
				double delay_ns;
				double offset_ns;
				double jitter_ns;
				double delta_ns;
			};
			typedef Result result_t;

			enum Status : int16_t
			{
				OK = 0,
				UNKNOWN_ERR = 1,
				INIT_WINSOCK_ERR = 2,
				CREATE_SOCKET_ERR = 3,
				SEND_MSG_ERR = 4,
				RECEIVE_MSG_ERR = 5,
				RECEIVE_MSG_TIMEOUT = 6,
				SET_WIN_TIME_ERR = 7,
				ADMIN_RIGHTS_NEEDED = 8
			};
			typedef Status status_t;

			const char status_s[9][50] = { "OK", "Unknown Error", "Init Winsock Error", "Create Socket Error", "Send Message Error",
				"Receive Message Error", "Recive Message Timeout", "Set Windows Time Error", "Admin Rights Needed" };

			Interface IClient
			{
				virtual status_t query(const char* hostname, result_t** result_out) = 0;
				virtual status_t query_and_sync(const char* hostname, result_t** result_out) = 0;
				virtual ~IClient() {};
			};
			typedef IClient* HNTP;
			//typedef Client* HNTP; // TODO check if works

			extern "C" EXPORT HNTP CDECL Client__create(void);
			extern "C" EXPORT void CDECL Client__close(HNTP self);
			extern "C" EXPORT status_t CDECL Client__query(HNTP self, const char* hostname, result_t** result_out);
			extern "C" EXPORT status_t CDECL Client__query_and_sync(HNTP self, const char* hostname, result_t** result_out);

			// static
			extern "C" EXPORT void CDECL Client__print_info(result_t* result);
			extern "C" EXPORT void CDECL Client__get_status_str(status_t status, char* str_out);
			extern "C" EXPORT void CDECL Client__extract_time_point(time_point_t tp, int& y, int& m, int& d, int& hr, int& min,
				int& sec, int& ms, int& us, int& ns);


			class Packet				 // Total: 384 bits or 48 bytes.
			{
			public:
				uint8_t li_vn_mode;      //  8 bits. li, vn, and mode.
										 //    -li   2 bits. Leap indicator.
										 //    -vn   3 bits. Version number of the protocol. 3,4
										 //    -mode 3 bits. Client will pick mode 3 for client.

				uint8_t stratum;         //  8 bits. Stratum level of the local clock.
				uint8_t poll;            //  8 bits. Maximum interval between successive messages.
				uint8_t precision;       //  8 bits. Precision of the local clock.
				uint32_t rootDelay;      // 32 bits. Total round trip delay time.
				uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.

				uint32_t refId;          // 32 bits. Reference clock identifier.
				uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
				uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

				uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds. = t1
				uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

				uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds. = t2
				uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

				uint32_t txTm_s;         // 32 bits. Transmit time-stamp seconds. = t3
				uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.
			};

			class Client : public IClient
			{
			protected:

			public:
				Client();
				virtual ~Client();

				virtual status_t query(const char* hostname, result_t** result_out);
				virtual status_t query_and_sync(const char* hostname, result_t** result_out);

				static void print_info(result_t* result);
				static void get_status_str(status_t status, char* str);
				static void extract_time_point(time_point_t tp, int& y, int& m, int& d, int& hr,
					int& min, int& sec, int& ms, int& us, int& ns) noexcept;

			private:
				WSADATA wsa;
				struct sockaddr_in si_addr;
				int s, slen;
				char ans[NTP_PACKET_LEN];
				char msg[NTP_PACKET_LEN];
				char host[STR_MAX_SIZE];
				Packet* packet;

				status_t init(const char* hostname);
				status_t query();
				status_t set_win_clock(time_point_t tm);

				double offset_ns;
				double delay_ns;
				time_point_t time;
				double jitter_ns;
				double delay_sum_ns;
				double prev_delay;

				double delay_ringbuff [JITTER_WINDOW];
				int ringbuff_front = 0;
				int ringbuff_count = 0;
			};
		}     
    }
}