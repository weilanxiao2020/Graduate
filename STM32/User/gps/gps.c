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
	Gps_Data_Print();
}

// extern GpsData gpsData;
void Gps_Data_Parse(void)
{
    char *subString;
	char *subStringNext;
	char i = 0;
	if (gpsData->isGetData)
	{
		gpsData->isGetData = false;
        Debug_Info(Gps_TAG, "start parse gps data");
        Debug_Info(Gps_TAG, gpsData->GPS_Buffer);

		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(gpsData->GPS_Buffer, ",")) == NULL)
                {
                    Debug_Error(Gps_TAG, "parse gps data error");	//解析错误
                    break;
                }
					
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(gpsData->UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(gpsData->latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(gpsData->N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(gpsData->longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(gpsData->E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					gpsData->isParseData = true;
					if(usefullBuffer[0] == 'A')
						gpsData->isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						gpsData->isUsefull = false;

				}
				else
				{
                    Debug_Error(Gps_TAG, "parse gps data error");	//解析错误
                    break;
				}
			}
		}
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



