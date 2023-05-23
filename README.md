# MVSX Hardware
- Quadcore Arm Cortex A7 @ 1ghz 
- 256MB Ram
- Running Linux Kernel 3.4.0

#ASP Hardware
- Quadcore Arm Cortex A7 @ 800mhz 
- 256MB Ram
- Running Linux Kernel 3.4.0

## AES
Advanced Executable Starter is a program. 
It supports the MVSX and ASP.

It can:
1. Launch any process as specified in its config.ini file. It supports hot swapping the USB for
fast iteration, and it can kill that process by pressing P2 start. (Useful if the process deadlocks)

2. Tests / demos all features of the MVSX / ASP.

## Compiling
Binaries targeting it should target: Arm5te architecture Soft float (no VSP) 
It should use the arm-2011.09 gnueabi (not gnueabihf) toolchain.
The build environment should be x86 Ubuntu with the arm-2011.09 gnueabi (not gnueabihf) toolchain.

Install Lubuntu (really Ubuntu 18.04) into a VM. 

You can confirm the build is correct by running `$ readelf -A [binaryFile]` and confirming: 
- Attribute Section: aeabi File Attributes 
- Tag_CPU_name: "5TE" 
- Tag_CPU_arch: v5TE 
- Tag_ARM_ISA_use: Yes 
- Tag_THUMB_ISA_use: Thumb-1 
- Tag_ABI_PCS_wchar_t: 4 
- Tag_ABI_FP_denormal: Needed 
- Tag_ABI_FP_exceptions: Needed 
- Tag_ABI_FP_number_model: IEEE 754 
- Tag_ABI_align_needed: 8-byte 
- Tag_ABI_align_preserved: 8-byte, except leaf SP 
- Tag_ABI_enum_size: int 
- Tag_DIV_use: Not allowed

Note the absence of VFP support.

## Using
1. Format a USB to exfat.
2. Create a folder called "assets" in the root of the USB
3. From this repo, copy "assets/fonts" and "assets/images" into "assets" on the USB.
4. Copy boot-usb.sh to the root of the USB
5. Copy AES to the root of the USB
6. Copy config.ini to the root of the USB
7. Update config.ini to launch the exectuable (with args) of your choice
