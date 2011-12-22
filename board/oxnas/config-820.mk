# U-boot configuration for the OX820 platforms
CROSS_COMPILE=

# U-boot's location when executing from DDR
TEXT_BASE = 0x60e00000

RPSCLK ?= 6250000

FPGA ?= 0

PROBE_MEM_SIZE ?= 0

# Memory size in megabytes if probing is not enabled
MEM_SIZE ?= 128

# Overwrite the mem= Linux bootarg stored in the environment with the amount
# determined from probing or MEM_SIZE above
SET_LINUX_MEM_SIZE ?= 1

USE_SATA ?= 1
USE_SATA_ENV ?= 1
USE_FLASH ?= 0
USE_OTP ?= 0
USE_OTP_MAC ?=0

USE_SPI ?= 0
USE_NAND ?= 1
USE_NAND_ENV ?= 0
USE_7715_PINS ?=0
USE_USB ?=0

LINUX_ROOT_RAIDED ?= 0

USE_EXTERNAL_UART ?= 0
INTERNAL_UART ?= 1

# uses leon counted time to update system time on power-up
USE_LEON_TIME_COUNT ?= 1

# allow the use of alternative device footprint
ifneq (USE_7715_PINS,0)
PLATFORM_CPPFLAGS += -DCONFIG_7715
endif

# This converts Make variables into C preprocessor #defines
PLATFORM_CPPFLAGS += -DNAS_VERSION=$(NAS_VERSION)\
		     -DLINUX_ROOT_RAIDED=$(LINUX_ROOT_RAIDED)\
		     -DMEM_SIZE=$(MEM_SIZE)\
		     -DPROBE_MEM_SIZE=$(PROBE_MEM_SIZE)\
		     -DSET_LINUX_MEM_SIZE=$(SET_LINUX_MEM_SIZE)\
		     -DFPGA=$(FPGA)\
		     -DINTERNAL_UART=$(INTERNAL_UART)\
		     -DUSE_EXTERNAL_UART=$(USE_EXTERNAL_UART)\
		     -DRPSCLK=$(RPSCLK)\
		     -DUSE_SATA=$(USE_SATA)\
		     -DUSE_SATA_ENV=$(USE_SATA_ENV)\
		     -DUSE_FLASH=$(USE_FLASH)\
		     -DUSE_LEON_TIME_COUNT=$(USE_LEON_TIME_COUNT)\
		     -DUSE_SPI=$(USE_SPI)\
		     -DUSE_NAND=$(USE_NAND)\
		     -DUSE_OTP=$(USE_OTP) \
		     -DUSE_OTP_MAC=$(USE_OTP_MAC) \
			 -DUSE_USB=$(USE_USB)

ifdef SDK_BUILD_NAND_BOOT
PLATFORM_CPPFLAGS += -DCFG_ENV_IS_NOWHERE -DSDK_BUILD_NAND_BOOT
endif

