#include "common.h"
#include "AD9959_Outset.h"
#include "AD9959.h"

//--------Í¨µÀ¼Ä´æÆ÷µØÖ·ºê¶¨Òå---------------//
#define CSR 0x00               //Í¨µÀÑ¡Ôñ¼Ä´æÆ÷
#define FR1 0x01               //¹¦ÄÜ¼Ä´æÆ÷1
#define FR2 0x02               //¹¦ÄÜ¼Ä´æÆ÷2
#define CFR 0x03               //Í¨µÀ¹¦ÄÜ¼Ä´æÆ÷
#define CFTW0 0x04             //32Î»Í¨µÀÆµÂÊ×ª»»×Ö¼Ä´æÆ÷
#define CPOW0 0x05             //14Î»Í¨µÀÏàÎ»×ª»»×Ö¼Ä´æÆ÷
#define ACR 0x06               //·ù¶È¿ØÖÆ¼Ä´æÆ÷
#define SRR 0x07               //ÏßĞÔÉ¨Ãè¶¨Ê±Æ÷
#define RDW 0x08               //ÏßĞÔÏòÉÏÉ¨Ãè¶¨Ê±Æ÷
#define FDW 0x09               //ÏßĞÔÏòÏÂÉ¨Ãè¶¨Ê±Æ÷



#define uchar unsigned char
#define uint unsigned int



	void AD9959_enablechannel0(void)                    //Í¨µÀ0Ê¹ÄÜ
	{
		uchar ChannelSelectRegisterdata0[1] =  {0x10};   // Í¨µÀÑ¡Ôñ¼Ä´æÆ÷£¬3Ïß´«Êä£¬Êı¾İ¸ßÎ»ÓÅÏÈ
																				
	  WriteToAD9959ViaSpi(CSR,1,ChannelSelectRegisterdata0,0); 
	}
	
	void AD9959_enablechannel1(void)                   //Í¨µÀ1Ê¹ÄÜ
	{
		uchar ChannelSelectRegisterdata1[1] = {0x20};   // Í¨µÀÑ¡Ôñ¼Ä´æÆ÷£¬3Ïß´«Êä£¬Êı¾İ¸ßÎ»ÓÅÏÈ 
																								  // SDIO0 Êı¾İÊäÈë and SDIO2 Êı¾İÊä³ö
		WriteToAD9959ViaSpi(CSR,1,ChannelSelectRegisterdata1,0); 
	}
	void AD9959_enablechannel2(void)                   //Í¨µÀ1Ê¹ÄÜ
	{
		uchar ChannelSelectRegisterdata2[1] = {0x40};   // Í¨µÀÑ¡Ôñ¼Ä´æÆ÷£¬3Ïß´«Êä£¬Êı¾İ¸ßÎ»ÓÅÏÈ 
																								  // SDIO0 Êı¾İÊäÈë and SDIO2 Êı¾İÊä³ö
		WriteToAD9959ViaSpi(CSR,1,ChannelSelectRegisterdata2,0); 
	}
	void AD9959_enablechannel3(void)                   //Í¨µÀ1Ê¹ÄÜ
	{
		uchar ChannelSelectRegisterdata3[1] = {0x80};   // Í¨µÀÑ¡Ôñ¼Ä´æÆ÷£¬3Ïß´«Êä£¬Êı¾İ¸ßÎ»ÓÅÏÈ 
																									  // SDIO0 Êı¾İÊäÈë and SDIO2 Êı¾İÊä³ö
		WriteToAD9959ViaSpi(CSR,1,ChannelSelectRegisterdata3,0); 
	}
	
	
	
  void AD9959_Setwavefrequency (double f)  	                       //Êä³öµ¥¸öÆµÂÊ
 { 
	 uchar ChannelFrequencyTuningWord1data[4];  
   uchar ChannelFunctionRegisterdata[3] = {0x00,0x23,0x35};        //µ¥ÆµÄ£Ê½Ê±Ñ¡Ôñ£¬ÆôÓÃÕıÏÒ¹¦ÄÜ£¨sine£©
 	 uchar FunctionRegister1data[3] = {0xD0,0x00,0x00};  
	 
	 		
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬Ä¿µÄÊÇÎªÁËÊµÏÖÕıÓàÏÒ¹¦ÄÜ
																													         //£¨Ã»ÓĞÒªÇóÊ±¿ÉÒÔ²»ÉèÖÃ¸Ã¼Ä´æÆ÷Í¬ÑùÒ²¿ÉÒÔÕı³£Êä³ö£©
																													         //¼Ä´æÆ÷8¡¢9Î»¿ØÖÆÊä³ö·ù¶È£¬·ÖËÄ¸öµµÎ»
   WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord1data);   //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord1data,1); //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö	
 }

  void AD9959_Setwavephase(double f,int p)													//Êä³öÏàÎ»³ÌĞò¿Éµ÷
 {
	 uchar ChannelPhaseOffsetTuningWorddata[2];
	 uchar ChannelFrequencyTuningWorddata[4];
	 
   uchar ChannelFunctionRegisterdata[3] = {0x00,0x23,0x35};        //µ¥ÆµÄ£Ê½Ê±Ñ¡Ôñ£¬ÆôÓÃÕıÏÒ¹¦ÄÜ£¨sine£©
   uchar FunctionRegister1data[3] = {0xD0,0x00,0x00};   
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬Ä¿µÄÊÇÎªÁËÊµÏÖÕıÓàÏÒ¹¦ÄÜ
	 
	 WrPhaseOffsetTuningWorddata(p,ChannelPhaseOffsetTuningWorddata); //ÉèÖÃÏàÎ»×ª»»×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWorddata,0); //Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
  
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWorddata);  	  //ÉèÖÃÆµÂÊ×ª»»×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWorddata,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
	 
	 
 }	
 
  void AD9959_Setwaveamplitute(double f,int a)                     //Êä³ö·ù¶È¿É×Ô¶¨Òåµ÷½Ú
 {
   uchar ChannelFrequencyTuningWorddata[4];
	 uchar ASRAmplituteWordata[3];
	 uchar AmplitudeControldata[3] = {0xff,0x17,0xff};               //ÊÖ¶¯¿ØÖÆÊä³ö·ù¶È
	 uchar ChannelFunctionRegisterdata[3] = {0x00,0x23,0x35};        //µ¥ÆµÄ£Ê½Ê±Ñ¡Ôñ£¬ÆôÓÃÕıÏÒ¹¦ÄÜ£¨sine£©
	 
   uchar FunctionRegister1data[3] = {0xD0,0x00,0x00}; 
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬Ä¿µÄÊÇÎªÁËÊµÏÖÕıÓàÏÒ¹¦ÄÜ
	 WrAmplitudeTuningWorddata1(a,AmplitudeControldata,ASRAmplituteWordata) ;
	 WriteToAD9959ViaSpi(ACR,3,ASRAmplituteWordata,0);              //ÉèÖÃÊä³ö·ù¶È¿ØÖÆÄ£Ê½
	
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWorddata);   //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWorddata,1); //Ğ´ÈëÆµÂÊ×ª»»×Ö
 }
 //ÉèÖÃ¶ş½×µ÷ÖÆ
 //////////////////////////////////////////////////////
  void AD9959_SetFremodulation2(double f1,double f2)
 {  
// 	 u8 a2 = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0x00,0x00}; 

   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷ 	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	   //ÉèÖÃÆğÊ¼ÆµÂÊS0£¨0x0A£©

	 WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);   //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
	 	
	  //ÉèÖÃÖÕÖ¹ÆµÂÊE0£¨0x04£©

   WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord1data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

 }

  void AD9959_SetPhamodulation2(double f,int p1,int p2)              //p1ÆğÊ¼ÏàÎ»£¬p2ÖÕÖ¹ÏàÎ»
 {
// 	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[2];
	 uchar ChannelPhaseOffsetTuningWord1data[2];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x34};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//¶ş¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0x00,0x00}; 
// 	 uchar FunctionRegister1data[3] = {0xD0,0x54,0x00};    						//¶ş¼¶µ÷ÖÆÊ±Ğè½øĞĞ·ù¶ÈRU/RDÊ±Ñ¡Ôñ
																																			//PO½øĞĞµ÷Æµ£¬P2½øĞĞ·ù¶ÈRU/RD
// 	 uchar AmplitudeControldata[3] = {0xff,0x1f,0xff}; 								//¿ªÆôRU/RD
		 
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯
//    WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0);    					//ĞèÊä³ö·ù¶ÈRU/RDÄ£Ê½²ÅÑ¡Ôñ,ÇÒÒªĞŞ¸Ä¹¦ÄÜ¼Ä´æÆ÷FR1
	  //ÉèÖÃµØÖ·0x05£¨CPOW0£© µÄÏàÎ»

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,0);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 //ÉèÖÃµØÖ·0x0a£¨CW1£© µÄÏàÎ»

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,0);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	// f=2000000;    //ÉèÖÃÊä³öÆµÂÊ
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord1data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord1data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö	
 }
 
  void AD9959_SetAM2(double f)
 { 
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelAmplitudeTuningWorddata[4]={0x1f,0x30,0x00,0x00}; //72mV
	 uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};  //500mV 
	 uchar FunctionRegister1data[3]={0xD0,0x00,0x00}; 
	
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷
	
	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©
   
   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWorddata,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©
																																		//ÉèÖÃÊä³öÆµÂÊ´óĞ¡
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
	 
 }
 
 /////////////////////////////////////////////////////////////////////
 
 //ËÄ½×µ÷ÖÆ   CH0/1      
 //////////////////////////////////////////////////////////////////////
 void AD9959_SetFremodulation4(double f1,double f2,double f3,double f4)
 {
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0x01,0x00};  	 
  
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷


   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
 void AD9959_SetPhamodulation4(double f,int p1,int p2,int p3,int p4)
 {
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//ËÄ¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0x01,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯
	 
	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	
	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
 }
 void AD9959_SetAM4(double f)
 {
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0x8f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0x5f,0x30,0x00,0x00};
	 
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0x01,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}

/////////////////////////////////////////////////////////////////////////
//  ËÄ½×µ÷ÖÆ CH2/3    
/////////////////////////////////////////////////////////////////////////

void AD9959_SetFremodulation42(double f1,double f2,double f3,double f4)
 {
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0x51,0x00};  	 
  
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
 }
 void AD9959_SetPhamodulation42(double f,int p1,int p2,int p3,int p4)
 {
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//ËÄ¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0x51,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
 }
 void AD9959_SetAM42(double f)
 {
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0x8f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0x5f,0x30,0x00,0x00};
	 
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0x51,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}

/////////////////////////////////////////////////////////////////////////
//  °Ë½×µ÷ÖÆ CH0  
/////////////////////////////////////////////////////////////////////////

void AD9959_SetFremodulation80(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8)
{
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFrequencyTuningWord4data[4];
	 uchar ChannelFrequencyTuningWord5data[4];
	 uchar ChannelFrequencyTuningWord6data[4];
	 uchar ChannelFrequencyTuningWord7data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0xc2,0x00};  	 
 
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f5,ChannelFrequencyTuningWord4data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0D,4,ChannelFrequencyTuningWord4data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f6,ChannelFrequencyTuningWord5data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0E,4,ChannelFrequencyTuningWord5data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f7,ChannelFrequencyTuningWord6data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0F,4,ChannelFrequencyTuningWord6data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f8,ChannelFrequencyTuningWord7data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x10,4,ChannelFrequencyTuningWord7data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetPhamodulation80(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8)
{
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 uchar ChannelPhaseOffsetTuningWord4data[4];
	 uchar ChannelPhaseOffsetTuningWord5data[4];
	 uchar ChannelPhaseOffsetTuningWord6data[4];
	 uchar ChannelPhaseOffsetTuningWord7data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//°Ë¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0xc2,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	
	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p5,ChannelPhaseOffsetTuningWord4data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0d,4,ChannelPhaseOffsetTuningWord4data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p6,ChannelPhaseOffsetTuningWord5data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0e,4,ChannelPhaseOffsetTuningWord5data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p7,ChannelPhaseOffsetTuningWord6data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0f,4,ChannelPhaseOffsetTuningWord6data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p8,ChannelPhaseOffsetTuningWord7data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x10,4,ChannelPhaseOffsetTuningWord7data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetAM80(double f)
{
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0xbf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0x9f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord4data[4]={0x7f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord5data[4]={0x5f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord6data[4]={0x3f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord7data[4]={0x1f,0x30,0x00,0x00};
	 
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0xc2,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);

	 WriteToAD9959ViaSpi(0x0d,4,ChannelAmplitudeTuningWord4data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0e,4,ChannelAmplitudeTuningWord5data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0f,4,ChannelAmplitudeTuningWord6data,0);

	 WriteToAD9959ViaSpi(0x10,4,ChannelAmplitudeTuningWord7data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}
/////////////////////////////////////////////////////////////////////////
//  °Ë½×µ÷ÖÆ CH1
/////////////////////////////////////////////////////////////////////////

void AD9959_SetFremodulation81(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8)
{
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFrequencyTuningWord4data[4];
	 uchar ChannelFrequencyTuningWord5data[4];
	 uchar ChannelFrequencyTuningWord6data[4];
	 uchar ChannelFrequencyTuningWord7data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0xd2,0x00};  	 
 
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f5,ChannelFrequencyTuningWord4data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0D,4,ChannelFrequencyTuningWord4data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f6,ChannelFrequencyTuningWord5data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0E,4,ChannelFrequencyTuningWord5data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f7,ChannelFrequencyTuningWord6data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0F,4,ChannelFrequencyTuningWord6data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f8,ChannelFrequencyTuningWord7data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x10,4,ChannelFrequencyTuningWord7data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetPhamodulation81(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8)
{
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 uchar ChannelPhaseOffsetTuningWord4data[4];
	 uchar ChannelPhaseOffsetTuningWord5data[4];
	 uchar ChannelPhaseOffsetTuningWord6data[4];
	 uchar ChannelPhaseOffsetTuningWord7data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//°Ë¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0xd2,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p5,ChannelPhaseOffsetTuningWord4data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0d,4,ChannelPhaseOffsetTuningWord4data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p6,ChannelPhaseOffsetTuningWord5data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0e,4,ChannelPhaseOffsetTuningWord5data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p7,ChannelPhaseOffsetTuningWord6data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0f,4,ChannelPhaseOffsetTuningWord6data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p8,ChannelPhaseOffsetTuningWord7data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x10,4,ChannelPhaseOffsetTuningWord7data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetAM81(double f)
{
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0xbf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0x9f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord4data[4]={0x7f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord5data[4]={0x5f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord6data[4]={0x3f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord7data[4]={0x1f,0x30,0x00,0x00};
	 
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0xd2,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷
	 
	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);

	 WriteToAD9959ViaSpi(0x0d,4,ChannelAmplitudeTuningWord4data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0e,4,ChannelAmplitudeTuningWord5data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0f,4,ChannelAmplitudeTuningWord6data,0);

	 WriteToAD9959ViaSpi(0x10,4,ChannelAmplitudeTuningWord7data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}
/////////////////////////////////////////////////////////////////////////
//  °Ë½×µ÷ÖÆ CH2    
/////////////////////////////////////////////////////////////////////////

void AD9959_SetFremodulation82(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8)
{
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFrequencyTuningWord4data[4];
	 uchar ChannelFrequencyTuningWord5data[4];
	 uchar ChannelFrequencyTuningWord6data[4];
	 uchar ChannelFrequencyTuningWord7data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0xe2,0x00};  	 
 
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f5,ChannelFrequencyTuningWord4data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0D,4,ChannelFrequencyTuningWord4data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f6,ChannelFrequencyTuningWord5data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0E,4,ChannelFrequencyTuningWord5data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f7,ChannelFrequencyTuningWord6data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0F,4,ChannelFrequencyTuningWord6data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f8,ChannelFrequencyTuningWord7data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x10,4,ChannelFrequencyTuningWord7data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetPhamodulation82(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8)
{
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 uchar ChannelPhaseOffsetTuningWord4data[4];
	 uchar ChannelPhaseOffsetTuningWord5data[4];
	 uchar ChannelPhaseOffsetTuningWord6data[4];
	 uchar ChannelPhaseOffsetTuningWord7data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//°Ë¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0xe2,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p5,ChannelPhaseOffsetTuningWord4data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0d,4,ChannelPhaseOffsetTuningWord4data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p6,ChannelPhaseOffsetTuningWord5data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0e,4,ChannelPhaseOffsetTuningWord5data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p7,ChannelPhaseOffsetTuningWord6data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0f,4,ChannelPhaseOffsetTuningWord6data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p8,ChannelPhaseOffsetTuningWord7data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x10,4,ChannelPhaseOffsetTuningWord7data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetAM82(double f)
{
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0xbf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0x9f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord4data[4]={0x7f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord5data[4]={0x5f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord6data[4]={0x3f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord7data[4]={0x1f,0x30,0x00,0x00};
	 
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0xe2,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);

	 WriteToAD9959ViaSpi(0x0d,4,ChannelAmplitudeTuningWord4data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0e,4,ChannelAmplitudeTuningWord5data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0f,4,ChannelAmplitudeTuningWord6data,0);

	 WriteToAD9959ViaSpi(0x10,4,ChannelAmplitudeTuningWord7data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}

/////////////////////////////////////////////////////////////////////////
//  °Ë½×µ÷ÖÆ CH3    
/////////////////////////////////////////////////////////////////////////

void AD9959_SetFremodulation83(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8)
{
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFrequencyTuningWord4data[4];
	 uchar ChannelFrequencyTuningWord5data[4];
	 uchar ChannelFrequencyTuningWord6data[4];
	 uchar ChannelFrequencyTuningWord7data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0xf2,0x00};  	 

   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f5,ChannelFrequencyTuningWord4data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0D,4,ChannelFrequencyTuningWord4data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f6,ChannelFrequencyTuningWord5data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0E,4,ChannelFrequencyTuningWord5data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f7,ChannelFrequencyTuningWord6data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0F,4,ChannelFrequencyTuningWord6data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f8,ChannelFrequencyTuningWord7data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x10,4,ChannelFrequencyTuningWord7data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetPhamodulation83(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8)
{
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 uchar ChannelPhaseOffsetTuningWord4data[4];
	 uchar ChannelPhaseOffsetTuningWord5data[4];
	 uchar ChannelPhaseOffsetTuningWord6data[4];
	 uchar ChannelPhaseOffsetTuningWord7data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//°Ë¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0xf2,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p5,ChannelPhaseOffsetTuningWord4data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0d,4,ChannelPhaseOffsetTuningWord4data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p6,ChannelPhaseOffsetTuningWord5data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0e,4,ChannelPhaseOffsetTuningWord5data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p7,ChannelPhaseOffsetTuningWord6data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0f,4,ChannelPhaseOffsetTuningWord6data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p8,ChannelPhaseOffsetTuningWord7data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x10,4,ChannelPhaseOffsetTuningWord7data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetAM83(double f)
{
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0xbf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0x9f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord4data[4]={0x7f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord5data[4]={0x5f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord6data[4]={0x3f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord7data[4]={0x1f,0x30,0x00,0x00};
	 
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0xf2,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);

	 WriteToAD9959ViaSpi(0x0d,4,ChannelAmplitudeTuningWord4data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0e,4,ChannelAmplitudeTuningWord5data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0f,4,ChannelAmplitudeTuningWord6data,0);

	 WriteToAD9959ViaSpi(0x10,4,ChannelAmplitudeTuningWord7data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}


/////////////////////////////////////////////////////////////////////////
//  Ê®Áù½×µ÷ÖÆ CH0   
/////////////////////////////////////////////////////////////////////////
void AD9959_SetFremodulation160(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8,double f9,double f10,double f11,double f12,double f13,double f14,double f15,double f16)
{
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFrequencyTuningWord4data[4];
	 uchar ChannelFrequencyTuningWord5data[4];
	 uchar ChannelFrequencyTuningWord6data[4];
	 uchar ChannelFrequencyTuningWord7data[4];
	 uchar ChannelFrequencyTuningWord8data[4];
	 uchar ChannelFrequencyTuningWord9data[4];
	 uchar ChannelFrequencyTuningWord10data[4];
	 uchar ChannelFrequencyTuningWord11data[4];
	 uchar ChannelFrequencyTuningWord12data[4];
	 uchar ChannelFrequencyTuningWord13data[4];
	 uchar ChannelFrequencyTuningWord14data[4];
	 uchar ChannelFrequencyTuningWord15data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0xc3,0x00};  	 
  
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f5,ChannelFrequencyTuningWord4data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0D,4,ChannelFrequencyTuningWord4data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f6,ChannelFrequencyTuningWord5data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0E,4,ChannelFrequencyTuningWord5data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f7,ChannelFrequencyTuningWord6data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0F,4,ChannelFrequencyTuningWord6data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f8,ChannelFrequencyTuningWord7data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x10,4,ChannelFrequencyTuningWord7data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f9,ChannelFrequencyTuningWord8data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x11,4,ChannelFrequencyTuningWord8data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f10,ChannelFrequencyTuningWord9data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x12,4,ChannelFrequencyTuningWord9data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f11,ChannelFrequencyTuningWord10data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x13,4,ChannelFrequencyTuningWord10data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f12,ChannelFrequencyTuningWord11data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x14,4,ChannelFrequencyTuningWord11data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f13,ChannelFrequencyTuningWord12data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x15,4,ChannelFrequencyTuningWord12data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f14,ChannelFrequencyTuningWord13data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x16,4,ChannelFrequencyTuningWord13data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f15,ChannelFrequencyTuningWord14data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x017,4,ChannelFrequencyTuningWord14data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f16,ChannelFrequencyTuningWord15data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x18,4,ChannelFrequencyTuningWord15data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetPhamodulation160(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9,int p10,int p11,int p12,int p13,int p14,int p15,int p16)
{
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 uchar ChannelPhaseOffsetTuningWord4data[4];
	 uchar ChannelPhaseOffsetTuningWord5data[4];
	 uchar ChannelPhaseOffsetTuningWord6data[4];
	 uchar ChannelPhaseOffsetTuningWord7data[4];
	 uchar ChannelPhaseOffsetTuningWord8data[4];
	 uchar ChannelPhaseOffsetTuningWord9data[4];
	 uchar ChannelPhaseOffsetTuningWord10data[4];
	 uchar ChannelPhaseOffsetTuningWord11data[4];
	 uchar ChannelPhaseOffsetTuningWord12data[4];
	 uchar ChannelPhaseOffsetTuningWord13data[4];
	 uchar ChannelPhaseOffsetTuningWord14data[4];
	 uchar ChannelPhaseOffsetTuningWord15data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//Ê®Áù¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0xc3,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p5,ChannelPhaseOffsetTuningWord4data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0d,4,ChannelPhaseOffsetTuningWord4data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p6,ChannelPhaseOffsetTuningWord5data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0e,4,ChannelPhaseOffsetTuningWord5data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p7,ChannelPhaseOffsetTuningWord6data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0f,4,ChannelPhaseOffsetTuningWord6data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p8,ChannelPhaseOffsetTuningWord7data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x10,4,ChannelPhaseOffsetTuningWord7data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p9,ChannelPhaseOffsetTuningWord8data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x11,4,ChannelPhaseOffsetTuningWord8data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p10,ChannelPhaseOffsetTuningWord9data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x12,4,ChannelPhaseOffsetTuningWord9data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p11,ChannelPhaseOffsetTuningWord10data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x13,4,ChannelPhaseOffsetTuningWord10data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p12,ChannelPhaseOffsetTuningWord11data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x14,4,ChannelPhaseOffsetTuningWord11data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p13,ChannelPhaseOffsetTuningWord12data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x15,4,ChannelPhaseOffsetTuningWord12data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p14,ChannelPhaseOffsetTuningWord13data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x16,4,ChannelPhaseOffsetTuningWord13data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p15,ChannelPhaseOffsetTuningWord14data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x17,4,ChannelPhaseOffsetTuningWord14data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	
	 WrPhaseOffsetTuningWorddata(p16,ChannelPhaseOffsetTuningWord15data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x18,4,ChannelPhaseOffsetTuningWord15data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetAM160(double f)
{
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xef,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0xcf,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord4data[4]={0xbf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord5data[4]={0xaf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord6data[4]={0x9f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord7data[4]={0x8f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord8data[4]={0x7f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord9data[4]={0x6f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord10data[4]={0x5f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord11data[4]={0x4f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord12data[4]={0x3f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord13data[4]={0x2f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord14data[4]={0x1f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord15data[4]={0x0f,0x30,0x00,0x00};
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0xc3,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);

	 WriteToAD9959ViaSpi(0x0d,4,ChannelAmplitudeTuningWord4data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0e,4,ChannelAmplitudeTuningWord5data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0f,4,ChannelAmplitudeTuningWord6data,0);

	 WriteToAD9959ViaSpi(0x10,4,ChannelAmplitudeTuningWord7data,0);

	 WriteToAD9959ViaSpi(0x11,4,ChannelAmplitudeTuningWord8data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x12,4,ChannelAmplitudeTuningWord9data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x13,4,ChannelAmplitudeTuningWord10data,0);

	 WriteToAD9959ViaSpi(0x14,4,ChannelAmplitudeTuningWord11data,0);

	 WriteToAD9959ViaSpi(0x15,4,ChannelAmplitudeTuningWord12data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x16,4,ChannelAmplitudeTuningWord13data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x17,4,ChannelAmplitudeTuningWord14data,0);

	 WriteToAD9959ViaSpi(0x18,4,ChannelAmplitudeTuningWord15data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}
/////////////////////////////////////////////////////////////////////////
//  Ê®Áù½×µ÷ÖÆ CH1  
/////////////////////////////////////////////////////////////////////////
void AD9959_SetFremodulation161(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8,double f9,double f10,double f11,double f12,double f13,double f14,double f15,double f16)
{
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFrequencyTuningWord4data[4];
	 uchar ChannelFrequencyTuningWord5data[4];
	 uchar ChannelFrequencyTuningWord6data[4];
	 uchar ChannelFrequencyTuningWord7data[4];
	 uchar ChannelFrequencyTuningWord8data[4];
	 uchar ChannelFrequencyTuningWord9data[4];
	 uchar ChannelFrequencyTuningWord10data[4];
	 uchar ChannelFrequencyTuningWord11data[4];
	 uchar ChannelFrequencyTuningWord12data[4];
	 uchar ChannelFrequencyTuningWord13data[4];
	 uchar ChannelFrequencyTuningWord14data[4];
	 uchar ChannelFrequencyTuningWord15data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0xd3,0x00};  	 
 
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f5,ChannelFrequencyTuningWord4data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0D,4,ChannelFrequencyTuningWord4data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f6,ChannelFrequencyTuningWord5data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0E,4,ChannelFrequencyTuningWord5data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f7,ChannelFrequencyTuningWord6data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0F,4,ChannelFrequencyTuningWord6data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f8,ChannelFrequencyTuningWord7data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x10,4,ChannelFrequencyTuningWord7data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f9,ChannelFrequencyTuningWord8data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x11,4,ChannelFrequencyTuningWord8data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
	 
	 WrFrequencyTuningWorddata(f10,ChannelFrequencyTuningWord9data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x12,4,ChannelFrequencyTuningWord9data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f11,ChannelFrequencyTuningWord10data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x13,4,ChannelFrequencyTuningWord10data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f12,ChannelFrequencyTuningWord11data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x14,4,ChannelFrequencyTuningWord11data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f13,ChannelFrequencyTuningWord12data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x15,4,ChannelFrequencyTuningWord12data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f14,ChannelFrequencyTuningWord13data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x16,4,ChannelFrequencyTuningWord13data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f15,ChannelFrequencyTuningWord14data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x017,4,ChannelFrequencyTuningWord14data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f16,ChannelFrequencyTuningWord15data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x18,4,ChannelFrequencyTuningWord15data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetPhamodulation161(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9,int p10,int p11,int p12,int p13,int p14,int p15,int p16)
{
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 uchar ChannelPhaseOffsetTuningWord4data[4];
	 uchar ChannelPhaseOffsetTuningWord5data[4];
	 uchar ChannelPhaseOffsetTuningWord6data[4];
	 uchar ChannelPhaseOffsetTuningWord7data[4];
	 uchar ChannelPhaseOffsetTuningWord8data[4];
	 uchar ChannelPhaseOffsetTuningWord9data[4];
	 uchar ChannelPhaseOffsetTuningWord10data[4];
	 uchar ChannelPhaseOffsetTuningWord11data[4];
	 uchar ChannelPhaseOffsetTuningWord12data[4];
	 uchar ChannelPhaseOffsetTuningWord13data[4];
	 uchar ChannelPhaseOffsetTuningWord14data[4];
	 uchar ChannelPhaseOffsetTuningWord15data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//Ê®Áù¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0xd3,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p5,ChannelPhaseOffsetTuningWord4data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0d,4,ChannelPhaseOffsetTuningWord4data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p6,ChannelPhaseOffsetTuningWord5data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0e,4,ChannelPhaseOffsetTuningWord5data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p7,ChannelPhaseOffsetTuningWord6data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0f,4,ChannelPhaseOffsetTuningWord6data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p8,ChannelPhaseOffsetTuningWord7data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x10,4,ChannelPhaseOffsetTuningWord7data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p9,ChannelPhaseOffsetTuningWord8data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x11,4,ChannelPhaseOffsetTuningWord8data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p10,ChannelPhaseOffsetTuningWord9data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x12,4,ChannelPhaseOffsetTuningWord9data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p11,ChannelPhaseOffsetTuningWord10data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x13,4,ChannelPhaseOffsetTuningWord10data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p12,ChannelPhaseOffsetTuningWord11data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x14,4,ChannelPhaseOffsetTuningWord11data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p13,ChannelPhaseOffsetTuningWord12data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x15,4,ChannelPhaseOffsetTuningWord12data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p14,ChannelPhaseOffsetTuningWord13data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x16,4,ChannelPhaseOffsetTuningWord13data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p15,ChannelPhaseOffsetTuningWord14data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x17,4,ChannelPhaseOffsetTuningWord14data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p16,ChannelPhaseOffsetTuningWord15data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x18,4,ChannelPhaseOffsetTuningWord15data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetAM161(double f)
{
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xef,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0xcf,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord4data[4]={0xbf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord5data[4]={0xaf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord6data[4]={0x9f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord7data[4]={0x8f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord8data[4]={0x7f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord9data[4]={0x6f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord10data[4]={0x5f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord11data[4]={0x4f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord12data[4]={0x3f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord13data[4]={0x2f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord14data[4]={0x1f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord15data[4]={0x0f,0x30,0x00,0x00};
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0xd3,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);

	 WriteToAD9959ViaSpi(0x0d,4,ChannelAmplitudeTuningWord4data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0e,4,ChannelAmplitudeTuningWord5data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0f,4,ChannelAmplitudeTuningWord6data,0);

	 WriteToAD9959ViaSpi(0x10,4,ChannelAmplitudeTuningWord7data,0);

	 WriteToAD9959ViaSpi(0x11,4,ChannelAmplitudeTuningWord8data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x12,4,ChannelAmplitudeTuningWord9data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x13,4,ChannelAmplitudeTuningWord10data,0);

	 WriteToAD9959ViaSpi(0x14,4,ChannelAmplitudeTuningWord11data,0);

	 WriteToAD9959ViaSpi(0x15,4,ChannelAmplitudeTuningWord12data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x16,4,ChannelAmplitudeTuningWord13data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x17,4,ChannelAmplitudeTuningWord14data,0);

	 WriteToAD9959ViaSpi(0x18,4,ChannelAmplitudeTuningWord15data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}

/////////////////////////////////////////////////////////////////////////
//  Ê®Áù½×µ÷ÖÆ CH2 
/////////////////////////////////////////////////////////////////////////
void AD9959_SetFremodulation162(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8,double f9,double f10,double f11,double f12,double f13,double f14,double f15,double f16)
{
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFrequencyTuningWord4data[4];
	 uchar ChannelFrequencyTuningWord5data[4];
	 uchar ChannelFrequencyTuningWord6data[4];
	 uchar ChannelFrequencyTuningWord7data[4];
	 uchar ChannelFrequencyTuningWord8data[4];
	 uchar ChannelFrequencyTuningWord9data[4];
	 uchar ChannelFrequencyTuningWord10data[4];
	 uchar ChannelFrequencyTuningWord11data[4];
	 uchar ChannelFrequencyTuningWord12data[4];
	 uchar ChannelFrequencyTuningWord13data[4];
	 uchar ChannelFrequencyTuningWord14data[4];
	 uchar ChannelFrequencyTuningWord15data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0xe3,0x00};  	 
  
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f5,ChannelFrequencyTuningWord4data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0D,4,ChannelFrequencyTuningWord4data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f6,ChannelFrequencyTuningWord5data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0E,4,ChannelFrequencyTuningWord5data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f7,ChannelFrequencyTuningWord6data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0F,4,ChannelFrequencyTuningWord6data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f8,ChannelFrequencyTuningWord7data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x10,4,ChannelFrequencyTuningWord7data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f9,ChannelFrequencyTuningWord8data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x11,4,ChannelFrequencyTuningWord8data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f10,ChannelFrequencyTuningWord9data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x12,4,ChannelFrequencyTuningWord9data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f11,ChannelFrequencyTuningWord10data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x13,4,ChannelFrequencyTuningWord10data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f12,ChannelFrequencyTuningWord11data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x14,4,ChannelFrequencyTuningWord11data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f13,ChannelFrequencyTuningWord12data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x15,4,ChannelFrequencyTuningWord12data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f14,ChannelFrequencyTuningWord13data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x16,4,ChannelFrequencyTuningWord13data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f15,ChannelFrequencyTuningWord14data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x017,4,ChannelFrequencyTuningWord14data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f16,ChannelFrequencyTuningWord15data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x18,4,ChannelFrequencyTuningWord15data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetPhamodulation162(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9,int p10,int p11,int p12,int p13,int p14,int p15,int p16)
{
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 uchar ChannelPhaseOffsetTuningWord4data[4];
	 uchar ChannelPhaseOffsetTuningWord5data[4];
	 uchar ChannelPhaseOffsetTuningWord6data[4];
	 uchar ChannelPhaseOffsetTuningWord7data[4];
	 uchar ChannelPhaseOffsetTuningWord8data[4];
	 uchar ChannelPhaseOffsetTuningWord9data[4];
	 uchar ChannelPhaseOffsetTuningWord10data[4];
	 uchar ChannelPhaseOffsetTuningWord11data[4];
	 uchar ChannelPhaseOffsetTuningWord12data[4];
	 uchar ChannelPhaseOffsetTuningWord13data[4];
	 uchar ChannelPhaseOffsetTuningWord14data[4];
	 uchar ChannelPhaseOffsetTuningWord15data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//Ê®Áù¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0xe3,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p5,ChannelPhaseOffsetTuningWord4data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0d,4,ChannelPhaseOffsetTuningWord4data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	
	 WrPhaseOffsetTuningWorddata(p6,ChannelPhaseOffsetTuningWord5data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0e,4,ChannelPhaseOffsetTuningWord5data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p7,ChannelPhaseOffsetTuningWord6data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0f,4,ChannelPhaseOffsetTuningWord6data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	
	 WrPhaseOffsetTuningWorddata(p8,ChannelPhaseOffsetTuningWord7data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x10,4,ChannelPhaseOffsetTuningWord7data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p9,ChannelPhaseOffsetTuningWord8data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x11,4,ChannelPhaseOffsetTuningWord8data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p10,ChannelPhaseOffsetTuningWord9data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x12,4,ChannelPhaseOffsetTuningWord9data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p11,ChannelPhaseOffsetTuningWord10data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x13,4,ChannelPhaseOffsetTuningWord10data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p12,ChannelPhaseOffsetTuningWord11data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x14,4,ChannelPhaseOffsetTuningWord11data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p13,ChannelPhaseOffsetTuningWord12data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x15,4,ChannelPhaseOffsetTuningWord12data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p14,ChannelPhaseOffsetTuningWord13data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x16,4,ChannelPhaseOffsetTuningWord13data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p15,ChannelPhaseOffsetTuningWord14data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x17,4,ChannelPhaseOffsetTuningWord14data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p16,ChannelPhaseOffsetTuningWord15data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x18,4,ChannelPhaseOffsetTuningWord15data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetAM162(double f)
{
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xef,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0xcf,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord4data[4]={0xbf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord5data[4]={0xaf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord6data[4]={0x9f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord7data[4]={0x8f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord8data[4]={0x7f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord9data[4]={0x6f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord10data[4]={0x5f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord11data[4]={0x4f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord12data[4]={0x3f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord13data[4]={0x2f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord14data[4]={0x1f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord15data[4]={0x0f,0x30,0x00,0x00};
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0xe3,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);

	 WriteToAD9959ViaSpi(0x0d,4,ChannelAmplitudeTuningWord4data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0e,4,ChannelAmplitudeTuningWord5data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0f,4,ChannelAmplitudeTuningWord6data,0);

	 WriteToAD9959ViaSpi(0x10,4,ChannelAmplitudeTuningWord7data,0);

	 WriteToAD9959ViaSpi(0x11,4,ChannelAmplitudeTuningWord8data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x12,4,ChannelAmplitudeTuningWord9data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x13,4,ChannelAmplitudeTuningWord10data,0);

	 WriteToAD9959ViaSpi(0x14,4,ChannelAmplitudeTuningWord11data,0);

	 WriteToAD9959ViaSpi(0x15,4,ChannelAmplitudeTuningWord12data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x16,4,ChannelAmplitudeTuningWord13data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x17,4,ChannelAmplitudeTuningWord14data,0);

	 WriteToAD9959ViaSpi(0x18,4,ChannelAmplitudeTuningWord15data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}




/////////////////////////////////////////////////////////////////////////
//  Ê®Áù½×µ÷ÖÆ CH3   
/////////////////////////////////////////////////////////////////////////

void AD9959_SetFremodulation163(double f1,double f2,double f3,double f4,double f5,double f6,double f7,double f8,double f9,double f10,double f11,double f12,double f13,double f14,double f15,double f16)
{
// 	 u8 a = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelFrequencyTuningWord1data[4];
	 uchar ChannelFrequencyTuningWord2data[4];
	 uchar ChannelFrequencyTuningWord3data[4];
	 uchar ChannelFrequencyTuningWord4data[4];
	 uchar ChannelFrequencyTuningWord5data[4];
	 uchar ChannelFrequencyTuningWord6data[4];
	 uchar ChannelFrequencyTuningWord7data[4];
	 uchar ChannelFrequencyTuningWord8data[4];
	 uchar ChannelFrequencyTuningWord9data[4];
	 uchar ChannelFrequencyTuningWord10data[4];
	 uchar ChannelFrequencyTuningWord11data[4];
	 uchar ChannelFrequencyTuningWord12data[4];
	 uchar ChannelFrequencyTuningWord13data[4];
	 uchar ChannelFrequencyTuningWord14data[4];
	 uchar ChannelFrequencyTuningWord15data[4];
	 uchar ChannelFunctionRegisterdata[3] = {0x80,0x23,0x30};        //ÎŞRU/RD
	 uchar FunctionRegister1data[3] = {0xD0,0xf3,0x00};  	 
  
   WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷	 
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);       //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

   WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWord0data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x04,4,ChannelFrequencyTuningWord0data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWord1data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWord1data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f3,ChannelFrequencyTuningWord2data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0B,4,ChannelFrequencyTuningWord2data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f4,ChannelFrequencyTuningWord3data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0C,4,ChannelFrequencyTuningWord3data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f5,ChannelFrequencyTuningWord4data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0D,4,ChannelFrequencyTuningWord4data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f6,ChannelFrequencyTuningWord5data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0E,4,ChannelFrequencyTuningWord5data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f7,ChannelFrequencyTuningWord6data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0F,4,ChannelFrequencyTuningWord6data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f8,ChannelFrequencyTuningWord7data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x10,4,ChannelFrequencyTuningWord7data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f9,ChannelFrequencyTuningWord8data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x11,4,ChannelFrequencyTuningWord8data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f10,ChannelFrequencyTuningWord9data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x12,4,ChannelFrequencyTuningWord9data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f11,ChannelFrequencyTuningWord10data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x13,4,ChannelFrequencyTuningWord10data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f12,ChannelFrequencyTuningWord11data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x14,4,ChannelFrequencyTuningWord11data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f13,ChannelFrequencyTuningWord12data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x15,4,ChannelFrequencyTuningWord12data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f14,ChannelFrequencyTuningWord13data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x16,4,ChannelFrequencyTuningWord13data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

	 WrFrequencyTuningWorddata(f15,ChannelFrequencyTuningWord14data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x017,4,ChannelFrequencyTuningWord14data,0);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö

   WrFrequencyTuningWorddata(f16,ChannelFrequencyTuningWord15data);    //½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x18,4,ChannelFrequencyTuningWord15data,1);  //Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetPhamodulation163(double f,int p1,int p2,int p3,int p4,int p5,int p6,int p7,int p8,int p9,int p10,int p11,int p12,int p13,int p14,int p15,int p16)
{
// 	 u8 b = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord0data[4];
	 uchar ChannelPhaseOffsetTuningWord1data[4];
	 uchar ChannelPhaseOffsetTuningWord2data[4];
	 uchar ChannelPhaseOffsetTuningWord3data[4];
	 uchar ChannelPhaseOffsetTuningWord4data[4];
	 uchar ChannelPhaseOffsetTuningWord5data[4];
	 uchar ChannelPhaseOffsetTuningWord6data[4];
	 uchar ChannelPhaseOffsetTuningWord7data[4];
	 uchar ChannelPhaseOffsetTuningWord8data[4];
	 uchar ChannelPhaseOffsetTuningWord9data[4];
	 uchar ChannelPhaseOffsetTuningWord10data[4];
	 uchar ChannelPhaseOffsetTuningWord11data[4];
	 uchar ChannelPhaseOffsetTuningWord12data[4];
	 uchar ChannelPhaseOffsetTuningWord13data[4];
	 uchar ChannelPhaseOffsetTuningWord14data[4];
	 uchar ChannelPhaseOffsetTuningWord15data[4];
	 
	 uchar ChannelFunctionRegisterdata[3] = {0xc0,0x03,0x30};  					//ÏàÎ»µ÷ÖÆÄ£Ê½ÆôÓÃ£¨Á¬ĞøÉ¨Ãè¿ÉÄÜ¿ªÆô£©
	 uchar AmplitudeControldata[3] = {0x00,0x03,0xff};									//Ê®Áù¼¶µ÷ÖÆÊ±ÆôÓÃ£¬Ä¬ÈÏÊä³ö×î´ó
	 uchar FunctionRegister1data[3] = {0xD0,0xf3,0x00}; 

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 		  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);    			//ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷£¬¼´ÏàÎ»µ÷ÖÆÆô¶¯

	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWord0data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWord0data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWord1data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWord1data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p3,ChannelPhaseOffsetTuningWord2data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0b,4,ChannelPhaseOffsetTuningWord2data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p4,ChannelPhaseOffsetTuningWord3data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0c,4,ChannelPhaseOffsetTuningWord3data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p5,ChannelPhaseOffsetTuningWord4data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0d,4,ChannelPhaseOffsetTuningWord4data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p6,ChannelPhaseOffsetTuningWord5data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x0e,4,ChannelPhaseOffsetTuningWord5data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p7,ChannelPhaseOffsetTuningWord6data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x0f,4,ChannelPhaseOffsetTuningWord6data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p8,ChannelPhaseOffsetTuningWord7data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x10,4,ChannelPhaseOffsetTuningWord7data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p9,ChannelPhaseOffsetTuningWord8data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x11,4,ChannelPhaseOffsetTuningWord8data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p10,ChannelPhaseOffsetTuningWord9data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x12,4,ChannelPhaseOffsetTuningWord9data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p11,ChannelPhaseOffsetTuningWord10data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x13,4,ChannelPhaseOffsetTuningWord10data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p12,ChannelPhaseOffsetTuningWord11data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x14,4,ChannelPhaseOffsetTuningWord11data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p13,ChannelPhaseOffsetTuningWord12data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x15,4,ChannelPhaseOffsetTuningWord12data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p14,ChannelPhaseOffsetTuningWord13data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×ÖÖ
	 WriteToAD9959ViaSpi(0x16,4,ChannelPhaseOffsetTuningWord13data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p15,ChannelPhaseOffsetTuningWord14data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x17,4,ChannelPhaseOffsetTuningWord14data,1);	//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö

	 WrPhaseOffsetTuningWorddata(p16,ChannelPhaseOffsetTuningWord15data); //½«Ê®½øÖÆÏàÎ»Êı×ª»»Îª2½øÖÆÏàÎ»¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(0x18,4,ChannelPhaseOffsetTuningWord15data,1);		//Ğ´ÈëÏàÎ»¿ØÖÆ×Ö
	 
	 f=200000;
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);    	//½«Ê®½øÖÆÆµÂÊÊı×ª»»Îª2½øÖÆÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);		//Ğ´ÈëÆµÂÊ¿ØÖÆ×Ö
}
void AD9959_SetAM163(double f)
{
// 	 u8 cn = 0;
	 uchar ChannelFrequencyTuningWord0data[4];
	
	 uchar AmplitudeControldata[3]={0x00,0x03,0xff};
	 uchar ChannelAmplitudeTuningWord1data[4]={0xef,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord2data[4]={0xdf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord3data[4]={0xcf,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord4data[4]={0xbf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord5data[4]={0xaf,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord6data[4]={0x9f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord7data[4]={0x8f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord8data[4]={0x7f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord9data[4]={0x6f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord10data[4]={0x5f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord11data[4]={0x4f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord12data[4]={0x3f,0x30,0x00,0x00};
	 uchar ChannelAmplitudeTuningWord13data[4]={0x2f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord14data[4]={0x1f,0x30,0x00,0x00};
   uchar ChannelAmplitudeTuningWord15data[4]={0x0f,0x30,0x00,0x00};
   uchar ChannelFunctionRegisterdata[3] = {0x40,0x03,0x30};
	 uchar FunctionRegister1data[3]={0xD0,0xf3,0x00};

	 WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0); 			 //ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷

	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWord1data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0b,4,ChannelAmplitudeTuningWord2data,0);

	 WriteToAD9959ViaSpi(0x0c,4,ChannelAmplitudeTuningWord3data,0);

	 WriteToAD9959ViaSpi(0x0d,4,ChannelAmplitudeTuningWord4data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x0e,4,ChannelAmplitudeTuningWord5data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x0f,4,ChannelAmplitudeTuningWord6data,0);

	 WriteToAD9959ViaSpi(0x10,4,ChannelAmplitudeTuningWord7data,0);

	 WriteToAD9959ViaSpi(0x11,4,ChannelAmplitudeTuningWord8data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x12,4,ChannelAmplitudeTuningWord9data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x13,4,ChannelAmplitudeTuningWord10data,0);

	 WriteToAD9959ViaSpi(0x14,4,ChannelAmplitudeTuningWord11data,0);

	 WriteToAD9959ViaSpi(0x15,4,ChannelAmplitudeTuningWord12data,0); 						 //ÉèÖÃ·ù¶È´óĞ¡£¨S1£©

   WriteToAD9959ViaSpi(0x16,4,ChannelAmplitudeTuningWord13data,0);	 // ÉèÖÃ·ù¶È´óĞ¡£¨S2£©

	 WriteToAD9959ViaSpi(0x17,4,ChannelAmplitudeTuningWord14data,0);

	 WriteToAD9959ViaSpi(0x18,4,ChannelAmplitudeTuningWord15data,0);
	 
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
   WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);
}

/////////////////////////////////////////////////////////////////////////
 void AD9959_Frequency_Sweep(double f1,double f2,double a1,double a2) //f1ÆğÊ¼ÆµÂÊ£¬f2ÖÕÖ¹ÆµÂÊ£¬a1ÉÏÉı¦Ä£¬a2ÏÂ½µ¦Ä
{ 
	uchar ChannelFrequencyTuningWorddata1[4];
  uchar ChannelFrequencyTuningWorddata2[4];
  uchar ChannelFrequencyTuningWorddata3[4];
  uchar ChannelFrequencyTuningWorddata4[4];
	uchar FunctionRegister1data[3] = {0xD0,0x00,0x00};    							//Ä¬ÈÏÇé¿öÏÂÑ¡Ôñ   
	uchar ChannelFunctionRegisterdata[3] = {0x80,0x43,0x20};						//É¨ÆµÄ£Ê½Ê±Ñ¡Ôñ;Á¬ĞøÉ¨ÃèÃ»ÓĞÆôÓÃ£¬ÎŞRU/RD
  uchar SweepRampRatedata[2] = {0xff,0xff};														//Ä¬ÈÏµ¥Î»É¨ÃèÊ±¼ä×î³¤
//uchar FunctionRegister1data[3] = {0xD0,0x04,0x00};    							//ÏßĞÔÉ¨ÃèÊ±ĞèÒªRU/RDÊ±Ñ¡Ôñ¿ªÆô 
// uchar AmplitudeControldata[3] = {0xff,0x1f,0xff};									//¿ªÆôRU/RD
	
  WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);        		 	 	  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
  WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);  					//  ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷
// 	WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,1); 								//ĞèÊä³ö·ù¶ÈRU/RDÄ£Ê½²ÅÑ¡Ôñ,ÇÒÒªĞŞ¸Ä¹¦ÄÜ¼Ä´æÆ÷FR1
// 	f=500000;    // ÉèÖÃÆğÊ¼ÆµÂÊS0£¨0x04£©
  
	
	WrFrequencyTuningWorddata(f1,ChannelFrequencyTuningWorddata1);   		//Ğ´ÆµÂÊ¿ØÖÆ×Ö
	WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWorddata1,0);
// 	f=5000000; //ÉèÖÃÖÕÖ¹ÆµÂÊ(0x0A)
 
  WrFrequencyTuningWorddata(f2,ChannelFrequencyTuningWorddata2);  	 //Ğ´ÆµÂÊ¿ØÖÆ×Ö
  WriteToAD9959ViaSpi(0x0A,4,ChannelFrequencyTuningWorddata2,0); 
// 	f=100;   //ÉèÖÃÉÏÉı¦Ä
		
	WrFrequencyTuningWorddata(a1,ChannelFrequencyTuningWorddata3);   	 //Ğ´ÆµÂÊ¿ØÖÆ×Ö
	WriteToAD9959ViaSpi(RDW,4,ChannelFrequencyTuningWorddata3,0);
	
//   f=100;  //ÉèÖÃÏÂ½µ¦Ä
	WrFrequencyTuningWorddata(a2,ChannelFrequencyTuningWorddata4);   	 //Ğ´ÆµÂÊ¿ØÖÆ×Ö
	WriteToAD9959ViaSpi(FDW,4,ChannelFrequencyTuningWorddata4,0);
		
  WriteToAD9959ViaSpi(SRR,2,SweepRampRatedata,1);										 //ÉèÖÃµ¥Î»²½½øÊ±¼ä	
 
}
 
 void AD9959_Phase_Sweep(int p1,int p2,int a1,int a2,double f)       //p1ÆğÊ¼ÏàÎ»£¬p2ÖÕÖ¹ÏàÎ»£¬a1ÉèÖÃÉÏÉı¦Ä£¬a2ÉèÖÃÏÂ½µ¦Ä
{
	uchar ChannelPhaseOffsetTuningWorddata1[2];
	uchar ChannelPhaseOffsetTuningWorddata2[2];
	uchar ChannelPhaseOffsetTuningWorddata3[2];
	uchar ChannelPhaseOffsetTuningWorddata4[2];
	uchar ChannelFrequencyTuningWord0data[4];
	
	uchar FunctionRegister1data[3] = {0xD0,0x00,0x00};   							 //Ä¬ÈÏÇé¿öÏÂÑ¡Ôñ   
	uchar ChannelFunctionRegisterdata[3] = {0xc0,0xC3,0x30};					 //É¨ÏàÄ£Ê½Ê±Ñ¡Ôñ£¬Á¬ĞøÉ¨ÃèÃ»ÓĞÆôÓÃ£¬ÎŞRU/RD
  uchar SweepRampRatedata[2] = {0xff,0xff};													 //Ä¬ÈÏµ¥Î»É¨ÃèÊ±¼ä×î³¤
//uchar FunctionRegister1data[3] = {0xD0,0x04,0x00};    						 //ÏßĞÔÉ¨ÃèÊ±ĞèÒªRU/RDÊ±Ñ¡Ôñ¿ªÆô 
// uchar AmplitudeControldata[3] = {0xff,0x1f,0xff};								 //¿ªÆôRU/RD

	WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         		 	 //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
 	 WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);				 //  ÉèÖÃÍ¨µÀ¹¦ÄÜ¼Ä´æÆ÷
// 	 WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,1); 						 //ĞèÊä³ö·ù¶ÈRU/RDÄ£Ê½²ÅÑ¡Ôñ,ÇÒÒªĞŞ¸Ä¹¦ÄÜ¼Ä´æÆ÷FR1
// 	 p=0;// ÉèÖÃÆğÊ¼ÏàÎ»S0£¨0x04£©   Êı¾İÀàĞÍÓë×Óº¯ÊıÀïÃæ²»Ò»Ñù
	 WrPhaseOffsetTuningWorddata(p1,ChannelPhaseOffsetTuningWorddata1);
	 WriteToAD9959ViaSpi(CPOW0,2,ChannelPhaseOffsetTuningWorddata1,0);
//    p=360;//ÉèÖÃÖÕÖ¹ÏàÎ»E0(0x0A)
	 WrPhaseOffsetTuningWorddata(p2,ChannelPhaseOffsetTuningWorddata2);
	 WriteToAD9959ViaSpi(0x0a,4,ChannelPhaseOffsetTuningWorddata2,0);
//    p=10;//ÉèÖÃÉÏÉı¦Ä
	 WrPhaseOffsetTuningWorddata(a1,ChannelPhaseOffsetTuningWorddata3);
	 WriteToAD9959ViaSpi(RDW,4,ChannelPhaseOffsetTuningWorddata3,0);
//    p=10;//ÉèÖÃÏÂ½µ¦Ä
	 WrPhaseOffsetTuningWorddata(a2,ChannelPhaseOffsetTuningWorddata4);
	 WriteToAD9959ViaSpi(FDW,4,ChannelPhaseOffsetTuningWorddata4,0);
	 WriteToAD9959ViaSpi(SRR,2,SweepRampRatedata,0);										//Ğ´µ¥Î»²½½øÊ±¼ä
// 	 f=400000;    //ÉèÖÃÊä³öÆµÂÊ´óĞ¡
	 WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWord0data); 		  //Ğ´ÆµÂÊ¿ØÖÆ×Ö
	 WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWord0data,1);

}

void AD9959_Amplitute_Sweep(int a,int a1,int a2,double f)   					//aÖÕÖ¹·ù¶È£¬a1ÉÏÉı¦Ä£¬a2ÏÂ½µ¦Ä£¬fÊä³öÆµÂÊ
{
	uchar ChannelAmplitudeTuningWorddata1[4];
	uchar ChannelAmplitudeTuningWorddata2[4];
	uchar ChannelAmplitudeTuningWorddata3[4];
	uchar ChannelFrequencyTuningWorddata4[4];
	
	uchar FunctionRegister1data[3] = {0xD0,0x00,0x00};    						//Ä¬ÈÏÇé¿öÏÂÑ¡Ôñ   
	uchar ChannelFunctionRegisterdata[3] = {0x40,0x43,0x20};					//·ù¶ÈÉ¨ÃèÎ»Æô¶¯
  uchar AmplitudeControldata[3] = {0x00,0x0,0x3f};  								//·ù¶ÈÉ¨ÃèÄ£Ê½Ê±ÆôÓÃ
  uchar SweepRampRatedata[2] = {0xff,0xff};													//Ä¬ÈÏµ¥Î»É¨ÃèÊ±¼ä×î³¤

  WriteToAD9959ViaSpi(FR1,3,FunctionRegister1data,0);         	 	  //ÉèÖÃ¹¦ÄÜ¼Ä´æÆ÷
	WriteToAD9959ViaSpi(CFR,3,ChannelFunctionRegisterdata,0);
	//Ğ´ÆğÊ¼·ù¶ÈS0
  WriteToAD9959ViaSpi(ACR,3,AmplitudeControldata,0);
	//Ğ´ÖÕÖ¹·ù¶ÈE0
// 	a=1023; //Ğ´ÉÏÉı¦Ä,²»ÄÜĞ´µ½1024£¬×î´óÎª1023
	WrAmplitudeTuningWorddata(a,ChannelAmplitudeTuningWorddata1);
  WriteToAD9959ViaSpi(0x0a,4,ChannelAmplitudeTuningWorddata1,0); 		//CTW0 address 0x04.Êä³ö10MHZÆµÂÊ
// 	a=10; //Ğ´ÉÏÉı¦Ä
	WrAmplitudeTuningWorddata(a1,ChannelAmplitudeTuningWorddata2);    //Ğ´ÆµÂÊ¿ØÖÆ×Ö
	WriteToAD9959ViaSpi(RDW,4,ChannelAmplitudeTuningWorddata2,0);
//   a=10;  //Ğ´ÏÂ½µ¦Ä
	WrAmplitudeTuningWorddata(a2,ChannelAmplitudeTuningWorddata3);    //Ğ´ÆµÂÊ¿ØÖÆ×Ö
	WriteToAD9959ViaSpi(FDW,4,ChannelAmplitudeTuningWorddata3,0);
 	WriteToAD9959ViaSpi(SRR,2,SweepRampRatedata,0);										//Ğ´µ¥Î»²½½øÊ±¼ä
// 	f=400000;    
	WrFrequencyTuningWorddata(f,ChannelFrequencyTuningWorddata4);   	//Ğ´ÆµÂÊ¿ØÖÆ×Ö
  WriteToAD9959ViaSpi(CFTW0,4,ChannelFrequencyTuningWorddata4,1);
	
}


