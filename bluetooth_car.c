#include <reg51.h>

sbit LEFT1 = P0^2;
sbit LEFT2 = P0^3;
sbit RIGHT1 = P0^0;
sbit RIGHT2 = P0^1;

sbit BUZZ = P1^0;
sbit LIGHT_LEFT = P1^1;
sbit LIGHT_RIGHT = P1^2;
sbit LIGHT = P1^3;

#define PWM_LEFT 35		// 越大，越快
#define PWM_RIGHT 35	// 越大，越快
#define PWM 25			// 越大，越慢

unsigned char Temp = 0;

void ConfigUART(unsigned int baud)		
{
	SCON = 0X50;
	TMOD &= 0X0F;
	TMOD |= 0X20;		// 模式2 , TL溢出, TH填充
	TH1 = 256 - (11059200 / 12 / 32) / baud;
	TL1 = TH1;
	ET1 = 0;	// 禁止T1中断
	ES = 1;
	TR1 = 1;
}


void Delay(unsigned char x)
{
	unsigned char i;
	unsigned char j;
	for(i = 5; i > 0; i--)
	{
		for(j = x; j > 0; j--)
		{
			;
		}
	}
	
}

void Go()
{
	LEFT1 = 0;
	LEFT2 = 1;
	Delay(PWM_LEFT);
	LEFT1 = 1;
	LEFT2 = 1;
	
	RIGHT1 = 0;
	RIGHT2 = 1;
	Delay(PWM_RIGHT);
	RIGHT1 = 1;
	RIGHT2 = 1;
	
	/*
	LEFT1 = 1;
	LEFT2 = 1;
	RIGHT1 = 1;
	RIGHT2 = 1;
	*/
	
	Delay(PWM);
}

void Back()
{
	LEFT1 = 1;
	LEFT2 = 0;
	Delay(PWM_LEFT);
	LEFT1 = 1;
	LEFT2 = 1;
	
	RIGHT1 = 1;
	RIGHT2 = 0;
	Delay(PWM_RIGHT);
	RIGHT1 = 1;
	RIGHT2 = 1;
	
	Delay(PWM);
}

void Left()
{
	LEFT1 = 1;
	LEFT2 = 0;
	Delay(PWM_LEFT);
	LEFT1 = 1;
	LEFT2 = 1;
	
	RIGHT1 = 0;
	RIGHT2 = 1;
	Delay(PWM_RIGHT);
	RIGHT1 = 1;
	RIGHT2 = 1;
	
	Delay(PWM);
	
	LEFT1 = 1;
	LEFT2 = 1;
	RIGHT1 = 1;
	RIGHT2 = 1;
	
	Delay(PWM);
}

void Right()
{
	LEFT1 = 0;
	LEFT2 = 1;
	Delay(PWM_LEFT);
	LEFT1 = 1;
	LEFT2 = 1;
	
	RIGHT1 = 1;
	RIGHT2 = 0;
	Delay(PWM_RIGHT);
	RIGHT1 = 1;
	RIGHT2 = 1;

	Delay(PWM);
}

void Down()
{
	LEFT1 = 1;
	LEFT2 = 1;
	RIGHT1 = 1;
	RIGHT2 = 1;
	
	LIGHT_LEFT = 1;
	LIGHT_RIGHT = 1;
}

main()
{
	EA = 1;
	ConfigUART(9600);
	
	LEFT1 = 1;
	LEFT2 = 1;
	RIGHT1 = 1;
	RIGHT2 = 1;
	
	while(1)
	{
		switch(Temp)
		{
			case 0x1F:
				Go();		break;
			case 0x2F:
				Back();		break;
			case 0x3F:
				Left(); LIGHT_LEFT = 0;		break;
			case 0x4F:
				Right(); LIGHT_RIGHT = 0;	break;
			case 0x00:
				Down();		break;
			
			case 0xA0:
				LIGHT = 0;	break;
			case 0xB0:
				LIGHT = 1;	break;
			case 0xC0:
				BUZZ = 0;	break;
			case 0xD0:
				BUZZ = 1;	break;
				
		}
		Down();
	}
}

void UART() interrupt 4	
{
	RI = 0;
	Temp = SBUF;
}
