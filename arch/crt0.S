/*
  ******************************************************************************
  * @file    crt0.s -APT32F101
  * @author  APT AE Team
  * @version V1.23
  * @date    2018/08/20
  ******************************************************************************
  *THIS SOFTWARE WHICH IS FOR ILLUSTRATIVE PURPOSES ONLY WHICH PROVIDES 
  *CUSTOMER WITH CODING INFORMATION REGARDING THEIR PRODUCTS.
  *APT CHIP SHALL NOT BE HELD RESPONSIBILITY ADN LIABILITY FOR ANY DIRECT, 
  *INDIRECT DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF 
  *SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION 
  *CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.AND APT CHIP RESERVES 
  *THE RIGHT TO MAKE CHANGES IN THE SOFTWARE WITHOUT NOTIFICATION
  ******************************************************************************
  */
//start from __start, 
//(0)initialize vector table
//(1)initialize all registers
//(2)prepare initial reg values for user process
//(3)initialize supervisor mode stack pointer
//(4)construct ASID Table
//(5)prepare PTE entry for user process start virtual address
//(6)creat a mapping between VPN:0 and PFN:0 for kernel
//(7)set VBR register
//(8)enable EE and MMU
//(9)jump to the main procedure using jsri main
//-----------------------user option configuration---------------------------------//
#define EnWDT        0x0000						//WDT Enable
#define DisWDT       0x5555						//WDT Disable
#define DataSize4K   0x300						//DataFlash 4k
#define DataSize2K   0x200						//DataFlash 2k
#define DataSize1K   0x100						//DataFlash 1k
#define DataSize0K   0x0000						//DataFlash Disable
#define ExtRstPA00   (0x5<<4)					//Set PA0.0 as Reset
#define ExtRstPC01   (0xa<<4)					//Set PC0.1 as Reset
#define NoExtRst     (0x00<<4)					//Reset Pin as IO
#define SWDDefault   (0x03<<2)					//SWD Pin default PA0.12/PA0.13
#define SWDOther     (0x0<<2)					//SWD Pin PA0.1/PA0.2
#define IS500K       (2)						//ISOSC 500K
#define IS3M         (3)						//ISOSC 3M
#define UserOption   ((IS3M + SWDDefault + NoExtRst) + DataSize0K) + (EnWDT<<16)
//-----------------------user option configuration---------------------------------//
.export __start
//.text
.export vector_table
//.import ISR_VecTable
.align  10
vector_table:           //totally 256 entries
//  .long   __start
//  .rept   128
//  .long   __dummy
//  .endr

.long __start
.long DummyHandler//MisalignedHandler
.long DummyHandler//AccessErrHandler
.long DummyHandler
.long DummyHandler//IllegalInstrHandler
.long DummyHandler//PriviledgeVioHandler
.long DummyHandler
.long DummyHandler//BreakPointHandler
.long DummyHandler//UnrecExecpHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler//Trap0Handler
.long DummyHandler//Trap1Handler
.long DummyHandler//Trap2Handler
.long DummyHandler//Trap3Handler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler//PendTrapHandler
.long CORETHandler//EPTHandler
.long SYSCONIntHandler
.long IFCIntHandler
.long ADCIntHandler
.long GTC0IntHandler//DummyHandler
.long GTC1IntHandler//GTC0IntHandler
.long GTC2IntHandler
.long EXI0IntHandler
.long EXI1IntHandler
.long GTC3IntHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long UART0IntHandler
.long UART1IntHandler
.long DummyHandler//PCM0IntHandler
.long DummyHandler//PCM1IntHandler
.long I2CIntHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long EXI2to3IntHandler
.long EXI4to8IntHandler
.long EXI9to13IntHandler
.long CNTAIntHandler
.long TKEYGRP0IntHandler
.long TKEYGRP1IntHandler
.long LEDIntHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long DummyHandler
.long UserOption


.text
//  .long __start
__start:
  //initialize all registers
  movi r0, 0
  movi r1, 0
  movi r2, 0
  movi r3, 0
  movi r4, 0
  movi r5, 0
  movi r6, 0
  movi r7, 0
  movi r8, 0
  movi r9, 0
  movi r10, 0
  movi r11, 0
  movi r12, 0
  movi r13, 0
  movi r14, 0
  movi r15, 0

//LVD Enable
/*MOVIH r3, 0x4001
ORI r4, r3, 0x204C
MOVIH r3, 0xB44B
ORI r5, r3, 0x01
ST.W r5, (r4,0)*/


//set VBR
  lrw r2, vector_table
  //lrw r2, ISR_VecTable
  mtcr r2, cr<1,0>

//enable EE bit of psr
  mfcr r2, cr<0,0>
  bseti r2, r2, 8
  mtcr r2, cr<0,0>

////set rom access delay
//  lrw     r1, 0xe00000
//  lrw     r2, 0x7
//  st.w    r2, (r1,0x0)

////enable cache
//  lrw     r1, 0xe000f000
//  movi    r2, 0x2
//  st.w    r2, (r1,0x0)
//  lrw     r2, 0x29
//  st.w    r2, (r1,0x4)
//  movi    r2, 0x1
//  st.w    r2, (r1,0x0)

//disable power peak 
  lrw     r1, 0xe000ef90
  movi    r2, 0x0
  st.w    r2, (r1, 0x0)

//initialize kernel stack
  lrw  r14, __kernel_stack
  subi r13,r14,0x4
  lrw  r3, 0x40
  subu r4, r14, r3
  lrw  r5, 0x0
INIT_KERLE_STACK:
  addi r4, 0x4
  st.w r5, (r4)
  //cmphs r14, r4
  cmphs r13, r4
  bt  INIT_KERLE_STACK
        
__to_main:
  lrw r0,__main
  jsr r0
  mov r0, r0
  mov r0, r0

  lrw r15, __exit
  lrw r0,main
  jmp r0
  mov r0, r0
  mov r0, r0
  mov r0, r0
  mov r0, r0
  mov r0, r0

.export __exit
__exit:

  lrw r4, 0x20003000
  lrw r5, 0x0
  st.w r5, (r4)

  mfcr r1, cr<0,0>
  movih r1, 0xFFFF
  mtcr r1, cr<11,0>
  movi    r1, 0xFFF
  movi    r0, 0x0
  st      r1, (r0)

.export DummyHandler
DummyHandler:
  //br      __fail
  movih r5,0xabcd
  ori r5,r5,0x1234
  mtcr r5,cr<31,0>
  br __start

.data
.align  10
.long __start
