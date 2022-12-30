# NRF5 SDK template

This repository is intended to simplify development for NRF52 (most notably NRF52832). It wraps the build system as a meson componenent, and adds a simple wrapper around SoftDevice initialization, so the developer can easily create new applications without copying existing code from repo to repo.

## Usage:

- Add this repo as a git subproject
- Create a meson.build file (for example with `meson init`). Add some basic flags common for ARM compilation (most likely you will need at least `-mthumb -mcpu=cortex-m4` for both linker and compiler, also you need to specify a linker sctipt with `-T`). Include this repo with `subdir`:

```
  project('ble_blinky', 'c',
  version : '0.1',
  default_options : [ 'optimization=s' ])

arch_cc_linker_args = [
  '-mthumb',
  '-mcpu=cortex-m4',
]

arch_linker_only_args = [
  '-specs=nano.specs',
  '-L../nrf-sdk/sdk/modules/nrfx/mdk/',
  '-T../nrf-sdk/sdk/config/nrf52832/armgcc/generic_gcc_nrf52.ld',
]

add_global_arguments(
  arch_cc_linker_args,
  language : 'c'
)

subdir('ble-template')

executable('ble_blinky',
           'blinky.c',
           link_with : nrf_sdk,
           include_directories : nrf_sdk_inc,
           link_args : [ arch_cc_linker_args, arch_linker_only_args ])
```

- Create a sample application:

```
#include <nrf_gpio.h>

#include "ble.h"

#define LED_PIN		27

static inline void led_switch(bool on) {
	nrf_gpio_pin_write(LED_PIN, on);
}

static inline void led_init(void) {
	nrf_gpio_pin_dir_set(LED_PIN, NRF_GPIO_PIN_DIR_OUTPUT);
	led_switch(false);
}

static void connection_events_handler(ble_evt_t const *event, void *user)
{
	/* Other events are already handled by the library */
	switch (event->header.evt_id) {
	case BLE_GAP_EVT_CONNECTED:
		led_switch(true);
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		led_switch(false);
		break;
	}
}

NRF_SDH_BLE_OBSERVER(connection_observer, BLE_C_OBSERVERS_PRIORITY, connection_events_handler, NULL);

/* You can add custom error logic here, see SDK for more examples */
void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t _info)
{
	__disable_irq();
	led_switch(true);

	while(1);
}

int main(void)
{
	led_init();
	ble_c_init();

	for (;;) {
		sd_app_evt_wait();
	}
}
`
