/**************************************
* Author:   Jakub Parez
* File:     ntp_client.cpp
* Project:  NTP client
* Company:  CTU/VIN
* Date:     2020/4/19 10:32
* License:  MIT
***************************************/

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


namespace CTU
{
	namespace VIN
	{
		namespace NTP_client
		{
			/* Exported DLL functions - instance */

			extern "C" EXPORT HNTP CDECL Client__create(void)
			{ 
				return new Client();
			}

			extern "C" EXPORT void CDECL Client__close(HNTP self)
			{
				delete self;
			}

			extern "C" EXPORT status_t CDECL Client__query(HNTP self, const char* hostname, result_t** result_out)
			{
				return self->query(hostname, result_out);
			}

			extern "C" EXPORT status_t CDECL Client__query_and_sync(HNTP self, const char* hostname, result_t** result_out)
			{
				return self->query_and_sync(hostname, result_out);
			}

			/* Exported DLL functions - static */

			extern "C" EXPORT void CDECL Client__print_info(result_t* result)
			{
				Client::print_info(result);
			}

			extern "C" EXPORT void CDECL Client__get_status_str(status_t status, char* str_out)
			{
				Client::get_status_str(status, str_out);
			}

			extern "C" EXPORT void CDECL Client__extract_time_point(time_point_t tp, int& y, int& m, int& d, int& hr,
				int& min, int& sec, int& ms, int& us, int& ns)
			{
				Client::extract_time_point(tp, y, m, d, hr, min, sec, ms, us, ns);
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

			status_t Client::query(const char* hostname, result_t** result_out)
			{
				assert(hostname != NULL && strlen(hostname) > 0);

				try
				{
					// if hostname changed, reinit
					if (strcmp(this->host, hostname) != 0)
					{
						status_t ret1 = this->init(hostname);
						strncpy_s(this->host, hostname, strlen(hostname));
						printf("Server: %s\n", this->host);

						if (ret1 != status_t::OK)
							return ret1;
					}
					
					// NTP server query
					status_t ret2 = this->query();

					if (ret2 != status_t::OK)
						return ret2;

					if (*result_out == NULL) // nullptr
						*result_out = new result_t();

					// save results
					(*result_out)->delay_ns = this->delay_ns;
                    (*result_out)->jitter_ns = this->delay_ns;
					(*result_out)->offset_ns = this->offset_ns;
					(*result_out)->time = this->time;

					// calc jitter
					if (this->prev_delay > 0)
					{
						double delay_diff = (*result_out)->delay_ns - this->prev_delay;
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

					(*result_out)->jitter_ns = this->jitter_ns;
					(*result_out)->delta_ns = this->delay_ns + this->offset_ns;
					this->prev_delay = (*result_out)->delay_ns;

					return ret2;
				}
				catch (const std::exception& exc)
				{  
					std::cout << exc.what();
					return status_t::UNKNOWN_ERR;
				}
			}

			status_t Client::query_and_sync(const char* hostname, result_t** result_out)
			{
				status_t ret1 = this->query(hostname, result_out);

				if (ret1 != status_t::OK)
					return ret1;

				return this->set_win_clock(this->time);
			}

			/*  Public Static Methods  */

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

			void Client::print_info(result_t* result) // constexpr??
			{
				if (result != NULL) // nullptr
				{
					int y, m, d, h, mm, s, ms, us, ns;
					Client::extract_time_point(result->time, y, m, d, h, mm, s, ms, us, ns);

					printf("Time:   %u/%02u/%02u %02u:%02u:%02u [%03u %03u %03u]\n", y, m, d, h, mm, s, ms, us, ns);
					printf("Offset: %.6f ms\n", result->offset_ns / 1000000.0);
					printf("Delay:  %.6f ms\n", result->delay_ns / 1000000.0);
					printf("Jitter: %.6f ms\n", result->jitter_ns / 1000000.0);
					printf("Delta:  %.6f ms\n", (result->offset_ns + result->delay_ns) / 1000000.0);
				}
			}

			void Client::get_status_str(status_t status, char* str)
			{
				auto s = (unsigned int)status;
				if (s >= 0 && s <= 8)
					strcpy(str, status_s[s]);
			}

			/*  Private Methods  */

			status_t Client::init(const char* hostname)
			{
				this->slen = sizeof(this->si_addr);

				//Initialise winsock
				//printf(&quot; \nInitialising Winsock... & quot;);
				if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0)
				{
					//printf(&quot; Failed.Error Code : % d & quot; , WSAGetLastError());
					return status_t::INIT_WINSOCK_ERR;
				}
				//printf(&quot; Initialised.\n & quot;);

				//create socket
				if ((this->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
				{
					//printf(&quot; socket() failed with error code : % d & quot;, WSAGetLastError());
					return status_t::CREATE_SOCKET_ERR;
				}

				//setup address structure
				memset((char*)&this->si_addr, 0, sizeof(si_addr));
				si_addr.sin_family = AF_INET;
				si_addr.sin_port = htons(UDP_PORT);
				InetPton(AF_INET, hostname, &(si_addr.sin_addr));

				int rx_timeout = RX_TIMEOUT;
				setsockopt(this->s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&rx_timeout, sizeof(rx_timeout));

				return status_t::OK;
			}

			status_t Client::query()
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
					return status_t::SEND_MSG_ERR;
				}

				// receive message
				if (recvfrom(this->s, this->ans, NTP_PACKET_LEN, 0, (struct sockaddr*) &this->si_addr, &this->slen) == SOCKET_ERROR)
				{
					//printf(&quot; recvfrom() failed with error code : % d & quot;, WSAGetLastError());
					if (WSAGetLastError() == WSAETIMEDOUT)
						return status_t::RECEIVE_MSG_TIMEOUT;
					return status_t::RECEIVE_MSG_ERR;
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

				return status_t::OK;
			}

			status_t Client::set_win_clock(time_point_t tm) // sysinfoapi.h
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
					return status_t::OK;
				}			
				else
				{
					auto err = GetLastError();
					if (err == 1314)
						return status_t::ADMIN_RIGHTS_NEEDED;
					else
						return status_t::SET_WIN_TIME_ERR;
				}			
			}
		}
	}
}