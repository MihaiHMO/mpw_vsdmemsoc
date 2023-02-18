// SPDX-FileCopyrightText: 2020 Efabless Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// SPDX-License-Identifier: Apache-2.0

`default_nettype none
/*
 *-------------------------------------------------------------
 *
 * user_proj_example
 *
 * This is an example of a (trivially simple) user project,
 * showing how the user project can connect to the logic
 * analyzer, the wishbone bus, and the I/O pads.
 *
 * This project generates an integer count, which is output
 * on the user area GPIO pads (digital output only).  The
 * wishbone connection allows the project to be controlled
 * (start and stop) from the management SoC program.
 *
 * See the testbenches in directory "mprj_counter" for the
 * example programs that drive this user project.  The three
 * testbenches are "io_ports", "la_test1", and "la_test2".
 *
 *-------------------------------------------------------------
 */

module vsdmemsoc #(
    parameter   [31:0]  BASE_ADDRESS    = 32'h3000_0000,        // base address
    parameter   [31:0]  MAX_MEM_ADR     = 32'hFF            //  memory capacity (FF = 256)
      )	(
`ifdef USE_POWER_PINS
    inout vccd1,	// User area 1 1.8V supply
    inout vssd1,	// User area 1 digital ground
`endif
    output [9:0] OUT,
    input ext_clk,
    input ext_rst,
    //input init_en,          internal (mem_wr)
    //input [7:0] init_addr,   replaced by wbs_adr_i
    //input [31:0] init_data,  replaced by wbs_dat_i
    input wb_clk_i,
    input wb_rst_i,
    input wbs_stb_i,
    input wbs_cyc_i,
    input wbs_we_i,
    input [3:0] wbs_sel_i,
    input [31:0] wbs_dat_i,
    input [31:0] wbs_adr_i,
    output reg wbs_ack_o,
    output reg[31:0] wbs_dat_o	
);
    wire       in_rst;
    wire        mem_wr_b;      //   write_enable signal for sram  
    wire [7:0]  mem_addr;    //   address bus for sram
    wire [31:0] imem_data;   //   sram dout data connection to cpu (instruction mem)
    wire [7:0]  imem_addr;   //   sram address connection from cpu (instruction mem)
    wire        mem_clk;            //   sram clk (from system or wishbon)
    wire [31:0] mem_din ;
    assign in_rst = ext_rst ? 1'b1 : !wb_rst_i ;            // rv core reset active in case of external reset or when memory is writen Wishbone if is active
    assign mem_clk = wb_rst_i ? ext_clk : wb_clk_i  ;       // memory clock = ext_clk if wb if is not active and wb clk when wb if is active
    assign mem_wr_b   = wb_rst_i ? 1'b1 : !wbs_we_i;           // memory write enable  = 1 (inactive) when wb is in reset , tracks wbs_we if wb interface in not in reset 
    assign mem_addr = wb_rst_i ? imem_addr : wbs_adr_i;


   always @(posedge wb_clk_i or posedge wb_rst_i) begin
        if (wb_rst_i) begin 
        	
            wbs_dat_o   <=0;
            wbs_ack_o   <=0;

        end else if (wbs_cyc_i && wbs_stb_i && !wbs_ack_o && wbs_we_i && (wbs_dat_i >= BASE_ADDRESS  && wbs_dat_i <= BASE_ADDRESS + MAX_MEM_ADR))begin // sram write 
            if (wbs_sel_i[0] ) mem_din[7:0]   <= wbs_dat_i[7:0] ;
            if (wbs_sel_i[1] ) mem_din[15:8]  <= wbs_dat_i[15:8] ; 
            if (wbs_sel_i[2] ) mem_din[23:16] <= wbs_dat_i[23:16] ;
            if (wbs_sel_i[3] ) mem_din[31:24] <= wbs_dat_i[31:24] ;
            wbs_ack_o <= 1;
        end else if (wbs_cyc_i && wbs_stb_i && !wbs_ack_o && !wbs_we_i) begin // sram read 
            wbs_dat_o <= imem_data;
            wbs_ack_o <= 1;
        end else begin 
            wbs_ack_o <= 0;
            wbs_dat_o <= 32'b0;
        end
    end
   
 
    rvmyth core (
        .OUT(OUT),
        .CLK(ext_clk),
        .reset(in_rst),

        .imem_addr(imem_addr),
        .imem_data(imem_data)
    );
    
    sky130_sram_1kbyte_1rw1r_32x256_8 mem (
`ifdef USE_POWER_PINS
        .vccd1(vccd1),	// User area 1 1.8V supply
        .vssd1(vssd1),	// User area 1 digital ground
`endif
        // Port 0: RW
        .clk0(mem_clk),
        .csb0(1'b0),
        .web0(mem_wr_b),
        .wmask0(4'b1111),
        .addr0(mem_addr),
        .din0(wbs_dat_i),
        .dout0(imem_data),
        
        // Port 1: R
        .clk1(mem_clk),
        .csb1(1'b1),
        .addr1(mem_addr),
        .dout1()
    );
endmodule
`default_nettype wire
