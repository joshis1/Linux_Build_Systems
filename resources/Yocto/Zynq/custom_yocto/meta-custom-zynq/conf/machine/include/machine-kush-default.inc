# Default Kush BSP Machine settings

MACHINE_FEATURES_BACKFILL_CONSIDERED += "rtc"

# File System Configuration
IMAGE_FSTYPES ?= "tar.gz cpio cpio.gz.u-boot"

# Kernel Configuration
KUSH_DEFAULT_KERNEL := "linux-zinc"
PREFERRED_PROVIDER_virtual/kernel ??= "${KUSH_DEFAULT_KERNEL}"

# U-Boot Configuration
KUSH_DEFAULT_UBOOT := "u-boot-kush"
PREFERRED_PROVIDER_virtual/bootloader ??= "${KUSH_DEFAULT_UBOOT}"
PREFERRED_PROVIDER_virtual/boot-bin ??= "${PREFERRED_PROVIDER_virtual/bootloader}"

UBOOT_SUFFIX ?= "img"

UBOOT_BINARY ?= "u-boot.${UBOOT_SUFFIX}"
UBOOT_ELF ?= "u-boot"
UBOOT_ELF_zynq ?= "u-boot"
UBOOT_ELF_aarch64 ?= "u-boot.elf"
UBOOT_ELF_versal ?= "u-boot.elf"


XSERVER ?= " \
	xserver-xorg \
	xf86-input-evdev \
	xf86-input-mouse \
	xf86-input-keyboard \
	xf86-video-fbdev \
	${XSERVER_EXT} \
	"

IMAGE_BOOT_FILES ?= "${@get_default_image_boot_files(d)}"

def get_default_image_boot_files(d):
    files = []

    # kernel images
    kerneltypes = set((d.getVar("KERNEL_IMAGETYPE") or "").split())
    kerneltypes |= set((d.getVar("KERNEL_IMAGETYPES") or "").split())
    for i in kerneltypes:
        files.append(i)

    # u-boot image
    if d.getVar("UBOOT_BINARY"):
        files.append(d.getVar("UBOOT_BINARY"))

    # device trees (device-tree only), these are first as they are likely desired over the kernel ones
    if "device-tree" in (d.getVar("MACHINE_ESSENTIAL_EXTRA_RDEPENDS") or ""):
        files.append("devicetree/*.dtb")


    # device trees (kernel only)
    if d.getVar("KERNEL_DEVICETREE"):
        dtbs = d.getVar("KERNEL_DEVICETREE").split(" ")
        dtbs = [os.path.basename(d) for d in dtbs]
        for dtb in dtbs:
            files.append(dtb)

    return " ".join(files)

XSERVER_EXT ?= ""
XSERVER_EXT_zynqmp ?= "xf86-video-armsoc"

# For MicroBlaze default all microblaze machines to use GDB 7.7.1 (for gdbserver/gdb)
PREFERRED_VERSION_gdb_microblaze = "7.7.1"

FPGA_MNGR_RECONFIG_ENABLE ?= "${@bb.utils.contains('IMAGE_FEATURES', 'fpga-manager', '1', '0', d)}"
