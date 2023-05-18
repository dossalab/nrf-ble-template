#include <nrf_log.h>
#include <ble/nrf_ble_gatt/nrf_ble_gatt.h>
#include <ble/common/ble_advdata.h>
#include <app_error.h>

#include "ble.h"
#include "ble_gap.h"
#include "app_util.h"

/* A helper for declaring an advertised packet */
#define BLE_C_ADVERTISING_PACKET_DEF(name) \
	static ble_gap_adv_data_t name = { \
		.adv_data = { \
			.p_data = (uint8_t[BLE_GAP_ADV_SET_DATA_SIZE_MAX]){}, \
			.len = BLE_GAP_ADV_SET_DATA_SIZE_MAX, \
		} \
	}

static uint16_t connection_handle = BLE_CONN_HANDLE_INVALID;
static uint8_t advertiser_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;

/* Connection parameters */
static unsigned min_connection_interval = BLE_C_DEFAULT_MIN_CONNECTION_INTERVAL;
static unsigned max_connection_interval = BLE_C_DEFAULT_MAX_CONNECTION_INTERVAL;
static unsigned connection_supervision_timeout = BLE_C_DEFAULT_SUPERVISION_TIMEOUT;
static unsigned slave_latency = BLE_C_DEFAULT_SLAVE_LATENCY;

BLE_C_ADVERTISING_PACKET_DEF(advertised_packet);
NRF_BLE_GATT_DEF(gatt_handle);

static ret_code_t update_connection_params(void)
{
	ble_gap_conn_params_t params = {
		.min_conn_interval = min_connection_interval,
		.max_conn_interval = max_connection_interval,
		.conn_sup_timeout  = connection_supervision_timeout,
		.slave_latency = slave_latency
	};

	return sd_ble_gap_ppcp_set(&params);
}

ret_code_t ble_c_set_min_connection_interval(unsigned val)
{
	min_connection_interval = val;
	return update_connection_params();
}

ret_code_t ble_c_set_max_connection_interval(unsigned val)
{
	max_connection_interval = val;

	if (max_connection_interval < min_connection_interval) {
		min_connection_interval = max_connection_interval;
	}

	return update_connection_params();
}

ret_code_t ble_c_set_supervision_timeout(unsigned val)
{
	connection_supervision_timeout = val;
	return update_connection_params();
}

ret_code_t ble_c_set_slave_latency(unsigned val)
{
	slave_latency = val;
	return update_connection_params();
}

static ret_code_t ble_c_set_name(const char *name)
{
	ble_gap_conn_sec_mode_t sec_mode;

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
	return sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)name, strlen(name));
}

static ret_code_t switch_advertiser(bool on)
{
	if (on) {
		return sd_ble_gap_adv_start(advertiser_handle, BLE_C_SD_CONFIG_TAG);
	} else {
		return sd_ble_gap_adv_stop(advertiser_handle);
	}
}

static ret_code_t advertising_init(void)
{
	ret_code_t err;
	ble_gap_adv_params_t adv_params;

	ble_advdata_t advdata = {
		.name_type = BLE_ADVDATA_FULL_NAME,
		.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE,
		.include_appearance = true,
	};

	err = ble_advdata_encode(&advdata, advertised_packet.adv_data.p_data, \
			&advertised_packet.adv_data.len);
	if (err != NRF_SUCCESS) {
		return err;
	}

	memset(&adv_params, 0, sizeof(adv_params));

	adv_params.primary_phy     = BLE_GAP_PHY_1MBPS;
	adv_params.duration        = BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED;
	adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
	adv_params.p_peer_addr     = NULL;
	adv_params.filter_policy   = BLE_GAP_ADV_FP_ANY;
	adv_params.interval        = MSEC_TO_UNITS(500, UNIT_0_625_MS);

	return sd_ble_gap_adv_set_configure(&advertiser_handle, &advertised_packet, &adv_params);
}

static void ble_event_handler(ble_evt_t const *event, void *user)
{
	ret_code_t err_code;

	switch (event->header.evt_id) {
	case BLE_GAP_EVT_CONNECTED:
		connection_handle = event->evt.gap_evt.conn_handle;
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		connection_handle = BLE_CONN_HANDLE_INVALID;
		switch_advertiser(true);
		break;

	case BLE_GATTS_EVT_TIMEOUT:
	case BLE_GATTC_EVT_TIMEOUT:
		err_code = sd_ble_gap_disconnect(connection_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GATTS_EVT_SYS_ATTR_MISSING:
		err_code = sd_ble_gatts_sys_attr_set(connection_handle, NULL, 0, 0);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
		err_code = sd_ble_gap_sec_params_reply(connection_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
		APP_ERROR_CHECK(err_code);
		break;

	default:
		break;
	}
}

NRF_SDH_BLE_OBSERVER(ble_observer, BLE_C_OBSERVERS_PRIORITY, ble_event_handler, NULL);

unsigned ble_c_create_service(unsigned base_uuid, uint16_t service_uuid)
{
	uint16_t handle;
	ret_code_t err;

	ble_uuid_t ble_service_uuid = {
		.uuid = service_uuid,
		.type = base_uuid,
	};

	err = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_service_uuid, &handle);
	APP_ERROR_CHECK(err);

	return handle;
}

void ble_c_init_with_name(const char *name)
{
	uint32_t ram_start = 0;
	ret_code_t err_code;

	err_code = nrf_sdh_enable_request();
	APP_ERROR_CHECK(err_code);

	err_code = nrf_sdh_ble_default_cfg_set(BLE_C_SD_CONFIG_TAG, &ram_start);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_sdh_ble_enable(&ram_start);
	APP_ERROR_CHECK(err_code);

	err_code = update_connection_params();
	APP_ERROR_CHECK(err_code);

	err_code = nrf_ble_gatt_init(&gatt_handle, NULL);
	APP_ERROR_CHECK(err_code);

	ble_c_set_name(name);
	APP_ERROR_CHECK(err_code);

	err_code = advertising_init();
	APP_ERROR_CHECK(err_code);

	err_code = switch_advertiser(true);
	APP_ERROR_CHECK(err_code);
}

void ble_c_init(void)
{
	ble_c_init_with_name(BLE_C_DEFAULT_NAME);
}
