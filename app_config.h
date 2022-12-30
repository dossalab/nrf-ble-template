/*
 * This file is prepended to the default SDK's sdk_config.h.
 *
 * That we can override only the values we are interested in, not
 * having to check-in like ~13k lines from the full config.
 */

#pragma once

#define NRF_SDH_ENABLED		1
#define NRF_SDH_BLE_ENABLED	1
#define NRF_BLE_GATT_ENABLED	1
#define NRF_SDH_BLE_PERIPHERAL_LINK_COUNT 1

/*
 * Source selection for low frequency clock (required for BLE)

 * Possible options are:
 * - NRF_CLOCK_LF_SRC_RC
 * - NRF_CLOCK_LF_SRC_XTAL - if your board has LF crystal installed
 * - NRF_CLOCK_LF_SRC_SYNTH - safe fallback, however it requires more energy
 */

#define NRF_SDH_CLOCK_LF_SRC	NRF_CLOCK_LF_SRC_SYNTH
