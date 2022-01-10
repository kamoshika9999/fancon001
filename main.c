/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F1827
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void TMR4_CustumCallBack();
int tmr4_count = 0;
uint16_t tempsignalon_count = 0;
uint16_t temp_duty = 0;
uint16_t old_temp_duty = 0;
/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    int8_t rxData;
    char strBuf[10];
    int srtBufPointer;
    uint16_t minDuty = 150;
    uint16_t maxDuty = 700;
        
    //初期化処理
    srtBufPointer = 0;
    memset(strBuf, '\0', sizeof(strBuf));

    //電源ON時ファンを2.5秒強制回転させる
    EPWM1_LoadDutyValue(maxDuty);
    __delay_ms(2500);
    EPWM1_LoadDutyValue(minDuty);
    
    
    //温調器シグナル取得用コールバック 10ms毎に呼ばれる
    TMR4_SetInterruptHandler(TMR4_CustumCallBack);
    
    while (1)
    {

        if( temp_duty != old_temp_duty){
            printf("duty=%d old_duty=%d\r\n",temp_duty,old_temp_duty);
            old_temp_duty = temp_duty;
            
            if( temp_duty < minDuty ){
                EPWM1_LoadDutyValue(minDuty);
            }
            else if( temp_duty > maxDuty ){
                EPWM1_LoadDutyValue(maxDuty);
            }else{
                EPWM1_LoadDutyValue(temp_duty);
            }
    }
}

void TMR4_CustumCallBack(){
    if( TEMP_SIGNAL_GetValue() == HIGH ){
        tempsignalon_count++;
    }
    
    if( ++tmr4_count == 100 ){
        tmr4_count = 0;
        temp_duty = tempsignalon_count*10;
        tempsignalon_count = 0;
    }
}
/**
 End of File
*/