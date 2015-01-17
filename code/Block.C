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
/*                           Block.C                                   */
/*                                                                     */
/*                       All rights reserved                           */
/*                                                                     */
/* Project : xp-Tetris Program Project                                 */
/* IDE     : Keil uV2                                                  */
/*                                                                     */
/* Instruction: 1.some functions used when the Tetris is running       */
/*              2.Basic-Drive File                                     */
/*                                                                     */
/* Date   : 13,Aug,2008                                                */
/* Author : CaoXiangpeng xpstudio2008                                  */
/*                                                                     */
/*          latest modified: 13,Aug,2008 ,23:30                        */
/***********************************************************************/

#include "MCUREG.H"
#include "SYSCONFIG.H"
#include "RAMDisp.h"
#include "DispROM.H"
//#include "UART.H"

typedef struct
{
    INT8U BlckIndex :4;     //�������0~6
    INT8U Status    :4;     //ÿ������ǰ��״0~3
    INT8U code *  pPIC;     //��Ӧ��ͼ������ָ��
    INT8U x,y;              //λ��
}TETRIS;

TETRIS this;


INT8U data BufChng[4][2];   //ת�������е���ʱ������
INT8U TopLine = 1;          //��������������ߵ���(ֵ��С)



#define     ShortDly()      delayms(6)
#define     LongDly()       Delay(20)

//--------------------------------------------------------------


/******************************************************************** 
* ����: void GetPicPointer()
* ����: �ɶ��� ��ż�״̬ ��ö��� ͼ����ָ��
*
* ����: ������,XPSTUDIO2008,12,Aug,2008
***********************************************************************/
void GetPicPointer()
{
    this.pPIC   = TtrsBlck
                + TtrsBlckPoint[this.BlckIndex] 
                + (this.Status) * 4;
}
/******************************************************************** 
* ����: INT8U ShowTetrisBlck()
* ����: ����Ӧ�ķ����ͼ��д��RAM��ȥ
*       �ɹ�����1,���ɹ�����0
*
* ����: ������,XPSTUDIO2008,12,Aug,2008
***********************************************************************/
INT8U ShowTetrisBlck()
{
    INT8U xtmp,Btmp;
    INT8U i;

    Btmp = this.x >> 3;     //ȡ��ͼ�ζ�Ӧ���ֽ���..
    xtmp = this.x & 0x07;   //ȡ��ͼ���ڶ�Ӧ�ֽ������λ��

    //�޸�RAM_BufChng
    for(i=0; i<4; i++)
    {
        if(xtmp <= 4)
        {
            BufChng[i][0] = this.pPIC[i] << (4 - xtmp);
            BufChng[i][1] = 0;
        }
        else// if(xtmp > 4)
        {
            BufChng[i][0] = this.pPIC[i] >> (xtmp - 4);
            BufChng[i][1] = this.pPIC[i] << (12 - xtmp);
        }        
    }

    //���RAM��Ӧλ���Ƿ���ͼ��
    for(i=0; i<4; i++)
    {
        if( (DisRAM[this.y + i][Btmp])   & (BufChng[i][0]) )
            return FAILURE;
        if( (DisRAM[this.y + i][Btmp+1]) & (BufChng[i][1]) )
            return FAILURE;
    }

    //��RAM_BUFFERд��RAM��ȥ
    for(i=0; i<4; i++)
    {
        DisRAM[this.y + i][Btmp]    |= BufChng[i][0];
        DisRAM[this.y + i][Btmp+1]  |= BufChng[i][1];
    }

    return SUCCESS;
}
/******************************************************************** 
* ����: void ClearTetrisBlck()
* ����: ���ԭ��λ���ϵķ���
*           ע��:û��ȥȷ��RAMBuff�������
*
* ����: ������,XPSTUDIO2008,12,Aug,2008
***********************************************************************/
void ClearTetrisBlck()
{
    INT8U Btmp;
    INT8U i;
    
    Btmp = this.x >> 3;     //ȡ��ͼ�ζ�Ӧ���ֽ���..
    for(i=0; i<4; i++)
    {
        DisRAM[this.y + i][Btmp]    &= ~BufChng[i][0];
        DisRAM[this.y + i][Btmp+1]  &= ~BufChng[i][1];
    }
}
/******************************************************************** 
* ����: void ClearLine(INT8U line)
* ����: ���һ��
*       
*
* ����: ������,XPSTUDIO2008,13,Aug,2008
***********************************************************************/
void ClearLine(INT8U line)
{
    #if TRS_X_BYTES >2
    INT8U xtmp;
    #endif

    for(; line>TopLine; line--)
    {
        //�Ȱѵ�һ���ֽڵ�һ������0
        DisRAM[line][TRS_X_BYTE_MIN] &= ~XBYTES_HEAD;
        DisRAM[line][TRS_X_BYTE_MIN] |= DisRAM[line-1][TRS_X_BYTE_MIN]
                                        & XBYTES_HEAD;
        //�м��ֱ�Ӹ�ֵ 
        #if TRS_X_BYTES >2
        for(xtmp=TRS_X_BYTE_MIN+1 ; xtmp<TRS_X_BYTE_MAX;  xtmp++)
        {
            DisRAM[line][xtmp] = DisRAM[line-1][xtmp];
        }
        #endif

        #if TRS_X_BYTES > 1
        //�ٰ����һ���ֽڵ�һ������0
        DisRAM[line][TRS_X_BYTE_MAX] &= ~XBYTES_END;
        DisRAM[line][TRS_X_BYTE_MAX] |= DisRAM[line-1][TRS_X_BYTE_MAX] 
                                        & XBYTES_END;
        #endif

    }
}



//  ���ϲ���Ϊ�Ӻ���
//--------------------------------------------------------------

/******************************************************************** 
* ����: INT8U MoveLeft()
* ����: ��ǰ���������ƶ�һ��
*       �ɹ�����SUCCESS,ʧ�ܷ���FAILURE
*
* ����: ������,XPSTUDIO2008,12,Aug,2008
***********************************************************************/
INT8U MoveLeft()
{
    #ifdef  CHECK_BOUND
    //�ȼ���Ƿ�Խ��
    if(this.x - 1 <= TRS_X_MIN) return FAILURE;
    #endif

    //�����ԭ����ͼ��
    ClearTetrisBlck(); 

    this.x -- ;

    if( ShowTetrisBlck() == FAILURE)
    {
        this.x ++ ;     //�����ƶ�,����ԭֵ
        ShowTetrisBlck();
        return FAILURE;
    }
    
    return SUCCESS;
}


/******************************************************************** 
* ����: INT8U MoveRight()
* ����: ��ǰ���������ƶ�һ��
*       �ɹ�����SUCCESS,ʧ�ܷ���FAILURE
*
* ����: ������,XPSTUDIO2008,12,Aug,2008
***********************************************************************/
INT8U MoveRight()
{
    #ifdef  CHECK_BOUND
    //�ȼ���Ƿ�Խ��
    if(this.x + 1 >= TRS_X_MAX) return FAILURE;    //test
    #endif

    //�����ԭ����ͼ��
    ClearTetrisBlck(); 

    this.x ++ ;

    if( ShowTetrisBlck() == FAILURE)
    {
        this.x -- ;     //�����ƶ�,����ԭֵ
        ShowTetrisBlck();
        return FAILURE;
    }
    
    return SUCCESS;
}


/******************************************************************** 
* ����: INT8U MoveDown()
* ����: ��ǰ���������ƶ�һ��
*       �ɹ�����SUCCESS,ʧ�ܷ���FAILURE
*
* ����: ������,XPSTUDIO2008,13,Aug,2008
***********************************************************************/
INT8U MoveDown()
{
    #ifdef  CHECK_BOUND
    //�ȼ���Ƿ�Խ��
    if(this.x + 1 > TRS_Y_MAX) return FAILURE;
    #endif

    //�����ԭ����ͼ��
    ClearTetrisBlck(); 

    this.y ++ ;

    if( ShowTetrisBlck() == FAILURE)
    {
        this.y -- ;     //�����ƶ�,����ԭֵ
        ShowTetrisBlck();

        if(this.y < TopLine) TopLine = this.y;      //�����ߵĵ�

        return FAILURE;
    }
    
    return SUCCESS;   
}
/******************************************************************** 
* ����: INT8U TurnStatus()
* ����: ����ת��״̬
*       �ɹ�����SUCCESS,ʧ�ܷ���FAILURE
*
* ����: ������,XPSTUDIO2008,13,Aug,2008
***********************************************************************/
INT8U TurnStatus()
{
    INT8U StatusPrev;

    #ifdef  CHECK_BOUND
    //�ȼ���Ƿ�Խ��
    if(this.x + 1 > TRS_Y_MAX) return FAILURE;
    #endif

    StatusPrev = this.Status;

    //�����ԭ����ͼ��
    ClearTetrisBlck(); 

    //״̬ת��
    this.Status ++;
    if(this.Status >= TtrsBlckStsNum[this.BlckIndex])
        this.Status = 0;
    GetPicPointer();            //���»��ͼ��ָ��

    if( ShowTetrisBlck() == FAILURE)
    {
        this.Status = StatusPrev;   //�ָ�ԭ��״̬
        GetPicPointer();            //���»��ͼ��ָ��

        ShowTetrisBlck();
        return FAILURE;
    }
    
    return SUCCESS;    
}


/******************************************************************** 
* ����: INT8U ClearAreaFull()
* ����: ���һ���������Ƿ���ȫ1����,��ɾ��ȫ1����
*       ����ɾ��������
*
* ����: ������,XPSTUDIO2008,13,Aug,2008
***********************************************************************/
INT8U ClearAreaFull()
{
    bit cFlag;
    INT8U ytmp;
    INT8U lines=0;        //����һ�����������

    #if TRS_X_BYTES > 2
    INT8U xtmp,Flag;
    #endif

    for(ytmp = TRS_Y_MAX-1 ; ytmp>this.y; ytmp--)
    {
        cFlag = TRUE;
        //����һ���Ƿ�ȫ1
        if( (DisRAM[ytmp][TRS_X_BYTE_MIN] & XBYTES_HEAD) != XBYTES_HEAD ) cFlag = FALSE;

        //�м��ֱ�Ӽ��    
        #if TRS_X_BYTES > 2
        Flag=FALSE;
        for(xtmp=TRS_X_BYTE_MIN+1 ; xtmp<TRS_X_BYTE_MAX;  xtmp++)
        {
            if( DisRAM[ytmp][xtmp] != 0xFF )
            {
                Flag=TRUE;
                break;
            }
        }
        if(Flag==TRUE)continue;
        #endif  
     
        #if TRS_X_BYTES > 1
        //������һ���Ƿ�ȫ1
        if( (DisRAM[ytmp][TRS_X_BYTE_MAX] & XBYTES_END)  != XBYTES_END )  cFlag = FALSE;
        #endif

        //��һ����������
        if(cFlag == TRUE)
        {
            lines++;
            ClearLine(ytmp);
            ytmp ++;
            ShortDly();
        }    
    }
    return lines;
}


/******************************************************************** 
* ����: INT8U BlockGenerate()
* ����: ����������,�����״̬
*       �ɹ�����SUCCESS,
* ע��: �������Ӧ��ʹ������������ɷ���,,����û��ʹ��,,,
*       ����ʹ����ѭ�����ɵķ���
*
* ����: ������,XPSTUDIO2008,13,Aug,2008
***********************************************************************/
INT8U BlockGenerate()
{
    static INT8U   X=0;
    this.x = ( TRS_X_MIN + TRS_X_MAX )>>1;
    this.y = TRS_Y_MIN+1;
    this.BlckIndex = (X++)%7;
    this.Status = 0;

    GetPicPointer();
    return ShowTetrisBlck();
}


/******************************************************************** 
* ����: void ClearTetrisScreen()
* ����: �巽����,,�Թ���Ϸ��ʼ
*       
*
* ����: ������,XPSTUDIO2008,13,Aug,2008
***********************************************************************/
void ClearTetrisScreen()
{
    INT8U xtemp,ytemp;

    //ȡ��
    for(ytemp = TRS_Y_MIN; ytemp <=TRS_Y_MAX; ytemp++)
    {
        for(xtemp = TRS_X_MIN; xtemp <=TRS_X_MAX; xtemp++)
        {
            AntiPix(xtemp, ytemp);
        }
    }
    LongDly();
    //��1
    for(ytemp = TRS_Y_MIN; ytemp <=TRS_Y_MAX; ytemp++)
    {
        for(xtemp = TRS_X_MIN; xtemp <=TRS_X_MAX; xtemp++)
        {
            SetPix(xtemp, ytemp);
        }
    }
    //��0
    for(ytemp = TRS_Y_MIN; ytemp <=TRS_Y_MAX; ytemp++)
    {
        for(xtemp = TRS_X_MIN; xtemp <=TRS_X_MAX; xtemp++)
        {
            ClearPix(xtemp, ytemp);
        }
    }
    LongDly();LongDly();
    //��߿�
    for(xtemp =TRS_X_MAX; xtemp >TRS_X_MIN; xtemp--)
    {
        SetPix(xtemp, TRS_Y_MIN);   
    }
    for(ytemp = TRS_Y_MIN; ytemp <=TRS_Y_MAX; ytemp++)
    {
        SetPix(TRS_X_MIN, ytemp);   
    }
    for(xtemp = TRS_X_MIN; xtemp <=TRS_X_MAX; xtemp++)
    {
        SetPix(xtemp, TRS_Y_MAX);   
    }
    for(ytemp = TRS_Y_MAX; ytemp >0; ytemp--)
    {
        SetPix(TRS_X_MAX, ytemp);   
    }
}


//-----------------------------End of File-----------------------------//