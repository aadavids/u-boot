/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * npi_common.h
 *
 * Environment variable definitions for MMC/SD on TI boards.
 */

#ifndef __CONFIG_NPI_COMMON_H__
#define __CONFIG_NPI_COMMON_H__

#define DEFAULT_MMC_ARGS \
	"mmcdev=0\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"finduuid=part uuid ${devtype} ${bootpart} uuid\0" \
	"args_mmc=run finduuid;setenv bootargs console=${console} " \
		"${cape_disable} " \
		"${cape_enable} " \
		"${cape_uboot} " \
		"root=PARTUUID=${uuid} ro " \
		"rootfstype=${mmcrootfstype} " \
		"${uboot_detected_capes} " \
		"${cmdline}\0" \
	"args_mmc_old=setenv bootargs console=${console} " \
		"${optargs} " \
		"${cape_disable} " \
		"${cape_enable} " \
		"${cape_uboot} " \
		"root=${oldroot} ro " \
		"rootfstype=${mmcrootfstype} " \
		"${uboot_detected_capes} " \
		"${cmdline}\0" \
	"args_mmc_uuid=setenv bootargs console=${console} " \
		"${optargs} " \
		"${cape_disable} " \
		"${cape_enable} " \
		"${cape_uboot} " \
		"root=UUID=${uuid} ro " \
		"rootfstype=${mmcrootfstype} " \
		"${uboot_detected_capes} " \
		"${cmdline}\0" \
	"args_uenv_root=setenv bootargs console=${console} " \
		"${optargs} " \
		"${cape_disable} " \
		"${cape_enable} " \
		"${cape_uboot} " \
		"root=${uenv_root} ro " \
		"rootfstype=${mmcrootfstype} " \
		"${uboot_detected_capes} " \
		"${cmdline}\0" \
	"args_netinstall=setenv bootargs ${netinstall_bootargs} " \
		"${optargs} " \
		"${cape_disable} " \
		"${cape_enable} " \
		"${cape_uboot} " \
		"root=/dev/ram rw " \
		"${uboot_detected_capes} " \
		"${cmdline}\0" \
	"script=boot.scr\0" \
	"scriptfile=${script}\0" \
	"loadbootscript=load ${devtype} ${bootpart} ${loadaddr} ${scriptfile};\0" \
	"bootscript=echo Running bootscript from mmc${bootpart} ...; " \
		"source ${loadaddr}\0" \
	"bootenvfile=uEnv.txt\0" \
	"bootenv=uEnv.txt\0" \
	"importbootenv=echo Importing environment from ${devtype} ...; " \
		"env import -t ${loadaddr} ${filesize}\0" \
	"loadbootenv=load ${devtype} ${bootpart} ${loadaddr} ${bootenvfile}\0" \
	"loadimage=load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadrd=load ${devtype} ${bootpart} ${rdaddr} ${bootdir}/${rdfile}; setenv rdsize ${filesize}\0" \
	"loadfdt=echo loading ${fdtdir}/${fdtfile} ...; load ${devtype} ${bootpart} ${fdtaddr} ${fdtdir}/${fdtfile}\0" \
	"loadoverlay=echo uboot_overlays: loading ${uboot_overlay} ...; " \
		"load ${devtype} ${bootpart} ${rdaddr} ${uboot_overlay}; " \
		"fdt addr ${fdtaddr}; fdt resize ${fdt_buffer}; " \
		"fdt apply ${rdaddr}; fdt resize ${fdt_buffer};\0" \
	"virtualloadoverlay=if test -e ${devtype} ${bootpart} ${uboot_overlay}; then " \
				"run loadoverlay;" \
			"else " \
				"echo uboot_overlays: unable to find [${devtype} ${bootpart} ${uboot_overlay}]...;" \
			"fi;\0" \
	"capeloadoverlay=if test -e ${devtype} ${bootpart} ${uboot_overlay}; then " \
				"run loadoverlay;" \
				"setenv cape_uboot bone_capemgr.uboot_capemgr_enabled=1; " \
			"else " \
				"echo uboot_overlays: unable to find [${devtype} ${bootpart} ${uboot_overlay}]...;" \
			"fi;\0" \
	"failumsboot=echo; echo FAILSAFE: U-Boot UMS (USB Mass Storage) enabled, media now available over the usb slave port ...; " \
		"ums 0 ${devtype} 1;\0" \
	"envboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadbootscript; then " \
				"run bootscript;" \
			"else " \
				"if run loadbootenv; then " \
					"echo Loaded env from ${bootenvfile};" \
					"run importbootenv;" \
				"fi;" \
				"if test -n $uenvcmd; then " \
					"echo Running uenvcmd ...;" \
					"run uenvcmd;" \
				"fi;" \
			"fi;" \
		"fi;\0" \
	"mmcloados=run args_mmc; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"if test -n ${uname_r}; then " \
					"bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}; " \
				"else " \
					"bootz ${loadaddr} - ${fdtaddr}; " \
				"fi; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"mmcboot=mmc dev ${mmcdev}; " \
		"setenv devnum ${mmcdev}; " \
		"setenv devtype mmc; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadimage; then " \
				"if test ${boot_fit} -eq 1; then " \
					"run loadfit; " \
				"else " \
					"run mmcloados;" \
				"fi;" \
			"fi;" \
		"fi;\0"


#define NPI_BOOT \
	"boot=${devtype} dev ${mmcdev}; " \
		"if ${devtype} rescan; then " \
			"gpio set 54;" \
			"setenv bootpart ${mmcdev}:1; " \
			"if test -e ${devtype} ${bootpart} /etc/fstab; then " \
				"setenv mmcpart 1;" \
			"fi; " \
			"echo Checking for: /boot/uEnv.txt ...;" \
			"if test -e ${devtype} ${bootpart} /boot/uEnv.txt; then " \
				"if run loadbootenv; then " \
					"gpio set 55;" \
					"echo Loaded environment from /boot/uEnv.txt;" \
					"run importbootenv;" \
				"fi;" \
				"echo Checking if uenvcmd is set ...;" \
				"if test -n ${uenvcmd}; then " \
					"gpio set 56; " \
					"echo Running uenvcmd ...;" \
					"run uenvcmd;" \
				"fi;" \
				"echo Checking if client_ip is set ...;" \
				"if test -n ${client_ip}; then " \
					"if test -n ${dtb}; then " \
						"setenv fdtfile ${dtb};" \
						"echo using ${fdtfile} ...;" \
					"fi;" \
					"gpio set 56; " \
					"if test -n ${uname_r}; then " \
						"echo Running nfsboot_uname_r ...;" \
						"run nfsboot_uname_r;" \
					"fi;" \
					"echo Running nfsboot ...;" \
					"run nfsboot;" \
				"fi;" \
			"fi; " \
			"echo Checking for: /${script} ...;" \
			"if test -e ${devtype} ${bootpart} /${script}; then " \
				"gpio set 55;" \
				"setenv scriptfile ${script};" \
				"run loadbootscript;" \
				"echo Loaded script from ${scriptfile};" \
				"gpio set 56; " \
				"run bootscript;" \
			"fi; " \
			"echo Checking for: /boot/${script} ...;" \
			"if test -e ${devtype} ${bootpart} /boot/${script}; then " \
				"gpio set 55;" \
				"setenv scriptfile /boot/${script};" \
				"run loadbootscript;" \
				"echo Loaded script from ${scriptfile};" \
				"gpio set 56; " \
				"run bootscript;" \
			"fi; " \
			"echo debug: get rootpoart: ${rootpart} ...;" \
			"if test -n ${rootpart}; then " \
				"echo Checking for: /boot/uEnv.txt ...;" \
				"setenv mmcpart ${rootpart};" \
				"setenv bootpart ${mmcdev}:${mmcpart};" \
				"if test -e ${devtype} ${bootpart} /boot/uEnv.txt; then " \
					"gpio set 55;" \
					"load ${devtype} ${bootpart} ${loadaddr} /boot/uEnv.txt;" \
					"env import -t ${loadaddr} ${filesize};" \
					"echo Loaded environment from /boot/uEnv.txt;" \
					"if test -n ${dtb}; then " \
						"echo debug: [dtb=${dtb}] ... ;" \
						"setenv fdtfile ${dtb};" \
						"echo Using: dtb=${fdtfile} ...;" \
					"fi;" \
					"echo Checking if uname_r is set in /boot/uEnv.txt...;" \
					"if test -n ${uname_r}; then " \
						"gpio set 56; " \
						"setenv oldroot /dev/mmcblk${mmcdev}p${mmcpart};" \
						"echo Running uname_boot ...;" \
						"run uname_boot;" \
					"fi;" \
				"fi;" \
			"fi;" \
		"fi;\0" \

#define EEWIKI_UNAME_BOOT \
	"uname_boot="\
		"setenv bootdir /boot; " \
		"setenv bootpart ${mmcdev}:${rootpart};" \
		"setenv oldroot /dev/mmcblk${mmcdev}p${rootpart};" \
		"setenv bootfile vmlinuz-${uname_r}; " \
		"if test -e ${devtype} ${bootpart} ${bootdir}/${bootfile}; then " \
			"echo loading ${bootdir}/${bootfile} ...; "\
			"run loadimage;" \
			"setenv fdtdir /boot/dtbs/${uname_r}; " \
			"echo debug: [enable_uboot_overlays=${enable_uboot_overlays}] ... ;" \
			"if test -n ${enable_uboot_overlays}; then " \
				"echo debug: [enable_uboot_cape_universal=${enable_uboot_cape_universal}] ... ;" \
				"if test -n ${enable_uboot_cape_universal}; then " \
					"echo debug: [uboot_base_dtb_univ=${uboot_base_dtb_univ}] ... ;" \
					"if test -n ${uboot_base_dtb_univ}; then " \
						"echo uboot_overlays: [uboot_base_dtb=${uboot_base_dtb_univ}] ... ;" \
						"if test -e ${devtype} ${bootpart} ${fdtdir}/${uboot_base_dtb_univ}; then " \
							"setenv fdtfile ${uboot_base_dtb_univ};" \
							"echo uboot_overlays: Switching too: dtb=${fdtfile} ...;" \
							"if test -n ${uboot_try_cape_universal}; then " \
								"env delete -f uboot_try_cape_universal; " \
							"fi;" \
							"setenv cape_uboot bone_capemgr.uboot_capemgr_enabled=1; " \
						"else " \
							"echo debug: unable to find [${uboot_base_dtb_univ}] using [${uboot_base_dtb}] instead ... ;" \
							"echo debug: [uboot_base_dtb_univ=${uboot_base_dtb}] ... ;" \
							"if test -n ${uboot_base_dtb}; then " \
								"echo uboot_overlays: [uboot_base_dtb=${uboot_base_dtb}] ... ;" \
								"if test -e ${devtype} ${bootpart} ${fdtdir}/${uboot_base_dtb}; then " \
									"setenv fdtfile ${uboot_base_dtb};" \
									"echo uboot_overlays: Switching too: dtb=${fdtfile} ...;" \
								"fi;" \
							"fi;" \
						"fi;" \
					"fi;" \
				"else " \
					"echo debug: [uboot_base_dtb_univ=${uboot_base_dtb}] ... ;" \
					"if test -n ${uboot_base_dtb}; then " \
						"echo uboot_overlays: [uboot_base_dtb=${uboot_base_dtb}] ... ;" \
						"if test -e ${devtype} ${bootpart} ${fdtdir}/${uboot_base_dtb}; then " \
							"setenv fdtfile ${uboot_base_dtb};" \
							"echo uboot_overlays: Switching too: dtb=${fdtfile} ...;" \
						"fi;" \
					"fi;" \
				"fi;" \
			"fi;" \
			"if test -e ${devtype} ${bootpart} ${fdtdir}/${fdtfile}; then " \
				"run loadfdt;" \
			"else " \
				"setenv fdtdir /usr/lib/linux-image-${uname_r}; " \
				"if test -e ${devtype} ${bootpart} ${fdtdir}/${fdtfile}; then " \
					"run loadfdt;" \
				"else " \
					"setenv fdtdir /lib/firmware/${uname_r}/device-tree; " \
					"if test -e ${devtype} ${bootpart} ${fdtdir}/${fdtfile}; then " \
						"run loadfdt;" \
					"else " \
						"setenv fdtdir /boot/dtb-${uname_r}; " \
						"if test -e ${devtype} ${bootpart} ${fdtdir}/${fdtfile}; then " \
							"run loadfdt;" \
						"else " \
							"setenv fdtdir /boot/dtbs; " \
							"if test -e ${devtype} ${bootpart} ${fdtdir}/${fdtfile}; then " \
								"run loadfdt;" \
							"else " \
								"setenv fdtdir /boot/dtb; " \
								"if test -e ${devtype} ${bootpart} ${fdtdir}/${fdtfile}; then " \
									"run loadfdt;" \
								"else " \
									"setenv fdtdir /boot; " \
									"if test -e ${devtype} ${bootpart} ${fdtdir}/${fdtfile}; then " \
										"run loadfdt;" \
									"else " \
										"if test -e ${devtype} ${bootpart} ${fdtfile}; then " \
											"run loadfdt;" \
										"else " \
											"echo; echo unable to find [dtb=${fdtfile}] did you name it correctly? ...; " \
											"run failumsboot;" \
										"fi;" \
									"fi;" \
								"fi;" \
							"fi;" \
						"fi;" \
					"fi;" \
				"fi;" \
			"fi; " \
			"if test -n ${enable_uboot_overlays}; then " \
				"setenv fdt_buffer 0x60000;" \
				"if test -n ${uboot_fdt_buffer}; then " \
					"setenv fdt_buffer ${uboot_fdt_buffer};" \
				"fi;" \
				"echo uboot_overlays: [fdt_buffer=${fdt_buffer}] ... ;" \
				"if test -n ${uboot_silicon}; then " \
					"setenv uboot_overlay ${uboot_silicon}; " \
					"run virtualloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_model}; then " \
					"setenv uboot_overlay ${uboot_model}; " \
					"run virtualloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr0}; then " \
					"if test -n ${disable_uboot_overlay_addr0}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_overlay_addr0}] disabled by /boot/uEnv.txt [disable_uboot_overlay_addr0=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_overlay_addr0}; " \
						"run capeloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr1}; then " \
					"if test -n ${disable_uboot_overlay_addr1}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_overlay_addr1}] disabled by /boot/uEnv.txt [disable_uboot_overlay_addr1=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_overlay_addr1}; " \
						"run capeloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr2}; then " \
					"if test -n ${disable_uboot_overlay_addr2}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_overlay_addr2}] disabled by /boot/uEnv.txt [disable_uboot_overlay_addr2=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_overlay_addr2}; " \
						"run capeloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr3}; then " \
					"if test -n ${disable_uboot_overlay_addr3}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_overlay_addr3}] disabled by /boot/uEnv.txt [disable_uboot_overlay_addr3=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_overlay_addr3}; " \
						"run capeloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr4}; then " \
					"setenv uboot_overlay ${uboot_overlay_addr4}; " \
					"run capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr5}; then " \
					"setenv uboot_overlay ${uboot_overlay_addr5}; " \
					"run capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr6}; then " \
					"setenv uboot_overlay ${uboot_overlay_addr6}; " \
					"run capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_overlay_addr7}; then " \
					"setenv uboot_overlay ${uboot_overlay_addr7}; " \
					"run capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_emmc}; then " \
					"if test -n ${disable_uboot_overlay_emmc}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_emmc}] disabled by /boot/uEnv.txt [disable_uboot_overlay_emmc=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_emmc}; " \
						"run virtualloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_video}; then " \
					"if test -n ${disable_uboot_overlay_video}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_video}] disabled by /boot/uEnv.txt [disable_uboot_overlay_video=1]...;" \
					"else " \
						"if test -n ${disable_uboot_overlay_audio}; then " \
							"echo uboot_overlays: uboot loading of [${uboot_video}] disabled by /boot/uEnv.txt [disable_uboot_overlay_audio=1]...;" \
							"setenv uboot_overlay ${uboot_video_naudio}; " \
							"run virtualloadoverlay;" \
						"else " \
							"setenv uboot_overlay ${uboot_video}; " \
							"run virtualloadoverlay;" \
						"fi;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_wireless}; then " \
					"if test -n ${disable_uboot_overlay_wireless}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_wireless}] disabled by /boot/uEnv.txt [disable_uboot_overlay_wireless=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_wireless}; " \
						"run virtualloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_adc}; then " \
					"if test -n ${disable_uboot_overlay_adc}; then " \
						"echo uboot_overlays: uboot loading of [${uboot_adc}] disabled by /boot/uEnv.txt [disable_uboot_overlay_adc=1]...;" \
					"else " \
						"setenv uboot_overlay ${uboot_adc}; " \
						"run virtualloadoverlay;" \
					"fi;" \
				"fi;" \
				"if test -n ${uboot_overlay_pru}; then " \
					"setenv uboot_overlay ${uboot_overlay_pru}; " \
					"run virtualloadoverlay;" \
				"fi;" \
				"if test -n ${dtb_overlay}; then " \
					"setenv uboot_overlay ${dtb_overlay}; " \
					"echo uboot_overlays: [dtb_overlay=${uboot_overlay}] ... ;" \
					"run capeloadoverlay;" \
				"fi;" \
				"if test -n ${uboot_detected_capes}; then " \
					"echo uboot_overlays: [uboot_detected_capes=${uboot_detected_capes_addr0}${uboot_detected_capes_addr1}${uboot_detected_capes_addr2}${uboot_detected_capes_addr3}] ... ;" \
					"setenv uboot_detected_capes uboot_detected_capes=${uboot_detected_capes_addr0}${uboot_detected_capes_addr1}${uboot_detected_capes_addr2}${uboot_detected_capes_addr3}; " \
				"fi;" \
				"if test -n ${uboot_try_cape_universal}; then " \
					"if test -n ${enable_uboot_cape_universal}; then " \
						"if test -n ${cape_uboot}; then " \
							"echo uboot_overlays: cape universal disabled, external cape enabled or detected...;" \
						"else " \
							"if test -n ${uboot_cape_universal_bbb}; then " \
								"if test -n ${disable_uboot_overlay_emmc}; then " \
									"if test -n ${disable_uboot_overlay_video}; then " \
										"setenv uboot_overlay /lib/firmware/univ-bbb-xxx-00A0.dtbo; " \
									"else " \
										"if test -n ${disable_uboot_overlay_audio}; then " \
											"setenv uboot_overlay /lib/firmware/univ-bbb-xVx-00A0.dtbo; " \
										"else " \
											"setenv uboot_overlay /lib/firmware/univ-bbb-xVA-00A0.dtbo; " \
										"fi;" \
									"fi;" \
								"else " \
									"if test -n ${disable_uboot_overlay_video}; then " \
										"setenv uboot_overlay /lib/firmware/univ-bbb-Exx-00A0.dtbo; " \
									"else " \
										"if test -n ${disable_uboot_overlay_audio}; then " \
											"setenv uboot_overlay /lib/firmware/univ-bbb-EVx-00A0.dtbo; " \
										"else " \
											"setenv uboot_overlay /lib/firmware/univ-bbb-EVA-00A0.dtbo; " \
										"fi;" \
									"fi;" \
								"fi;" \
								"run capeloadoverlay;" \
							"fi;" \
							"if test -n ${uboot_cape_universal_bbg}; then " \
								"if test -n ${disable_uboot_overlay_emmc}; then " \
									"setenv uboot_overlay /lib/firmware/univ-bbb-xxx-00A0.dtbo; " \
								"else " \
									"setenv uboot_overlay /lib/firmware/univ-bbb-Exx-00A0.dtbo; " \
								"fi;" \
								"run capeloadoverlay;" \
							"fi;" \
							"if test -n ${uboot_cape_universal_bbgw}; then " \
								"if test -n ${disable_uboot_overlay_emmc}; then " \
									"if test -n ${disable_uboot_overlay_wireless}; then " \
										"setenv uboot_overlay /lib/firmware/univ-bbgw-xx-00A0.dtbo; " \
									"else " \
										"setenv uboot_overlay /lib/firmware/univ-bbgw-xW-00A0.dtbo; " \
									"fi;" \
								"else " \
									"if test -n ${disable_uboot_overlay_wireless}; then " \
										"setenv uboot_overlay /lib/firmware/univ-bbgw-Ex-00A0.dtbo; " \
									"else " \
										"setenv uboot_overlay /lib/firmware/univ-bbgw-EW-00A0.dtbo; " \
									"fi;" \
								"fi;" \
								"run capeloadoverlay;" \
							"fi;" \
						"fi;" \
					"else " \
						"echo uboot_overlays: add [enable_uboot_cape_universal=1] to /boot/uEnv.txt to enable...;" \
					"fi;" \
				"fi;" \
			"else " \
				"echo uboot_overlays: add [enable_uboot_overlays=1] to /boot/uEnv.txt to enable...;" \
			"fi;" \
			"setenv rdfile initrd.img-${uname_r}; " \
			"if test -e ${devtype} ${bootpart} ${bootdir}/${rdfile}; then " \
				"echo loading ${bootdir}/${rdfile} ...; "\
				"run loadrd;" \
				"if test -n ${netinstall_enable}; then " \
					"run args_netinstall; run message;" \
					"echo debug: [${bootargs}] ... ;" \
					"echo debug: [bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}] ... ;" \
					"bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}; " \
				"fi;" \
				"if test -n ${uenv_root}; then " \
					"run args_uenv_root;" \
					"echo debug: [${bootargs}] ... ;" \
					"echo debug: [bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}] ... ;" \
					"bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}; " \
				"fi;" \
				"if test -n ${uuid}; then " \
					"run args_mmc_uuid;" \
					"echo debug: [${bootargs}] ... ;" \
					"echo debug: [bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}] ... ;" \
					"bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}; " \
				"fi;" \
				"run args_mmc_old;" \
				"echo debug: [${bootargs}] ... ;" \
				"echo debug: [bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}] ... ;" \
				"bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}; " \
			"else " \
				"if test -n ${uenv_root}; then " \
					"run args_uenv_root;" \
					"echo debug: [${bootargs}] ... ;" \
					"echo debug: [bootz ${loadaddr} - ${fdtaddr}] ... ;" \
					"bootz ${loadaddr} - ${fdtaddr}; " \
				"fi;" \
				"run args_mmc_old;" \
				"echo debug: [${bootargs}] ... ;" \
				"echo debug: [bootz ${loadaddr} - ${fdtaddr}] ... ;" \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"fi;" \
		"fi;\0" \

#endif//__CONFIG_NPI_COMMON_H__
