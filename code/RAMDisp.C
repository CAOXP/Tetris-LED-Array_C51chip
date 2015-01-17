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
/*                           RAMdisp.C                                 */
/*                                                                     */
/*                       All rights reserved                           */
/*                                                                     */
/* Project : xp-Tetris Program Project                                 */
/* IDE     : Keil uV2                                                  */
/*                                                                     */
/* Instruction: 1.the basic drive to Flash Data to the LED-Screen      */
/*                user can change the TH0 TL0 of timer0                */
/*                to change the freqence.                              */
/*                In file RAMdisp.H                                    */
/*                                                                     */
/* Date   : 13,Aug,2008                                                */
/* Author : CaoXiangpeng xpstudio2008                                  */
/*                                                                     */
/*          latest modified: 13,Aug,2008 ,23:30                        */
/***********************************************************************/

#include "MCUREG.H"
#include "SYSCONFIG.H"
#include "RAMDisp.h" 

/***********************************************************************
* ����: INT8U   GrayScale;
* ����: ���ȿ��Ʊ���,ָʾ��ʾ��������,��10������
*
* ����: ������,XPSTUDIO2008,0808
***********************************************************************/
INT8U   GrayScale;


/***********************************************************************
* ����: INT8U idata DisRAM[ROW_NUM][ROW_NUM]
* ����: �Դ�,,,��Ӧ����ʾ��
*			����:LINE_NUM
*			����:ROW_NUM
* ����: ������,XPSTUDIO2008,0808
***********************************************************************/
INT8U idata DisRAM[LINE_NUM][ROW_NUM]=
{/*--  ��ǰ����x�߶�=32x16  --*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x66,0x7C,0x00,0x32,0x66,0x66,0x00,
0x30,0x3C,0x66,0x18,0x30,0x18,0x66,0x0C,0x30,0x3C,0x66,0x06,0x32,0x66,0x66,0xF3,
0x1E,0x66,0x7C,0x06,0x00,0x00,0x60,0x0C,0x00,0x00,0x60,0x18,0x00,0x00,0x60,0x00,
0x55,0x55,0x55,0x55,0xAA,0xAA,0xAA,0xAA,0x55,0x55,0x55,0x55,0xAA,0xAA,0xAA,0xAA,
};

/**********************************************************************
* ����: SendSrlDt8(INT8U sData)
* ����: ͨ��6B595�������, һ���ֽ�.
*
* ����: ������,XPSTUDIO2008,0808
***********************************************************************/
void SendSrlDt8(INT8U sData)
{
    INT8U i;
    set595_SRCLR();      //��λ�Ĵ���ʹ��
    clr595_RCK();
    for(i=8; i>0; i--)
    {
        clr595_SRCK();
        if( sData&0x80 ) set595_SERIN();
        else             clr595_SERIN();
        sData<<=1;
        set595_SRCK();   //��������������
    }
    set595_RCK();        //�������������
}



/********************************************************************** 
* ����: RAMDispInit()
* ����: ��ʾ�ĳ�ʼ������,��ʼ����ʱ��0,���ȼ���
*
* ����: ������,XPSTUDIO2008,0808
***********************************************************************/
void RAMDispInit()
{
	TMOD &=  0xF0;		//��ʱ��0,������ʽ 1
	TMOD |=  0x01;

    TH0  = T0INIT_H;    //��ʱ��ֵ
    TL0  = T0INIT_L;

    ET0  = TRUE;        //����ʱ��0 �ж�
    TR0  = TRUE;        //��ʱ��0����

    GrayScale = GRAY_DEFAULT;    //���ȳ�ֵ
}



/********************************************************************** 
* ����: void Timer0_INT(void) interrupt 1
* ����: ���Դ����������ʾ����,���ö�ʱ��0
*
* ����: ������,XPSTUDIO2008,0808
***********************************************************************/
void Timer0_INT(void) interrupt 1
{
    static INT8U 	T0intCnt	= 0;    //������¼�����жϵĴ���
	static INT8U	LineScanCnt = 0;	//������¼ɨ�赽��һ��

    TH0  = T0INIT_H;   					//���¼��س�ֵ
    TL0  = T0INIT_L;

    T0intCnt ++ ;

	if(T0intCnt == GrayScale)CloseOut();//���ȿ���


	if(T0intCnt < 10) return;			//��ʱ���жϴ���
	T0intCnt = 0;

	LineScanCnt ++;
	if(LineScanCnt == LINE_NUM)			//һ��LINE_NUM������
		LineScanCnt = 0;	

	CloseOut();							//1.�ر����,��ֹ��Ӱ
	SendSrlDt8(DisRAM[LineScanCnt][0]);	//2.�������
	SendSrlDt8(DisRAM[LineScanCnt][1]);
	SendSrlDt8(DisRAM[LineScanCnt][2]);
	SendSrlDt8(DisRAM[LineScanCnt][3]);
	ScanIO = LineScanCnt|(ScanIO&0XF0);	//3.ȷ��ɨ���LineScanCnt��
	OpenOut();							//4.�����,��ʾ����
}

#define     dlydly  delayms(10)
/********************************************************************** 
* ����: void SetPix(INT8U x, INT8U y) 
* ����: ������x,y��ĵ���1
*
* ����: ������,XPSTUDIO2008,0813
***********************************************************************/
void SetPix(INT8U x, INT8U y) 
{
    DisRAM[y][x>>3] |=  0x80 >>(x&0x07);
    dlydly;
}
/********************************************************************** 
* ����: void ClearPix(INT8U x, INT8U y) 
* ����: ������x,y��ĵ���0
*
* ����: ������,XPSTUDIO2008,0813
***********************************************************************/
void ClearPix(INT8U x, INT8U y) 
{
    DisRAM[y][x>>3] &= ~(0x80>>(x&0x07));
    dlydly;
}

/********************************************************************** 
* ����: void AntiPix(INT8U x, INT8U y) 
* ����: ������x,y��ĵ�ȡ��
*
* ����: ������,XPSTUDIO2008,0813
***********************************************************************/
void AntiPix(INT8U x, INT8U y) 
{
    DisRAM[y][x>>3] ^=  0x80 >>(x&0x07);
    dlydly;
}

//-----------------------------End of File-----------------------------//