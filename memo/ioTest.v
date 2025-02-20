// --------------------------------------------------------------------
// Copyright (c) 2019 by MicroPhase Technologies Inc. 
// --------------------------------------------------------------------
//
// Permission:
//
//   MicroPhase grants permission to use and modify this code for use
//   in synthesis for all MicroPhase Development Boards.
//   Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  MicroPhase provides no warranty regarding the use 
//   or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     MicroPhase Technologies Inc
//                     Shanghai, China
//
//                     web: http://www.microphase.cn/   
//                     email: support@microphase.cn
//
// --------------------------------------------------------------------
// --------------------------------------------------------------------
//
// Major Functions:	
//
// --------------------------------------------------------------------
// --------------------------------------------------------------------
//
//  Revision History:
//  Date          By            Revision    Change Description
//---------------------------------------------------------------------
//2019-11-22      Chaochen Wei  1.0          Original
//2019/                         1.1          
// --------------------------------------------------------------------
// --------------------------------------------------------------------

module ioTest
#(parameter N=20)
(
    
    output [60:0]	ioA,
    output [67:0]	ioB,
    output [19:0]	DFP,
    output [19:0]	DFN,
    output RS485DI,    
    
    input wire sysClk50m,
    input wire resetN,
    input wire RS485RO,
    
    output  ledV1,         //output io
    output  ledV3,         //output io
    output  ledV4         //output io
);

//==================================================
//internal singals
//==================================================
wire    clkLocked;
reg     [31:0]  baseTimer;
reg     [60:0]  ioA_r;
reg     [67:0]  ioB_r;
reg     [0:0]   RS485DI_r;
reg     [19:0]  DF_r;


//differential clock to single clock
/*
IBUFDS   sys_clk_ibufds(
    .O(clk          ),
    .I(sys_clk_p    ),
    .IB(sys_clk_n   )
);
*/

//timer 
always @(posedge sysClk200m)begin
    baseTimer <= baseTimer + 1'b1;
    ioA_r[7:0]    <=  baseTimer[23:16];
    ioA_r[15:8]    <=  baseTimer[23:16];
    ioA_r[23:16]    <=  baseTimer[23:16];
    ioA_r[31:24]    <=  baseTimer[23:16];
    ioA_r[39:32]    <=  baseTimer[23:16];
    ioA_r[47:40]    <=  baseTimer[23:16];
    ioA_r[55:48]    <=  baseTimer[23:16];
    ioA_r[60:56]    <=  baseTimer[20:16];

    ioB_r[7:0]    <=  baseTimer[23:16];
    ioB_r[15:8]    <=  baseTimer[23:16];
    ioB_r[23:16]    <=  baseTimer[23:16];
    ioB_r[31:24]    <=  baseTimer[23:16];
    ioB_r[39:32]    <=  baseTimer[23:16];
    ioB_r[47:40]    <=  baseTimer[23:16];
    ioB_r[55:48]    <=  baseTimer[23:16];
    ioB_r[63:56]    <=  baseTimer[23:16];
    ioB_r[67:64]    <=  baseTimer[19:16];

    RS485DI_r    <=  baseTimer[16:16];

    DF_r[7:0]    <=  baseTimer[23:16];
    DF_r[15:8]    <=  baseTimer[23:16];
    DF_r[19:16]    <=  baseTimer[19:16];
    
    
end
assign ledV1=baseTimer[23];
assign ledV3=baseTimer[24];
assign ledV4=baseTimer[25];
assign ioA[60:0]=ioA_r[60:0];
assign ioB[67:0]=ioB_r[67:0];
assign RS485DI=RS485DI_r;



//PLL 200Mhz
clk_wiz_0 clk_wiz_0_inst(
    .clk_in1(sysClk50m),    
    .clk_out1(sysClk200m),  
    .reset(~resetN),
    .locked(clkLocked)
    );

generate
for(genvar i=0;i<20;i=i+1)begin
  OBUFDS #(
  .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst0 (
      .O(DFP[i]),        
      .OB(DFN[i]),       
      .I(DF_r[i])        
    );
    end
endgenerate


/*
//=========================================    
//df0    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst0 (
      .O(DFP[0]),        
      .OB(DFN[0]),       
      .I(DF_r[0])        
    );
//=========================================    
//df1    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst1 (
      .O(DFP[1:1]),        
      .OB(DFN[1:1]),       
      .I(DF_r[1:1])        
    );
//=========================================    
//df2    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst2 (
      .O(DFP[2:2]),        
      .OB(DFN[2:2]),       
      .I(DF_r[2:2])        
    );
//=========================================    
//df3    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst3 (
      .O(DFP[3:3]),        
      .OB(DFN[3:3]),       
      .I(DF_r[3:3])        
    );
//=========================================    
//df4    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst4 (
      .O(DFP[4:4]),        
      .OB(DFN[4:4]),       
      .I(DF_r[4:4])        
    );
//=========================================    
//df5    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst5 (
      .O(DFP[5:5]),        
      .OB(DFN[5:5]),       
      .I(DF_r[5:5])        
    );
//=========================================    
//df6    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst6 (
      .O(DFP[6:6]),        
      .OB(DFN[6:6]),       
      .I(DF_r[6:6])        
    );
//=========================================    
//df7    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst7 (
      .O(DFP[7:7]),        
      .OB(DFN[7:7]),       
      .I(DF_r[7:7])        
    );
//=========================================    
//df8    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst8 (
      .O(DFP[8:8]),        
      .OB(DFN[8:8]),       
      .I(DF_r[8:8])        
    );
//=========================================    
//df9    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst9 (
      .O(DFP[9:9]),        
      .OB(DFN[9:9]),       
      .I(DF_r[9:9])        
    );
//=========================================    
//df10    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst10 (
      .O(DFP[10:10]),        
      .OB(DFN[10:10]),       
      .I(DF_r[10:10])        
    );
//=========================================    
//df11    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst11 (
      .O(DFP[11:11]),        
      .OB(DFN[11:11]),       
      .I(DF_r[11:11])        
    );
//=========================================    
//df12    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst12 (
      .O(DFP[12:12]),        
      .OB(DFN[12:12]),       
      .I(DF_r[12:12])        
    );
//=========================================    
//df13    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst13 (
      .O(DFP[13:13]),        
      .OB(DFN[13:13]),       
      .I(DF_r[13:13])        
    );
//=========================================    
//df14    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst14 (
      .O(DFP[14:14]),        
      .OB(DFN[14:14]),       
      .I(DF_r[14:14])        
    );
//=========================================    
//df15    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst15 (
      .O(DFP[15:15]),        
      .OB(DFN[15:15]),       
      .I(DF_r[15:15])        
    );
//=========================================    
//df16    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst16 (
      .O(DFP[16:16]),        
      .OB(DFN[16:16]),       
      .I(DF_r[16:16])        
    );
//=========================================    
//df17    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst17 (
      .O(DFP[17:17]),        
      .OB(DFN[17:17]),       
      .I(DF_r[17:17])        
    );
//=========================================    
//df18    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst18 (
      .O(DFP[18:18]),        
      .OB(DFN[18:18]),       
      .I(DF_r[18:18])        
    );
//=========================================    
//df19    
    OBUFDS #(
      .IOSTANDARD("DEFAULT"), 
      .SLEW("SLOW")           
    ) OBUFDS_inst19 (
      .O(DFP[19:19]),        
      .OB(DFN[19:19]),       
      .I(DF_r[19:19])        
    );
    
*/    
    


endmodule
