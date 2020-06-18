//==============================================================================
//
// Title:		ntp_client_gui
// Purpose:		A short description of the application.
//
// Created on:	5.5.2020 at 11:31:15 by Jakub Parez.
// Copyright:	CTU/VIN
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include "windows.h"
#include <cvirte.h>		
#include <userint.h>
#include "toolbox.h"
#include "asynctmr.h"
#include "ntp_client_gui_ui.h"
#include "ntp_client_gui.h"
#include "ntp_client.h"
#undef GetSystemTime

//==============================================================================
// Constants
#define TIMER_THREAD_SLEEP 	100
#define PTS					200

//==============================================================================
// Prototypes
int CVICALLBACK MainThread (void *functionData);
int CVICALLBACK TimerThread (void *functionData);


//==============================================================================
// Static global variables

static int panelHandle = 0;          /* main panel handle */
static int enabled = 0;			     /* enable state */

static HNTP client; 			  	 /* NTP client object */

static enum Status s;			     /* result status enum */
static struct Result* result;        /* result struct with all data */
static char status_str[100];     	 /* status string from enum container */
static char ip[20];				     /* NTP server IP */
static int interval = 100;		 	 /* interval between querries */

static int quit = 0;		 	 	 /* thread exit flag */
static int threadId;			  	 /* main thread id */
static int threadId2;			  	 /* timer thread id */

static double data_all[PTS] = {0};   /* data buffer for averaging */
static double data_delta[PTS] = {0}; /* data buffer for averaging */
static int data_idx = 0; 		  	 /* buffer current index */
static int data_cnt = 0;		     /* buffer total count */

//==============================================================================
// Static functions
int round_up_to_max_pow(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}
//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	//errChk (panelHandle = LoadPanel (0, "ntp_client_gui.uir", PANEL));
	errChk (panelHandle = MainPANEL(0));
		
	/* create NTP client */
	client = Client__create();
	
	/* display the panel */
	errChk (DisplayPanel (panelHandle));
	
	/* set trace legend names */
	SetTraceAttributeEx (panelHandle, PANEL_STRIPCHART, 1, ATTR_TRACE_LG_TEXT, "Delta [ms]");
	SetTraceAttributeEx (panelHandle, PANEL_STRIPCHART, 2, ATTR_TRACE_LG_TEXT, "Delta avg");
	SetTraceAttributeEx (panelHandle, PANEL_STRIPCHART, 3, ATTR_TRACE_LG_TEXT, "Jitter [ms]");
	
	/* set axis x offset */
	double currTime = 0;
	GetCurrentDateTime(&currTime);
	SetCtrlAttribute(panelHandle, PANEL_STRIPCHART, ATTR_XAXIS_OFFSET, currTime);
	
	/* run MainThread */
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, MainThread, NULL, &threadId);
	
	/* run TimerThread */
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, TimerThread, NULL, &threadId2);
	
	/* run the user interface */
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
	{
		/* set flags */
		enabled = 0;
		quit = 1;
		
		/* close NTP client */
		Client__close(client);	
		
		/* wait for main thread quits */
		CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, threadId, 0);
		CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, threadId2, 0);
		
		/* quit this program */
		QuitUserInterface (0);
	}	
	
	return 0;
}

int CVICALLBACK callEnable (int panel, int control, int event, 
		void *callbackData, int eventData1, int eventData2)
{	
	if (event != EVENT_COMMIT) 
		return 0;		

	/* toggle flag */
	int _enabled = !enabled;
	
	/* toggle dimmed state */
	SetCtrlAttribute(panelHandle, PANEL_CTRL_IP, ATTR_DIMMED, _enabled);
	SetCtrlAttribute(panelHandle, PANEL_CTRL_INTERVAL, ATTR_DIMMED, _enabled);
	
	if (_enabled)
	{
		/* set control properties */
		SetCtrlVal(panelHandle, PANEL_CTRL_STATUS, "Init");
		SetCtrlAttribute(panelHandle, PANEL_CTRL_STATUS, ATTR_TEXT_COLOR, 0xFFA500L);
		GetCtrlVal(panelHandle, PANEL_CTRL_IP, ip);
		GetCtrlVal(panelHandle, PANEL_CTRL_INTERVAL, &interval);
		
		/* adjust stripchart time values to match current time offset and update interval */
		double currTime = 0;
		GetCurrentDateTime(&currTime);
		SetCtrlAttribute(panelHandle, PANEL_STRIPCHART, ATTR_XAXIS_OFFSET, currTime);
		SetCtrlAttribute(panelHandle, PANEL_STRIPCHART, ATTR_XAXIS_GAIN, interval / 1000.0);
		ClearStripChart(panelHandle, PANEL_STRIPCHART);
		
		/* clear max buffer */
		data_idx = 0;
		data_cnt = 0;
	}
	else
	{
		/* set control propeties */
		SetCtrlVal(panelHandle, PANEL_CTRL_STATUS, "Disabled");
		SetCtrlAttribute(panelHandle, PANEL_CTRL_STATUS, ATTR_TEXT_COLOR, VAL_BLACK);
	}
	
	/* finally enable / disable timer and main thread via flag */
	enabled = _enabled;

	return 0;
}

int CVICALLBACK MainThread (void *functionData)
{
	while (!quit) 
	{
		if (enabled && client != NULL)
		{
			/* set Wait status */
			SetCtrlVal(panelHandle, PANEL_CTRL_STATUS, "Wait Rx");
			SetCtrlAttribute(panelHandle, PANEL_CTRL_STATUS, ATTR_TEXT_COLOR, 0xFFA500L);
		
			/* main NTP server query */
		    s = Client__query_and_sync(client, ip, &result);    		

			/* if everything is fine */
		    if (s == OK) 
			{  
				char offset_str[50];
				char delay_str[50];
				char jitter_str[50];
				char delta_str[50];
				
				/* get metrics strings */
				sprintf(offset_str, "%.3f", result->mtr.offset_ns / 1000000.0);
				sprintf(delay_str, "%.3f", result->mtr.delay_ns / 1000000.0);
				sprintf(jitter_str, "%.3f", result->mtr.jitter_ns / 1000000.0);
				sprintf(delta_str, "%.3f", result->mtr.delta_ns / 1000000.0);
					
				/* adjust strip chart properties */
				SetCtrlAttribute(panelHandle, PANEL_CTRL_STATUS, ATTR_TEXT_COLOR, VAL_GREEN);
		        SetCtrlVal(panelHandle, PANEL_CTRL_OFFSET, offset_str);
				SetCtrlVal(panelHandle, PANEL_CTRL_DELAY, delay_str);
				SetCtrlVal(panelHandle, PANEL_CTRL_JITTER, jitter_str);
				SetCtrlVal(panelHandle, PANEL_CTRL_DELTA, delta_str);	
								
				/* get max of delta and jitter, ceil it to tenth, put in circular buffer and find max */
				double points[3];
				points[0] = result->mtr.delta_ns / 1000000.0;
				points[2] = result->mtr.jitter_ns / 1000000.0;
				double max = points[0];
				
				if (points[2] > max)
					max = points[2];
				
				/* insert data into buffers */
				data_all[data_idx] = (double)round_up_to_max_pow(ceil(max), 2);
				data_delta[data_idx] = points[0];
				
				/* shift helper vars */
				if (data_idx == 199)
					data_idx = 0;
				else
					data_idx++;
				
				if (data_cnt < 200)
					data_cnt++;
				
				/* find max and avg from circular buffer */
				max = 0;
				double avg = 0;
				for (int i = 0; i < data_cnt; i++)
				{
					if (data_all[i] > max)
						max = (double)data_all[i];
					avg += (double)data_delta[i];
				}
				avg /= data_cnt;
									
				/* set chart limits based on averaged max */
				SetAxisScalingMode(panelHandle, PANEL_STRIPCHART, VAL_LEFT_YAXIS , VAL_MANUAL, 0.0, max);
				
				/* plot stripchart points */
				points[1] = avg;
				PlotStripChart(panelHandle, PANEL_STRIPCHART, points, 3, 0, 0, VAL_DOUBLE);
		    }
		    else /* if there was an error */
			{           
				/* adjust strip chart properties */
				
				SetCtrlAttribute(panelHandle, PANEL_CTRL_STATUS, ATTR_TEXT_COLOR, VAL_RED);
				/*
				SetCtrlVal(panelHandle, PANEL_CTRL_ENABLE, 0);	
				callEnable(0, 0, EVENT_COMMIT, NULL, 0, 0);
				SetCtrlVal(panelHandle, PANEL_CTRL_OFFSET, "0");
				SetCtrlVal(panelHandle, PANEL_CTRL_DELAY, "0");
				SetCtrlVal(panelHandle, PANEL_CTRL_JITTER, "0");
				SetCtrlVal(panelHandle, PANEL_CTRL_DELTA, "0");
				*/
		    } 
			
			/* convert status enum to string and print it */
			Client__get_status_str(s, status_str);
		    SetCtrlVal(panelHandle, PANEL_CTRL_STATUS, status_str);
			
			/* finally delete result - dynamic object */
			Client__free_result(result); 
		}
		Sleep(interval);
    }

	return 0;
}

int CVICALLBACK TimerThread (void *functionData)
{
	while (!quit) 
	{
		SYSTEMTIME sdktime; 
		int hour, min, sec, msec;

		/* get system time */
		GetLocalTime (&sdktime);

		hour = sdktime.wHour;
		min = sdktime.wMinute;
		sec = sdktime.wSecond;
		msec = sdktime.wMilliseconds;

		/* print it */
		char time_str[50];
		sprintf(time_str, "%02d:%02d:%02d.%03d", hour, min, sec, msec);
		SetCtrlVal(panelHandle, PANEL_CTRL_TIME, time_str);
		
		Sleep(TIMER_THREAD_SLEEP);
	}
	
	return 0;
}