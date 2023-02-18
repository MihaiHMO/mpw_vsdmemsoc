/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

// This include is relative to $CARAVEL_PATH (see Makefile)
#include <defs.h>
#include <stub.c>

#define reg_mprj_slave (*(volatile uint32_t*)0x30000000)
// define the memorz registers
#define BASE_ADDR 0x30000000
#define MEM_SIZE 256

volatile uint32_t* const REGS[MEM_SIZE] = {
    [0 ... MEM_SIZE-1] = (volatile uint32_t*)(BASE_ADDR),
};

for (int i = 1; i < MEM_SIZE; i++) {
    REGS[i] = (volatile uint32_t*)(BASE_ADDR + i * sizeof(uint32_t));
}

#define REG(x) (*REGS[x])
/*
	IO Test:
		- Configures MPRJ lower 8-IO pins as outputs
		- Observes counter value through the MPRJ lower 8 IO pins (in the testbench)
*/

void main()
{
	/* 
	IO Control Registers
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 3-bits | 1-bit | 1-bit | 1-bit  | 1-bit  | 1-bit | 1-bit   | 1-bit   | 1-bit | 1-bit | 1-bit   |

	Output: 0000_0110_0000_1110  (0x1808) = GPIO_MODE_USER_STD_OUTPUT
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 110    | 0     | 0     | 0      | 0      | 0     | 0       | 1       | 0     | 0     | 0       |
	
	 
	Input: 0000_0001_0000_1111 (0x0402) = GPIO_MODE_USER_STD_INPUT_NOPULL
	| DM     | VTRIP | SLOW  | AN_POL | AN_SEL | AN_EN | MOD_SEL | INP_DIS | HOLDH | OEB_N | MGMT_EN |
	| 001    | 0     | 0     | 0      | 0      | 0     | 0       | 0       | 0     | 1     | 0       |

	*/

	/* Set up the housekeeping SPI to be connected internally so	*/
	/* that external pin changes don't affect it.			*/
         reg_wb_enable = 1;
	 reg_spi_enable = 1;
	// reg_spimaster_cs = 0x10001;
	// reg_spimaster_control = 0x0801;

	// reg_spimaster_control = 0xa002;	// Enable, prescaler = 2,
                                        // connect to housekeeping SPI

	// Connect the housekeeping SPI to the SPI master
	// so that the CSB line is not left floating.  This allows
	// all of the GPIO pins to be used for user functions.

	// Configure GPIO [31:22] as user output
	// Observe counter value in the testbench
	reg_mprj_io_22 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_23 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_24 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_25 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_26 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_27 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_28 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_29 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_30 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_31 =  GPIO_MODE_USER_STD_OUTPUT;
	reg_mprj_io_32 =  GPIO_MODE_USER_STD_OUTPUT;
	
	/* Apply configuration */
	reg_mprj_xfer = 1;
	while (reg_mprj_xfer == 1);
	
	    // Delay
        for (int i = 0; i < 5; i++);

        // IMem initiation
	for (int i = 0; i < 13; i++) {
                REG[i] = 
                        i == 0x0        ?       0b00000000000100000000010010010011      :
                        i == 0x1        ?       0b00000010101100000000010100010011      :
                        i == 0x2        ?       0b00000000000000000000010110010011      :
                        i == 0x3        ?       0b00000000000000000000100010010011      :
                        i == 0x4        ?       0b00000000101110001000100010110011      :
                        i == 0x5        ?       0b00000000000101011000010110010011      :
                        i == 0x6        ?       0b11111110101001011001110011100011      :
                        i == 0x7        ?       0b00000000101110001000100010110011      :
                        i == 0x8        ?       0b01000000101110001000100010110011      :
                        i == 0x9        ?       0b01000000100101011000010110110011      :
                        i == 0xA        ?       0b11111110100101011001110011100011      :
                        i == 0xB        ?       0b01000000101110001000100010110011      :
                        i == 0xC        ?       0b11111110000000000000000011100011      :
		}

        // Wait for a few clocks to propagate signals
        for (int i = 0; i < 2; i++);
        // Wait for the expected result
        while ((reg_la2_data & 0x000003FF) != 0x2D);
        // Test has been done successfully
	reg_mprj_datal = 0xAB610000;
}

