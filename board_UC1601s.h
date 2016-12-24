/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    boards/addons/gdisp/board_UC1601s_i2c.h
 * @brief   GDISP Graphic Driver subsystem board interface for the UC1601s display.
 *
 * @note	This file contains a mix of hardware specific and operating system specific
 *			code. You will need to change it for your CPU and/or operating system.
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

// For a multiple display configuration we would put all this in a structure and then
//	set g->board to that structure.
//#define UC1601s_I2C_ADDRESS_COMMAND	0x70  // control address
//#define UC1601s_I2C_ADDRESS_DATA	0x72  // data address

#define UC1601s_I2C_ADDRESS_COMMAND	0x38  // control address
#define UC1601s_I2C_ADDRESS_DATA	0x39  // data address

#define UC1601s_RESET_PORT			GPIOB
#define UC1601s_RESET_PIN			5
#define UC1601s_SDA_PORT			GPIOB
#define UC1601s_SDA_PIN				7
#define UC1601s_SCL_PORT			GPIOB 
#define UC1601s_SCL_PIN				6

#define SET_RST						palSetPad(UC1601s_RESET_PORT, UC1601s_RESET_PIN);
#define CLR_RST						palClearPad(UC1601s_RESET_PORT, UC1601s_RESET_PIN);

// I2C configuration structure.
static const I2CConfig i2cconfig = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2
};

#if GFX_USE_OS_CHIBIOS
	static int32_t thdPriority = 0;
#endif

static GFXINLINE void init_board(GDisplay *g) {

	// As we are not using multiple displays we set g->board to NULL as we don't use it.
	g->board = 0;

	switch(g->controllerdisplay) {
	case 0:											// Set up for Display 0
		// RESET pin.
		palSetPadMode(UC1601s_RESET_PORT, UC1601s_RESET_PIN, PAL_MODE_OUTPUT_PUSHPULL);


		/*
		 * Initializes the I2C driver 1. The I2C1 signals are routed as follows:
		 * PB6 - SCL.
		 * PB7 - SDA.
		 * Timing value comes from ST I2C config tool (xls):
		 * 0x00901D2B;		// 100kHz Standard Mode
		 * 0x00300444;		// 100kHz Fast Mode
		 * 0x0030020A;		// 400kHz Fast Mode
		 * 0x00100002;		// 800kHz Fast Mode +
		 */
		palSetPadMode(UC1601s_SCL_PORT, UC1601s_SCL_PIN, PAL_MODE_STM32_ALTERNATE_OPENDRAIN );
		palSetPadMode(UC1601s_SDA_PORT, UC1601s_SDA_PIN, PAL_MODE_STM32_ALTERNATE_OPENDRAIN );

		i2cInit();
		break;
	}
}

static GFXINLINE void post_init_board(GDisplay *g) {
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, bool_t state) {
	(void) g;
	if(state)
		CLR_RST
	else
		SET_RST
}

static GFXINLINE void acquire_bus(GDisplay *g) {
	(void) g;
	#if GFX_USE_OS_CHIBIOS
		thdPriority = (int32_t)chThdGetPriorityX();
		chThdSetPriority(HIGHPRIO);
	#endif
	i2cAcquireBus(&I2CD1);
}

static GFXINLINE void release_bus(GDisplay *g) {
	(void) g;
	#if GFX_USE_OS_CHIBIOS
		chThdSetPriority(thdPriority);
	#endif
	i2cReleaseBus(&I2CD1);
}

static GFXINLINE void write_cmd(GDisplay *g, uint8_t cmd) {
	(void) g;

	uint8_t command[1];
	command[0] = cmd;

	i2cStart(&I2CD1, &i2cconfig);
	i2cMasterTransmitTimeout(&I2CD1, UC1601s_I2C_ADDRESS_COMMAND, command, 1, 0, 0, MS2ST(10));
	i2cStop(&I2CD1);
}

static GFXINLINE void write_cmd2(GDisplay *g, uint8_t cmd1, uint8_t cmd2) {
	(void) g;

	uint8_t command[2];
	command[0] = cmd1;
	command[1] = cmd2;

	i2cStart(&I2CD1, &i2cconfig);
	i2cMasterTransmitTimeout(&I2CD1, UC1601s_I2C_ADDRESS_COMMAND, command, 2, 0, 0, MS2ST(10));
	i2cStop(&I2CD1);
}

static GFXINLINE void write_cmd3(GDisplay *g, uint8_t cmd1, uint8_t cmd2, uint8_t cmd3) {
	(void) g;

	uint8_t command[3];
	command[0] = cmd1;
	command[1] = cmd2;
	command[2] = cmd3;

	i2cStart(&I2CD1, &i2cconfig);
	i2cMasterTransmitTimeout(&I2CD1, UC1601s_I2C_ADDRESS_COMMAND, command, 3, 0, 0, MS2ST(10));
	i2cStop(&I2CD1);
}

static GFXINLINE void write_data(GDisplay *g, uint8_t* data, uint16_t length) {
	(void) g;

	i2cStart(&I2CD1, &i2cconfig);
	i2cMasterTransmitTimeout(&I2CD1, UC1601s_I2C_ADDRESS_DATA, data, length, 0, 0, MS2ST(10));
	i2cStop(&I2CD1);
}

static GFXINLINE void set_backlight(GDisplay *g, uint8_t procent)
{
	(void) g;
	(void) procent;
}

#endif /* _GDISP_LLD_BOARD_H */


