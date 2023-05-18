#pragma once

#include <stdbool.h>

#include <softdevice/s132/headers/ble.h>
#include <nrf_sdh.h>
#include <nrf_sdh_ble.h>
#include <ble_srv_common.h>

/* A tag identifying the SoftDevice BLE configuration. */
#define BLE_C_SD_CONFIG_TAG	1

/* Most system components use priorities 1 and 2 */
#define BLE_C_OBSERVERS_PRIORITY	3

/*
 * The lower the values are the lower the ping and faster the transmission speed
 * It is possible to override those settings using setters below.
*/
#define BLE_C_DEFAULT_MIN_CONNECTION_INTERVAL	MSEC_TO_UNITS(100, UNIT_1_25_MS)
#define BLE_C_DEFAULT_MAX_CONNECTION_INTERVAL	MSEC_TO_UNITS(500, UNIT_1_25_MS)
#define BLE_C_DEFAULT_SUPERVISION_TIMEOUT	MSEC_TO_UNITS(4000, UNIT_1_25_MS)
#define BLE_C_DEFAULT_SLAVE_LATENCY		0
#define BLE_C_DEFAULT_NAME			"Generic NRF52 device"

ret_code_t ble_c_set_min_connection_interval(unsigned val);
ret_code_t ble_c_set_max_connection_interval(unsigned val);
ret_code_t ble_c_set_supervision_timeout(unsigned val);
ret_code_t ble_c_set_slave_latency(unsigned val);

uint16_t ble_c_create_service(ble_uuid128_t base_uuid, uint16_t service_uuid);

void ble_c_init_with_name(const char *name);
void ble_c_init(void);
