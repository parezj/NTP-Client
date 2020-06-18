/**************************************
* Author:   Jakub Parez
* File:     ntp_client.h
* Project:  NTP client
* Company:  CTU/VIN
* Date:     2020/4/19 10:30
* License:  MIT
***************************************/

#ifndef ntp_client_h
#define ntp_client_h

#define EXPORT		__declspec(dllexport)
#define IMPORT		__declspec(dllimport)

#ifdef __cplusplus
#include <chrono>
#include <winsock2.h>
#endif

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
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| LI | VN | Mode | Stratum | Poll | Precision |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Root Delay |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Root Dispersion |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Reference Identifier |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                   Reference Timestamp(64)                     |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                   Originate Timestamp(64)                     |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                    Receive Timestamp(64)                      |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                    Transmit Timestamp(64)                     |
|                                                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
| Key Identifier(optional) (32) |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                                                               |
|                                                               |
|                 Message Digest(optional) (128)                |
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

#ifdef __cplusplus
namespace CTU {	namespace VIN { namespace NTP_client
{
#endif
	struct TimePt 
	{
		int tm_nsec;			/* nano seconds, range 0 to 999     */
		int tm_usec;			/* micro seconds, range 0 to 999    */
		int tm_msec;			/* mili seconds, range 0 to 999     */
		int tm_sec;				/* seconds, range 0 to 59           */
		int tm_min;				/* minutes, range 0 to 59           */
		int tm_hour;			/* hours, range 0 to 23             */
		int tm_mday;			/* day of the month, range 1 to 31  */
		int tm_mon;				/* month, range 0 to 11             */
		int tm_year;			/* The number of years since 1900   */
	};

	struct Metrics
	{ 
		double delay_ns;		/* delay from transporting packet through network */
		double offset_ns;		/* offset from the server time */
		double jitter_ns;		/* network jitter accumulated in time when calling sync */
		double delta_ns;		/* actual difference between server and client time */
	};

	struct Result
	{
		struct TimePt time;		/* plain C style struct time point with actual time*/
		struct Metrics mtr;		/* metrics values with other relevant data */
	};

#ifdef __cplusplus
	typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time_point_t;

	class ResultEx
	{
	public:
		time_point_t time;		/* time point class with actual time */
		Metrics mtr;			/* metrics values with other relevant data */
	};
#endif

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

	typedef void* HNTP;

#ifdef __cplusplus
	extern "C" {
#endif 
	/* object lifecycle */
	EXPORT HNTP __cdecl Client__create(void);
	EXPORT void __cdecl Client__close(HNTP self);

	/* main NTP server query functions */
	EXPORT enum Status __cdecl Client__query(HNTP self, const char* hostname, struct Result** result_out);
	EXPORT enum Status __cdecl Client__query_and_sync(HNTP self, const char* hostname, struct Result** result_out);

	/* helper functions */
	EXPORT void __cdecl Client__format_info_str(struct Result* result, char* str_out);
	EXPORT void __cdecl Client__get_status_str(enum Status status, char* str_out);
	EXPORT void __cdecl Client__free_result(struct Result* result);
#ifdef __cplusplus      
	}

	Interface IClient
	{
		virtual Status query(const char* hostname, ResultEx** result_out) = 0;
		virtual Status query_and_sync(const char* hostname, ResultEx** result_out) = 0;
		virtual ~IClient() {};
	};

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

		virtual Status query(const char* hostname, ResultEx** result_out);
		virtual Status query_and_sync(const char* hostname, ResultEx** result_out);

		static void format_info_str(Result* result, char* str);
		static void format_info_str(ResultEx* result, char* str);
		static void get_status_str(Status status, char* str);
		static void time_pt_class_to_struct(time_point_t tp, TimePt& time_pt);
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

		enum Status init(const char* hostname);
		enum Status query();
		enum Status set_win_clock(time_point_t tm);

		double offset_ns = 0;
		double delay_ns = 0;
		time_point_t time;
		double jitter_ns = 0;
		double delay_sum_ns = 0;
		double prev_delay = 0;

		double delay_ringbuff [JITTER_WINDOW];
		int ringbuff_front = 0;
		int ringbuff_count = 0;
	};

	const char status_s[9][50] = { "OK", "Unknown Error", "Init Winsock Error", "Create Socket Error", "Send Message Error",
		"Receive Message Error", "Receive Message Timeout", "Set Windows Time Error", "Admin Rights Needed" };
}}}
#endif
#endif