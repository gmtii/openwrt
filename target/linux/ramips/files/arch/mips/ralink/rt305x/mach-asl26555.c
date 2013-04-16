/*
 *  Alpha ASL 26555 802.11n router 
 *
 *  Copyright (C) 2012 <>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>

#include <asm/mach-ralink/machine.h>
#include <asm/mach-ralink/dev-gpio-buttons.h>
#include <asm/mach-ralink/dev-gpio-leds.h>
#include <asm/mach-ralink/rt305x.h>
#include <asm/mach-ralink/rt305x_regs.h>
//#include <asm/sizes.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>


#include "devices.h"

#define ASL26555_GPIO_BUTTON_RESET	10
#define ASL26555_GPIO_BUTTON_WPS	12

#define ASL26555_GPIO_LED_USB		0
#define ASL26555_GPIO_LED_WIFI		7
#define ASL26555_GPIO_LED_POWER	9
#define ASL26555_GPIO_LED_STATUS	8
#define ASL26555_GPIO_LED_ADSL		11

#define ASL26555_KEYS_POLL_INTERVAL 20
#define ASL26555_KEYS_DEBOUNCE_INTERVAL	(3 * ASL26555_KEYS_POLL_INTERVAL)

const struct flash_platform_data asl26555_flash = {
	.type		= "s25sl12801",
};

struct spi_board_info asl26555_spi_slave_info[] __initdata = {
	{
		.modalias	= "m25p80",
		.platform_data	= &asl26555_flash,
		.irq		= -1,
		.max_speed_hz	= 10000000,
		.bus_num	= 0,
		.chip_select	= 0,
	},
};

static struct gpio_keys_button asl26555_gpio_buttons[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = ASL26555_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= ASL26555_GPIO_BUTTON_RESET,
		.active_low	= 0,
	}, {
		.desc		= "wps",
		.type		= EV_KEY,
		.code		= KEY_WPS_BUTTON,
		.debounce_interval = ASL26555_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= ASL26555_GPIO_BUTTON_WPS,
		.active_low	= 0,
	}
};

static struct gpio_led asl26555_leds_gpio[] __initdata = {
	{
		.name		= "asl26555:green:wifi",
		.gpio		= ASL26555_GPIO_LED_WIFI,
		.active_low	= 0,
	}, {
		.name		= "asl26555:red:power",
		.gpio		= ASL26555_GPIO_LED_POWER,
		.active_low	= 0,
	}, {
		.name		= "asl26555:green:status",
		.gpio		= ASL26555_GPIO_LED_STATUS,
		.active_low	= 0,
	}, {
		.name		= "asl26555:green:adsl",
		.gpio		= ASL26555_GPIO_LED_ADSL,
		.active_low	= 0,
	}, {
		.name		= "asl26555:green:usb",
		.gpio		= ASL26555_GPIO_LED_USB,
		.active_low	= 0,
	}
};

static void __init asl26555_init(void)
{
	rt305x_gpio_init((RT305X_GPIO_MODE_GPIO << RT305X_GPIO_MODE_UART0_SHIFT) | RT305X_GPIO_MODE_I2C);
	rt305x_register_spi(asl26555_spi_slave_info,
			    ARRAY_SIZE(asl26555_spi_slave_info));
	rt305x_esw_data.vlan_config = RT305X_ESW_VLAN_CONFIG_WLLLL;
	rt305x_register_ethernet();
	ramips_register_gpio_leds(-1, ARRAY_SIZE(asl26555_leds_gpio),
				  asl26555_leds_gpio);
	ramips_register_gpio_buttons(-1, ASL26555_KEYS_POLL_INTERVAL,
				     ARRAY_SIZE(asl26555_gpio_buttons),
				     asl26555_gpio_buttons);
	rt305x_register_wifi();
	rt305x_register_wdt();
	rt305x_register_usb();
	rt305x_register_i2c();
}

MIPS_MACHINE(RAMIPS_MACH_ASL26555, "ASL26555", "Alpha ASL26555",
	     asl26555_init);
