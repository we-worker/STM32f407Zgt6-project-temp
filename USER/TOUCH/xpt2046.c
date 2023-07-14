#include "xpt2046.h" 
#include "lcd.h"
#include "stdlib.h"
 


/*********************************************************************************
*********************�������� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: xpt2046.c                                                            *
* �ļ�������������������������                                                   *
* �������ڣ�2018.08.30                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����                                                                     * 
**********************************************************************************
*********************************************************************************/	 
	
u16 Xdown=0; 		 
u16 Ydown=0;	     //�����������¾ͷ��صĵ�����ֵ
u16 Xup=0;
u16 Yup=0; 			   //������������֮��̧�𷵻صĵ�����ֵ 

/**************Ĭ��Ϊ��������***************/
u8  CMD_RDX=0XD0;
u8  CMD_RDY=0X90;

float xFactor=0.06671114;	 //����У׼���� 
float yFactor=0.09117551;
short xOffset=-11;       //xOffset��yOffset ���Ը���ʵ����ͷ�ĵ������ʵ���������׼
short yOffset=-18;
/*******************************************/

/**************����Ϊ��ͨIOģ��SPIͨ��XPT2046***************/
/****************************************************************************
* ��    ��: void SPI_Write_Byte(u8 num) 
* ��    �ܣ�SPIд����,������ICд��1byte���� 
* ��ڲ�����num:Ҫд�������
* ���ز�������
* ˵    ����       
****************************************************************************/
void SPI_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 
		delay_us(1);
		TCLK=1;		 //��������Ч	        
	}		 			    
} 	

/****************************************************************************
* ��    ��: u16 SPI_Read_AD(u8 CMD)
* ��    �ܣ�SPI������ ,�Ӵ�����IC��ȡadcֵ
* ��ڲ�����CMD:ָ��
* ���ز���: ����������	   
* ˵    ����       
****************************************************************************/  
u16 SPI_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;		//������ʱ�� 	 
	TDIN=0; 	//����������
	TCS=0; 		//ѡ�д�����IC 2046
	SPI_Write_Byte(CMD);//����������
	delay_us(6); 
	TCLK=0; 	     	    
	delay_us(1);    	   
	TCLK=1;		 
	delay_us(1);    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	 //�½�����Ч  	    	   
		delay_us(1);    
 		TCLK=1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//��12λ����Ч
	TCS=1;		  //�ͷ�Ƭѡ	 
	return(Num);   
}
/**************����Ϊ��ͨIOģ��SPIͨ��XPT2046***************/


/****************************************************************************
* ��    ��: u16 RTouch_Read_XorY(u8 xy)
* ��    �ܣ���ȡһ������ֵ(x����y)
* ��ڲ�����xoy:ָ�CMD_RDX/CMD_RDY
* ���ز���: ����������	   
* ˵    ����������ȡ5������,����Щ������������, Ȼ��ȥ����ͺ���1����,ȡƽ��ֵ       
****************************************************************************/ 
u16 RTouch_Read_XorY(u8 xoy)
{
	u16 i, j;
	u16 buf[5];
	u16 sum=0;
	u16 temp;
	
	for(i=0;i<5;i++)buf[i] = SPI_Read_AD(xoy);		 		    
	for(i=0;i<5-1; i++)  //����
	{
		for(j=i+1;j<5;j++)
		{
			if(buf[i]>buf[j])//���� 
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=1;i<5-1;i++)sum+=buf[i];
	temp=sum/(5-2*1);
	return temp;   
} 

/****************************************************************************
* ��    ��: u8 RTouch_Read_XY(u16 *x,u16 *y)
* ��    �ܣ���ȡx,y����
* ��ڲ�����x,y:��ȡ��������ֵ
* ���ز���: ����ֵ:0,ʧ��;1,�ɹ�   
* ˵    ������Сֵ��������50.       
****************************************************************************/ 
u8 RTouch_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=RTouch_Read_XorY(CMD_RDX);
	ytemp=RTouch_Read_XorY(CMD_RDY);	  												   
	if(xtemp<50||ytemp<50)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}

/****************************************************************************
* ��    ��: u8 RTouch_Read_XY2(u16 *x,u16 *y)
* ��    �ܣ�����2�ζ�ȡ������IC
* ��ڲ�����x,y:��ȡ��������ֵ
* ���ز���: ����ֵ:0,ʧ��;1,�ɹ�   
* ˵    ����        
****************************************************************************/
u8 RTouch_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=RTouch_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=RTouch_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+50)||(x1<=x2&&x2<x1+50))  //ǰ�����β�����+-50��
    &&((y2<=y1&&y1<y2+50)||(y1<=y2&&y2<y1+50)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  


u16 x;
u16 y;
u8 time;
/****************************************************************************
* ��    ��: void XPT2046_Scan(u8 tp)
* ��    �ܣ���������ɨ��	
* ��ڲ�����type:0,��Ļ����;1,�������� 
* ���ز���: ��  
* ˵    ����        
****************************************************************************/
void XPT2046_Scan(u8 type)
{			   
		Xup=0xffff;
    Yup=0xffff;	 
	if(PEN==0)//�а�������
	{
		if(type)RTouch_Read_XY2(&x,&y);//��ȡ��������
		else if(RTouch_Read_XY2(&x,&y))//��ȡ��Ļ����
		{
	 		x=xFactor*x+xOffset;     //�����ת��Ϊ��Ļ����
			y=yFactor*y+yOffset;  
	 	} 	
		Xdown=x;
		Ydown=y;
		
		time++;		   
	}else  //��̧��
	{    
		 if(time>2)
		 {
			 		Xup=x;
		      Yup=y;	 
		 }
		 time=0;
		 Xdown=0xffff;
     Ydown=0xffff;	 
	}
}	

//xpt2046��ʼ��  		    
void XPT2046_Init(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;	  
		
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOB,C,Fʱ��

	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2; //����Ϊ��������
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	 GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	 GPIO_Init(GPIOF, &GPIO_InitStructure); 
	
	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;//����Ϊ�������
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //���ģʽ
	 GPIO_Init(GPIOB, &GPIO_InitStructure); 

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	 GPIO_Init(GPIOA, &GPIO_InitStructure); 	
   
		if(dir_flag)  //���������ת X Y
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	
      xFactor=-0.09195402;	//����У׼���� 
      yFactor=0.06736275;
      xOffset=348;
      yOffset=-19;			
		}
		else				    
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;
      xFactor=0.06671114;	  //����У׼���� 
      yFactor=0.09117551;
      xOffset=-11;  
      yOffset=-18;				
		}										 
}

