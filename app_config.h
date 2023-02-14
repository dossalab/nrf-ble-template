/*
 * This file is prepended to the default SDK's sdk_config.h.
 *
 * That we can override only the values we are interested in, not
 * having to check-in like ~13k lines from the full config.
 */

#pragma once

#define NRF_SDH_ENABLED			1
#define NRF_SDH_SOC_ENABLED		1
#define NRF_SDH_BLE_ENABLED		1
#define NRF_BLE_GATT_ENABLED		1
#define NRF_SDH_BLE_VS_UUID_COUNT	5
#define NRF_SDH_BLE_PERIPHERAL_LINK_COUNT 1

/*
 * Source selection for low frequency clock (required for BLE)
 *
 * Possible options are:
 * - NRF_CLOCK_LF_SRC_RC    - internal RC clock, requires periodic calibration hence
 *                            requires more energy; calibration params are set by CTIV
 *
 * - NRF_CLOCK_LF_SRC_XTAL  - extennal LF crystal, most powersaving option, requires
 *                            separate crystal to be installed on the board
 *
 * - NRF_CLOCK_LF_SRC_SYNTH - internal clock derived from HF crystal; requires PLL to be
 *                            running at all time, drawing much more energy (in mA range)
 */

#define NRF_SDH_CLOCK_LF_SRC		NRF_CLOCK_LF_SRC_RC
#define NRF_SDH_CLOCK_LF_ACCURACY	NRF_CLOCK_LF_ACCURACY_500_PPM

/* See CTIV register in "Power and clock management" in the datasheet */
#define NRF_SDH_CLOCK_LF_RC_CTIV	16
#define NRF_SDH_CLOCK_LF_RC_TEMP_CTIV	2

/*
 * Enabled built-in services
 */
#define BLE_BAS_ENABLED 1
#define BLE_DIS_ENABLED 1
