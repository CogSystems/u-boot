/*
 * USB armory MkI board configuration settings
 * http://inversepath.com/usbarmory
 *
 * Copyright (C) 2015, Inverse Path
 * Andrej Rosano <andrej@inversepath.com>
 *
 * SPDX-License-Identifier:|____GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MX53
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_OF_LIBFDT
#define CONFIG_SYS_GENERIC_BOARD
#define CONFIG_MXC_GPIO

#include <asm/arch/imx-regs.h>
#include <config_cmd_default.h>

#include <config_distro_defaults.h>

/* U-Boot commands */
#define CONFIG_CMD_MEMTEST
#undef CONFIG_CMD_IMLS
#define CONFIG_CMD_ELF

/* Tftp port */
#define CONFIG_TFTP_PORT

/* USB Downloads */
#define CONFIG_CI_UDC
#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_GADGET
#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_GADGET_MASS_STORAGE
#define CONFIG_USBDOWNLOAD_GADGET
#define CONFIG_USB_GADGET_VBUS_DRAW     2
#define CONFIG_USB_ETHER
#define CONFIG_USB_ETH_CDC
#define CONFIG_G_DNL_VENDOR_NUM         0x0525
#define CONFIG_G_DNL_PRODUCT_NUM        0xa4a5
#define CONFIG_G_DNL_MANUFACTURER       "FSL"
#define CONFIG_SYS_CACHELINE_SIZE       64
/* Fastboot */
#define CONFIG_CMD_FASTBOOT
#define CONFIG_IMAGE_FORMAT_LEGACY
#define CONFIG_ANDROID_BOOT_IMAGE
#define CONFIG_USB_FASTBOOT_BUF_ADDR	CONFIG_SYS_LOAD_ADDR
#define CONFIG_USB_FASTBOOT_BUF_SIZE	0x07000000

/* USB Device Firmware Update support */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_DFU_FUNCTION
#define CONFIG_CMD_DFU
#define DFU_ALT_INFO_NAND ""
#define CONFIG_DFU_RAM
#define DFU_ALT_INFO_RAM \
	"dfu_alt_info_ram=" \
	"kernel ram 0x80200000 0xD80000;" \
	"fdt ram 0x80F80000 0x80000;" \
	"ramdisk ram 0x81000000 0x4000000\0"
#define DFUARGS \
	"dfu_alt_info_emmc=rawemmc raw 0 3751936\0" \
	DFU_ALT_INFO_MMC \
	DFU_ALT_INFO_RAM \
	DFU_ALT_INFO_NAND
#endif

/* U-Boot environment */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_NO_FLASH
#define CONFIG_ENV_OFFSET	(6 * 64 * 1024)
#define CONFIG_ENV_SIZE		(8 * 1024)
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV	0

/* U-Boot general configurations */
#define CONFIG_SYS_CBSIZE	512
#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS	16
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE

/* UART */
#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONFIG_CONS_INDEX	1
#define CONFIG_BAUDRATE		115200

/* SD/MMC */
#define CONFIG_CMD_MMC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_ESDHC_NUM	1
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC

/* USB */
#define CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX5
#define CONFIG_USB_STORAGE
/*#define CONFIG_MXC_USB_PORT	1*/
#define CONFIG_MXC_USB_PORT	0
/*#define CONFIG_MXC_USB_PORTSC	(PORT_PTS_UTMI | PORT_PTS_PTW)*/
/*#define CONFIG_MXC_USB_FLAGS	0*/
#define	CONFIG_MXC_USB_PORTSC	(1 << 28)
#define  CONFIG_MXC_USB_FLAGS    MXC_EHCI_INTERNAL_PHY

/* I2C */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC

/* Fuse */
#define CONFIG_CMD_FUSE
#define CONFIG_FSL_IIM

/* Linux boot */
#define CONFIG_LOADADDR		0x88000000
#define CONFIG_SYS_TEXT_BASE	0x77800000
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR
#define CONFIG_HOSTNAME		usbarmory
#define CONFIG_BOOTCOMMAND						\
	"run distro_bootcmd; "						\
	"setenv bootargs console=${console} ${bootargs_default}; "	\
	"ext2load mmc 0:1 ${kernel_addr_r} /boot/uImage; "		\
	"ext2load mmc 0:1 ${fdt_addr_r} /boot/${fdtfile}; "		\
	"bootm ${kernel_addr_r} - ${fdt_addr_r}"

#define BOOT_TARGET_DEVICES(func) func(MMC, mmc, 0)

#include <config_distro_bootcmd.h>

#define MEM_LAYOUT_ENV_SETTINGS			\
	"kernel_addr_r=0x70800000\0"		\
	"fdt_addr_r=0x71000000\0"		\
	"scriptaddr=0x70800000\0"		\
	"pxefile_addr_r=0x70800000\0"		\
	"ramdisk_addr_r=0x73000000\0"

#define CONFIG_EXTRA_ENV_SETTINGS				\
	MEM_LAYOUT_ENV_SETTINGS					\
	"bootargs_default=root=/dev/mmcblk0p1 rootwait rw\0"	\
	"fdtfile=imx53-usbarmory.dtb\0"				\
	"console=ttymxc0,115200\0"				\
	BOOTENV

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			CSD0_BASE_ADDR
#define PHYS_SDRAM_SIZE			(gd->ram_size)

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_SYS_MEMTEST_START	0x70000000
#define CONFIG_SYS_MEMTEST_END		0x90000000

#define CONFIG_SYS_MALLOC_LEN		(10 * 1024 * 1024)

#endif				/* __CONFIG_H */
