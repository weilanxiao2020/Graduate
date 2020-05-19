#include "gps.h"

GpsData* gpsData;

void Gps_Init(void)
{
    Gps_Gpio_Init();
    Gps_Usart_Init();
    Gps_Usart_NVIC();
	gpsData = Gps_Buf_Init();
	Debug_Info(Gps_TAG, "gps running...");
    Gps_Data_Clear();
}

void Gps_Data_Get(void)
{
	Gps_Data_Parse();
	// Gps_Data_Print();
}

GpsData* Gps_Data_Buf(void)
{
	Debug_Info(Gps_TAG, "get gps data");
	return gpsData;
}

// extern GpsData gpsData;
void Gps_Data_Parse(void)
{
    char *subString;
	char *subStringNext;
	char len = 0;
	char usefullBuffer[2];
	if (gpsData->isGetData)
	{
		gpsData->isGetData = false;
        Debug_Info(Gps_TAG, "start parse gps data");
        Debug_Info(Gps_TAG, gpsData->GPS_Buffer);

		len = sscanf(gpsData->GPS_Buffer, 
				"$GPRMC,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",
				gpsData->UTCTime, usefullBuffer, gpsData->latitude, gpsData->N_S,
				gpsData->longitude, gpsData->E_W);

		if(len==6) {
			gpsData->isParseData = true;
			if(usefullBuffer[0] == 'A')
				gpsData->isUsefull = true;
			else if(usefullBuffer[0] == 'V')
				gpsData->isUsefull = false;
		} else {
			gpsData->isParseData = false;
			Debug_Error(Gps_TAG, "parse gps data error");
		}
	} else {
		gpsData->isUsefull = false;
	}
}

void Gps_Data_Print(void)
{
	if (gpsData->isParseData)
	{
		gpsData->isParseData = false;
		
		printf("gpsData.UTCTime = ");
		printf(gpsData->UTCTime);
		printf("\r\n");

		if(gpsData->isUsefull)
		{
			gpsData->isUsefull = false;
			printf("gpsData.latitude = ");
			printf(gpsData->latitude);
			printf("\r\n");


			printf("gpsData.N_S = ");
			printf(gpsData->N_S);
			printf("\r\n");

			printf("gpsData.longitude = ");
			printf(gpsData->longitude);
			printf("\r\n");

			printf("gpsData.E_W = ");
			printf(gpsData->E_W);
			printf("\r\n");
		}
		else
		{
			printf("gps data is not usefull!\r\n");
		}
		
	}
}



