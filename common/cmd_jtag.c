/*
 * Change GPIO mux to enable ARM Jtag on RPi2/BCM2836
 *
 * (C) Copyright 2012-2013,2015 Stephen Warren
 * Copyright (c) 2015 Cog Systems
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <command.h>
#include <errno.h>
#include <dm.h>
#include <asm/gpio.h>
#include <asm/io.h>


/* A bunch of stuff lifted from pinctrl-bcm2835.c in Linux */

/* GPIO register offsets */
#define GPFSEL0     0x0 /* Function Select */
#define GPSET0      0x1c    /* Pin Output Set */
#define GPCLR0      0x28    /* Pin Output Clear */
#define GPLEV0      0x34    /* Pin Level */
#define GPEDS0      0x40    /* Pin Event Detect Status */
#define GPREN0      0x4c    /* Pin Rising Edge Detect Enable */
#define GPFEN0      0x58    /* Pin Falling Edge Detect Enable */
#define GPHEN0      0x64    /* Pin High Detect Enable */
#define GPLEN0      0x70    /* Pin Low Detect Enable */
#define GPAREN0     0x7c    /* Pin Async Rising Edge Detect */
#define GPAFEN0     0x88    /* Pin Async Falling Edge Detect */
#define GPPUD       0x94    /* Pin Pull-up/down Enable */
#define GPPUDCLK0   0x98    /* Pin Pull-up/down Enable Clock */

#define FSEL_REG(p)     (GPFSEL0 + (((p) / 10) * 4))
#define FSEL_SHIFT(p)       (((p) % 10) * 3)
#define GPIO_REG_OFFSET(p)  ((p) / 32)
#define GPIO_REG_SHIFT(p)   ((p) % 32)

enum bcm2835_fsel {
    BCM2835_FSEL_GPIO_IN = 0,
    BCM2835_FSEL_GPIO_OUT = 1,
    BCM2835_FSEL_ALT0 = 4,
    BCM2835_FSEL_ALT1 = 5,
    BCM2835_FSEL_ALT2 = 6,
    BCM2835_FSEL_ALT3 = 7,
    BCM2835_FSEL_ALT4 = 3,
    BCM2835_FSEL_ALT5 = 2,
    BCM2835_FSEL_COUNT = 8,
    BCM2835_FSEL_MASK = 0x7,
};

static inline uint32_t bcm2835_gpio_rd(uint32_t reg)
{
    return readl((void *)(BCM2835_GPIO_BASE + reg));
}

static inline void bcm2835_gpio_wr(uint32_t reg, uint32_t val)
{
    writel(val, (void *)(BCM2835_GPIO_BASE + reg));
}

static inline void bcm2835_pinctrl_fsel_set(int pin, enum bcm2835_fsel fsel)
{
    uint32_t val = bcm2835_gpio_rd(FSEL_REG(pin));
    enum bcm2835_fsel cur = (val >> FSEL_SHIFT(pin)) & BCM2835_FSEL_MASK;

    if (cur == fsel)
        return;

    if (cur != BCM2835_FSEL_GPIO_IN && fsel != BCM2835_FSEL_GPIO_IN) {
        /* always transition through GPIO_IN */
        val &= ~(BCM2835_FSEL_MASK << FSEL_SHIFT(pin));
        val |= BCM2835_FSEL_GPIO_IN << FSEL_SHIFT(pin);
        bcm2835_gpio_wr(FSEL_REG(pin), val);
    }

    val &= ~(BCM2835_FSEL_MASK << FSEL_SHIFT(pin));
    val |= fsel << FSEL_SHIFT(pin);

    bcm2835_gpio_wr(FSEL_REG(pin), val);
}


static int do_arm_jtag_enable(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    int result = 0;

    #if 0
    // For rpi2.
    bcm2835_pinctrl_fsel_set(26, BCM2835_FSEL_ALT4);
    #else
    // For earlier rpi with limited gpios hooked up.
    // Cog GPIO breakout board is wired this way.
    bcm2835_pinctrl_fsel_set(4,  BCM2835_FSEL_ALT5);
    #endif
    bcm2835_pinctrl_fsel_set(22, BCM2835_FSEL_ALT4);
    bcm2835_pinctrl_fsel_set(23, BCM2835_FSEL_ALT4);
    bcm2835_pinctrl_fsel_set(24, BCM2835_FSEL_ALT4);
    bcm2835_pinctrl_fsel_set(25, BCM2835_FSEL_ALT4);
    bcm2835_pinctrl_fsel_set(27, BCM2835_FSEL_ALT4);

    return result;
}

U_BOOT_CMD( jtag_enable,    /*command name*/
            1,              /*max args */
            0,              /* rep */
            do_arm_jtag_enable, /* function to call */
       "enable ARM JTAG pins in GPIO mux.", /* usage */
       "jtag_enable - enable ARM JTAG pins in GPIO mux." /* help */ );
