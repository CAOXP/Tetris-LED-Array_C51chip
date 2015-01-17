/* Copyright (c) 2008 xpstudio, CaoXiangpeng
 * <xpstudio2003@yahoo.com.cn>
 * <http://xpstudio2003.blog.163.com>
 * <http://blog.ednchina.com/xpstudio2003>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */
 
/***********************************************************************/
/*                                                                     */
/*                           RAMDisp.H                                 */
/*                       All rights reserved                           */
/* Project : xp-Tetris Program Project                                 */
/* IDE     : Keil uV2                                                  */
/* Date   : 13,Aug,2008                                                */
/* Author : CaoXiangpeng xpstudio2008                                  */
/*                                                                     */
/*          latest modified: 13,Aug,2008 ,23:30                        */
/***********************************************************************/

#ifndef    __RAMDISP_H__
#define    __RAMDISP_H__
/*--------------------------------------------------
			    GND |
			    VCC |
				0	|
				.	|
				3	|
----------------------------------------------------
  ��������   SerIn  |  /SRCLR  ������λ�Ĵ�������
                NC  |  SRCK    ��λ�Ĵ���ʱ��
��    ����     GND  |  RCK     ����Ĵ���ʱ��
      ��Դ     VCC  |  /G      ���ʹ��
--------------------------------------------------*/

	#define		T0INIT_H		0xFF	//��ʱ����ֵ,������ʱʱ��
	#define		T0INIT_L		0xF0	
	#define		LINE_NUM		0x10	//16��
	#define		ROW_NUM			0x04	//4*8��
	#define		GRAY_DEFAULT	1		//Ĭ������
	#define		GRAY_NUM		10		//�����ܼ���

	sbit        B595_SRCLR       = P1^3;
	sbit        B595_SRCK        = P1^2;
	sbit        B595_RCK         = P1^1;
	sbit        B595_G           = P1^0;
	sbit        B595_SERIN       = P1^4;
	#define		ScanIO			   P2		//ɨ��IO��

	#define     set595_SRCLR()   (B595_SRCLR    = 1)
	#define     set595_SRCK()    (B595_SRCK     = 1)
	#define     set595_RCK()     (B595_RCK      = 1)
	#define     set595_G()       (B595_G        = 1)
	#define     set595_SERIN()   (B595_SERIN    = 1)
	#define     clr595_SRCLR()   (B595_SRCLR    = 0)
	#define     clr595_SRCK()    (B595_SRCK     = 0)
	#define     clr595_RCK()     (B595_RCK      = 0)
	#define     clr595_G()       (B595_G        = 0)
	#define     clr595_SERIN()   (B595_SERIN    = 0)


	#define     CloseOut()      set595_G()        //�ر����
	#define     OpenOut()       clr595_G()        //�����
	#define     ClearReg()      clr595_SRCLR()    //����Ĵ���������
	#define     HoldReg()       set595_SRCLR()    //���ּĴ���������


	extern INT8U idata DisRAM[16][4];	//�Դ�
	extern INT8U GrayScale; 			//����
	extern void SendSrlDt8(INT8U sData);
	extern void RAMDispInit();

    extern void SetPix(INT8U x, INT8U y) ;
    extern void ClearPix(INT8U x, INT8U y) ;
    extern void AntiPix(INT8U x, INT8U y) ;

#endif