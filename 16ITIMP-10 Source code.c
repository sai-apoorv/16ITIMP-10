#include<reg51.h>

#define Ldata P1
void delay(unsigned int time);
void lcd_cmd(unsigned char ch);
void lcd_data(unsigned char ch);
void lcdprint(unsigned char lcdstring[]);
void serialprint(unsigned char serial[]);

sbit rs=P0^0;
sbit rw=P0^1;
sbit en=P0^2;

sbit m1=P3^2;
sbit m2=P3^3;
sbit light=P3^4;

sbit DHT11=P3^7;		
sbit soilsensor=P3^6;

int I_RH,D_RH,I_Temp,D_Temp,CheckSum; 


void Request()			
{
	DHT11 = 0;		
	delay(50);	
	DHT11 = 1;		
}
void Response()			
{
	while(DHT11==1);
	while(DHT11==0);
	while(DHT11==1);
}

int Receive_data()		
{
	int q,c=0;	
	for (q=0; q<8; q++)
	{
		while(DHT11==0);
		delay(50);
		if(DHT11 == 1)	
		c = (c<<1)|(0x01);
		else		
		c = (c<<1);
		while(DHT11==1);
	}
	return c;
}

void main()
{
	unsigned char dat[20];
	int d1,d2;
	light=0;
	fan=0;
	rw=0;
	
	lcd_cmd(0x38);
	lcd_cmd(0x01);
	lcd_cmd(0x06);
	lcd_cmd(0x0c);
	lcd_cmd(0x80);
	lcdprint("IOT BASED GREEN ");
	lcd_cmd(0xc0);
	lcdprint("HOUSE MONITORING");
	delay(500);
	lcd_cmd(0x01);
	while(1)
	{	
		Request();	
		Response();	
		I_RH=Receive_data();	
		D_RH=Receive_data();	
		I_Temp=Receive_data();
		D_Temp=Receive_data();	
		CheckSum=Receive_data();
		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			lcdprint("ERR");
		}
		else
		{
			TMOD=0x20;
			SCON=0x50;
			TH1=-3;
			TR1=1;
	                d1=I_Temp/10;
			d2=I_Temp%10;
			lcd_cmd(0x80);
			lcdprint("Tem:");
			lcd_data(d1);
			lcd_data(d2);
			lcd_data(0xDF);
			lcd_data('C');
			lcd_cmd(0x89);
			lcdprint("Hum:");
			d1=I_RH/10;
			d2=I_RH%10;
			lcd_data(d1);
			lcd_data(d2);
			lcd_data('%');
			lcd_cmd(0xc4);
			https://web-chat.global.assistant.watson.cloud.ibm.com/preview.html?region=eu-gb&integrationID=19b1c456-565a-4077-8ef9-8a665d391bd7&serviceInstanceID=2febb893-6020-4cee-832c-a7ed2488fe40
			lcdprint("SOIL:");
                        serialprint("<h1>IOT BASED GREEN HOUSE MONITORING<h1>");
			if(I_Temp>=38)
			{
				fan=1;
				serialprint("<h2>Temprature:High  Fan: ON<h2>");
                        }
			if(I_Temp<38)
			{
				fan=0;
				serialprint("<h2>Temprature:Normal Fan:OFF<h2>");
                        }
			if(soilsensor==1)
			{
				light=1;
				lcd_cmd(0xC9);
                                lcdprint("DRY");
				serialprint("<h2>SOIL DRY: MOTOR ON<h2>");
                        }	
			if(soilsensor==0)
			{
				light=0;
				lcd_cmd(0xC9);
				lcdprint("WET");
				serialprint("<h2>SOIL WET :MOTOR OFF<h2>");
                        }	
			        delay(200);

		}		
	}	
}
void lcdprint(unsigned char lcdstring[])
{
	int i;
	for(i=0;lcdstring[i]!= '\0';i++)
	{
		lcd_data(lcdstring[i]);
        }
}
void lcd_cmd(unsigned char ch)
		{
			Ldata=ch;
			rs=0;
			en=1;
			delay(10);
			en=0;
		}
void lcd_data(unsigned char ch)
		{
			Ldata=ch;
			rs=1;
			en=1;
			delay(10);
			en=0;
		}
void delay(unsigned int time)
{
	int i,j;
	for(i=0;i<time;i++)
	for(j=0;j<12750;j++);
}
void serialprint(unsigned char serial[])
{
	int i;
	for(i=0;serial[i]!= '/0';i++)
	{
		SBUF=serial[i];
		while(TI==0);
		TI=0;
        }
}
