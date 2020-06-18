/**************************************
* Author:   Jakub Parez
* File:     ntp_client.cpp
* Project:  NTP client
* Company:  CTU/VIN
* Date:     2020/4/19 10:32
* License:  MIT
***************************************/

#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinSDKVer.h>
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0601		// Windows 7 and higher
#include <SDKDDKVer.h>
//#define Interface __interface
//#define CDECL		__cdecl
//#define PACKED_STRUCT(name) __pragma(pack(push, 1)) struct name __pragma(pack(pop))
#else
#error Unknown / unsupported platform
#endif

//Ws2_32.lib
//Mswsock.lib
//AdvApi32.lib

#include <assert.h>
#include <ws2tcpip.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "..\include\ntp_client.h"


#if defined(WIN32)
BOOL WINAPIV DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	// Perform actions based on the reason for calling.
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.            
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.            
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		break;
	}
	return TRUE;
}
#endif


namespace CTU {	namespace VIN { namespace NTP_client
{
	/* Exported DLL functions - instance */

	extern "C" EXPORT HNTP CDECL Client__create(void)
	{ 
		return reinterpret_cast<void*>(new Client());
	}

	extern "C" EXPORT void CDECL Client__close(HNTP self)
	{
		delete reinterpret_cast<IClient*>(self);
	}

	extern "C" EXPORT enum Status CDECL Client__query(HNTP self, const char* hostname, struct Result** result_out)
	{
		ResultEx* result = nullptr;
		*result_out = new Result();

		Status ret = reinterpret_cast<IClient*>(self)->query(hostname, &result);

		if (result != nullptr)
		{
			(*result_out)->mtr = result->mtr;
			Client::time_pt_class_to_struct(result->time, (*result_out)->time);
			delete result;
		}
		return ret;
	}

	extern "C" EXPORT enum Status CDECL Client__query_and_sync(HNTP self, const char* hostname, struct Result** result_out)
	{
		ResultEx* result = nullptr;
		*result_out = new Result();

		Status ret = reinterpret_cast<IClient*>(self)->query_and_sync(hostname, &result);

		if (result != nullptr)
		{
			(*result_out)->mtr = result->mtr;
			Client::time_pt_class_to_struct(result->time, (*result_out)->time);
			delete result;
		}
		return ret;
	}

	/* Exported DLL functions - static */

	extern "C" EXPORT void CDECL Client__format_info_str(struct Result* result, char* str_out)
	{
		Client::format_info_str(result, str_out);
	}

	extern "C" EXPORT void CDECL Client__get_status_str(enum Status status, char* str_out)
	{
		Client::get_status_str(status, str_out);
	}

	extern "C" EXPORT void CDECL Client__free_result(struct Result* result)
	{
		if (result != NULL)
			delete result;
	}

	/*  Public Instance Methods  */

	Client::Client()
	{
		this->packet = new Packet();
		this->delay_sum_ns = 0;
		this->prev_delay = 0;
	}

	Client::~Client()
	{	
		delete this->packet;
	}

	Status Client::query(const char* hostname, ResultEx** result_out)
	{
		assert(hostname != NULL && strlen(hostname) > 0);

		try
		{
			// if hostname changed, reinit
			if (strcmp(this->host, hostname) != 0)
			{
				Status ret1 = this->init(hostname);
				strncpy_s(this->host, hostname, strlen(hostname));
				printf("Server: %s\n", this->host);

				if (ret1 != Status::OK)
					return ret1;
			}
					
			// start meas time
			//auto t1 = std::chrono::high_resolution_clock::now();

			// NTP server query
			Status ret2 = this->query();

			// stop meas time
			//auto t2 = std::chrono::high_resolution_clock::now();

			if (ret2 != Status::OK)
				return ret2;

			if (*result_out == nullptr)
				*result_out = new ResultEx();

			// save results
			(*result_out)->mtr.delay_ns = this->delay_ns;
            (*result_out)->mtr.jitter_ns = this->delay_ns;
			(*result_out)->mtr.offset_ns = this->offset_ns;
			(*result_out)->time = this->time;
			double t_diff = 0;

			// calc jitter
			if (this->prev_delay > 0)
			{
				//double t1_ns = (double)t1.time_since_epoch().count();
				//double t2_ns = (double)t2.time_since_epoch().count();
				//t_diff = (t2_ns - t1_ns);
				//double delay_diff = t_diff - this->prev_delay;
				double delay_diff = (*result_out)->mtr.delay_ns - this->prev_delay;
				if (delay_diff < 0)
					delay_diff *= -1;

				double sum = 0;
				delay_ringbuff[ringbuff_front] = delay_diff;

				if (ringbuff_front < JITTER_WINDOW - 1)
					ringbuff_front++;
				else
					ringbuff_front = 0;

				if (ringbuff_count < JITTER_WINDOW)
					ringbuff_count++;

				for (int i = 0; i < ringbuff_count; i++)
					sum += delay_ringbuff[i];

				this->jitter_ns = sum / ringbuff_count;
			}

			(*result_out)->mtr.jitter_ns = this->jitter_ns;
			(*result_out)->mtr.delta_ns = this->delay_ns + this->offset_ns;
			this->prev_delay = (*result_out)->mtr.delay_ns; // t_diff

			return ret2;
		}
		catch (const std::exception& exc)
		{  
			std::cout << exc.what();
			return Status::UNKNOWN_ERR;
		}
	}

	Status Client::query_and_sync(const char* hostname, ResultEx** result_out)
	{
		Status ret1 = this->query(hostname, result_out);

		if (ret1 != Status::OK)
			return ret1;

		return this->set_win_clock(this->time);
	}

	/*  Public Static Methods  */

	void Client::time_pt_class_to_struct(time_point_t tp, TimePt& time_pt)
	{
		int y, m, d, h, mm, s, ms, us, ns;
		Client::extract_time_point(tp, y, m, d, h, mm, s, ms, us, ns);

		time_pt.tm_nsec = ns;
		time_pt.tm_usec = us;
		time_pt.tm_msec = ms;
		time_pt.tm_sec = s;
		time_pt.tm_min = mm;
		time_pt.tm_hour = h;
		time_pt.tm_mday = d;
		time_pt.tm_mon = m;
		time_pt.tm_year = y;
	}

	void Client::extract_time_point(time_point_t tp, int& y, int& m, int& d, int& hr, int& min, int& sec, int& ms, int& us, int& ns) noexcept
	{
		time_t tt = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
		tm local_tm;
		localtime_s(&local_tm , &tt);

		y = local_tm.tm_year + 1900;
		m = local_tm.tm_mon + 1;
		d = local_tm.tm_mday;
		hr = local_tm.tm_hour;
		min = local_tm.tm_min;
		sec = local_tm.tm_sec;

		auto nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count() - (tt * 1000000000);
		ms = (int)(nanosec / 1000000);
		nanosec -= ms * 1000000;
		us = (int)(nanosec / 1000);
		nanosec -= us * 1000;
		ns = (int)(nanosec);
	}

	void Client::format_info_str(Result* result, char* str)
	{
		if (result != nullptr)
		{
			int y, m, d, h, mm, s, ms, us, ns;
			y = result->time.tm_year;
			m = result->time.tm_mon;
			d = result->time.tm_mday;
			h = result->time.tm_hour;
			mm = result->time.tm_min;
			s = result->time.tm_sec;
			ms = result->time.tm_msec;
			us = result->time.tm_usec;
			ns = result->time.tm_nsec;

			char time_str[200];
			char offset_str[50];
			char delay_str[50];
			char jitter_str[50];
			char delta_str[50];

			sprintf(time_str, "Time:   %u/%02u/%02u %02u:%02u:%02u [%03u %03u %03u]\n", y, m, d, h, mm, s, ms, us, ns);
			sprintf(offset_str, "Offset: %.6f ms\n", result->mtr.offset_ns / 1000000.0);
			sprintf(delay_str, "Delay:  %.6f ms\n", result->mtr.delay_ns / 1000000.0);
			sprintf(jitter_str, "Jitter: %.6f ms\n", result->mtr.jitter_ns / 1000000.0);
			sprintf(delta_str, "Delta:  %.6f ms\n", (result->mtr.offset_ns + result->mtr.delay_ns) / 1000000.0);
			sprintf(str, "%s%s%s%s%s", time_str, offset_str, delay_str, jitter_str, delta_str);
		}
	}

	void Client::format_info_str(ResultEx* result, char* str)
	{
		if (result != nullptr)
		{
			Result res;
			Client::time_pt_class_to_struct(result->time, res.time);
			Client::format_info_str(&res, str);
		}		
	}

	void Client::get_status_str(Status status, char* str)
	{
		auto s = (unsigned int)status;
		if (s >= 0 && s <= 8)
			strcpy(str, status_s[s]);
	}

	/*  Private Methods  */

	Status Client::init(const char* hostname)
	{
		this->slen = sizeof(this->si_addr);

		//Initialise winsock
		//printf(&quot; \nInitialising Winsock... & quot;);
		if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0)
		{
			//printf(&quot; Failed.Error Code : % d & quot; , WSAGetLastError());
			return Status::INIT_WINSOCK_ERR;
		}
		//printf(&quot; Initialised.\n & quot;);

		//create socket
		if ((this->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
		{
			//printf(&quot; socket() failed with error code : % d & quot;, WSAGetLastError());
			return Status::CREATE_SOCKET_ERR;
		}

		//setup address structure
		memset((char*)&this->si_addr, 0, sizeof(si_addr));
		si_addr.sin_family = AF_INET;
		si_addr.sin_port = htons(UDP_PORT);
		InetPtonA(AF_INET, hostname, &(si_addr.sin_addr));

		int rx_timeout = RX_TIMEOUT;
		setsockopt(this->s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&rx_timeout, sizeof(rx_timeout));

		return Status::OK;
	}

	Status Client::query()
	{
		memset(this->packet, '\0', NTP_PACKET_LEN);
		memset(this->ans, '\0', NTP_PACKET_LEN);

		this->packet->li_vn_mode = 227;			// 11100011 (client mode + NTPv4)
		this->packet->stratum = 0;				// Stratum level of the local clock
		this->packet->poll = 4;					// Maximum interval between successive messages
		this->packet->precision = (uint8_t)-10;	// Precision of the local clock (expressed in power of 2, -10 means 2-10, that is to say 1/1024=0.97ms)
		this->packet->rootDelay = 256;			// Total round trip delay time
		this->packet->rootDispersion = 256;		// Max error aloud from primary clock source
				
		memcpy(this->msg, this->packet, NTP_PACKET_LEN);

		//send the message
		time_point_t t1 = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
		auto t1_b = std::chrono::high_resolution_clock::now();

		if (sendto(this->s, this->msg, NTP_PACKET_LEN, 0, (struct sockaddr*) &this->si_addr, this->slen) == SOCKET_ERROR)
		{
			//printf(&quot; sendto() failed with error code : % d & quot;, WSAGetLastError());
			return Status::SEND_MSG_ERR;
		}

		// receive message
		if (recvfrom(this->s, this->ans, NTP_PACKET_LEN, 0, (struct sockaddr*) &this->si_addr, &this->slen) == SOCKET_ERROR)
		{
			//printf(&quot; recvfrom() failed with error code : % d & quot;, WSAGetLastError());
			if (WSAGetLastError() == WSAETIMEDOUT)
				return Status::RECEIVE_MSG_TIMEOUT;
			return Status::RECEIVE_MSG_ERR;
		}
		time_point_t t4 = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
		auto t4_b = std::chrono::high_resolution_clock::now();

		memcpy(this->packet, this->ans, NTP_PACKET_LEN);

		this->packet->txTm_s = ntohl(this->packet->txTm_s);
		this->packet->txTm_f = ntohl(this->packet->txTm_f); 

		this->packet->rxTm_s = ntohl(this->packet->rxTm_s);
		this->packet->rxTm_f = ntohl(this->packet->rxTm_f);

		uint32_t txTm = this->packet->txTm_s - NTP_TS_DELTA; // seconds from 1970
		uint32_t rxTm = this->packet->rxTm_s - NTP_TS_DELTA;

		double t2_ns = (double)txTm * 1000000000; // nanoseconds from 1970
		double t3_ns = (double)rxTm * 1000000000;

		t2_ns += ((double)this->packet->txTm_f / 4294967295) * 1000000000; // add fraction
		t3_ns += ((double)this->packet->rxTm_f / 4294967295) * 1000000000; // 0x100000000ULL = 4294967295

		double t1_ns = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t1.time_since_epoch()).count(); // nanoseconds from 1970
		double t4_ns = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(t4.time_since_epoch()).count();

		double t1_b_ns = (double)t1_b.time_since_epoch().count(); // nanoseconds from start measuring
		double t4_b_ns = (double)t4_b.time_since_epoch().count();

		this->offset_ns = ((t2_ns - t1_ns) + (t3_ns - t4_ns)) / 2;
		this->delay_ns = (t4_b_ns - t1_b_ns) - (t3_ns - t2_ns);
		this->time = t4 + std::chrono::nanoseconds((long long)(offset_ns + delay_ns));

		return Status::OK;
	}

	Status Client::set_win_clock(time_point_t tm) // sysinfoapi.h
	{
		SYSTEMTIME st;
		//GetSystemTime(&gt);

		int y, m, d, h, mm, sec, ms, us, ns;
		Client::extract_time_point(tm, y, m, d, h, mm, sec, ms, us, ns);

		st.wYear = (WORD)y;
		st.wMonth = (WORD)m;
		st.wDay = (WORD)d;
		st.wHour = (WORD)h;
		st.wMinute = (WORD)mm;
		st.wSecond = (WORD)sec;
		st.wMilliseconds = (WORD)ms;

		auto ret = SetLocalTime(&st);

		if (ret)
		{
			return Status::OK;
		}			
		else
		{
			auto err = GetLastError();
			if (err == 1314)
				return Status::ADMIN_RIGHTS_NEEDED;
			else
				return Status::SET_WIN_TIME_ERR;
		}			
	}
}}}