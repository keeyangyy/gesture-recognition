`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/03/30 16:56:48
// Design Name: 
// Module Name: mbsys_top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module mbsys_top(
    output [12:0]DDR3_addr,
    output [2:0]DDR3_ba,
    output DDR3_cas_n,
    output [0:0]DDR3_ck_n,
    output [0:0]DDR3_ck_p,
    output [0:0]DDR3_cke,
    output [1:0]DDR3_dm,
    inout [15:0]DDR3_dq,
    inout [1:0]DDR3_dqs_n,
    inout [1:0]DDR3_dqs_p,
    output [0:0]DDR3_odt,
    output DDR3_ras_n,
    output DDR3_reset_n,
    output DDR3_we_n,
    input [6:0]key,
    output [7:0]led,
    input [0:0]touch_int,
    output [0:0]touch_rst,
    output [0:0]touch_sck,
    inout [0:0]touch_sda,
    output LCD_CS,
    output [15:0]LCD_DATA,
    output LCD_RD,
    output LCD_RS,
    output LCD_RSTN,
    output LCD_WR,
    inout spi_flash_io0_io,
    inout spi_flash_io1_io,
    inout [0:0]spi_flash_ss_io,
    input clk,
    
    output gpio2214_sck0,
    output gpio2214_sck1,
    inout gpio2214_sda0,
    inout gpio2214_sda1,
    
    output gpio2214_clk0,
    output gpio2214_clk1
    );   
       
   wire clk_40M;
   
   assign gpio2214_clk0 = clk_40M;
   assign gpio2214_clk1 = clk_40M;
   
    mbsys_wrapper(
        .DDR3_addr(DDR3_addr),
        .DDR3_ba(DDR3_ba),
        .DDR3_cas_n(DDR3_cas_n),
        .DDR3_ck_n(DDR3_ck_n),
        .DDR3_ck_p(DDR3_ck_p),
        .DDR3_cke(DDR3_cke),
        .DDR3_dm(DDR3_dm),
        .DDR3_dq(DDR3_dq),
        .DDR3_dqs_n(DDR3_dqs_n),
        .DDR3_dqs_p(DDR3_dqs_p),
        .DDR3_odt(DDR3_odt),
        .DDR3_ras_n(DDR3_ras_n),
        .DDR3_reset_n(DDR3_reset_n),
        .DDR3_we_n(DDR3_we_n),
        .GPIO_keys_tri_i(key),
        .GPIO_leds_tri_o(led),
        .GPIO_touch_int_tri_i(touch_int),
        .GPIO_touch_rst_tri_o(touch_rst),
        .GPIO_touch_sck_tri_o(touch_sck),
        .GPIO_touch_sda_tri_io(touch_sda),
        .LCD_CS(LCD_CS),
        .LCD_DATA(LCD_DATA),
        .LCD_RD(LCD_RD),
        .LCD_RS(LCD_RS),
        .LCD_RSTN(LCD_RSTN),
        .LCD_WR(LCD_WR),
        .SPI_flash_io0_io(spi_flash_io0_io),
        .SPI_flash_io1_io(spi_flash_io1_io),
        .SPI_flash_ss_io(spi_flash_ss_io),
        .sys_clk_i(clk),
        .sys_rst(1'b1),
        .clk_40M(clk_40M),
        .GPIO_sck0_tri_o(gpio2214_sck0),
        .GPIO_sck1_tri_o(gpio2214_sck1),
        .GPIO_sda0_tri_io(gpio2214_sda0),
        .GPIO_sda1_tri_io(gpio2214_sda1)
        );
        
endmodule