nrf_sdk_src = [
  'ble.c',
  'sdk/modules/nrfx/mdk/gcc_startup_nrf52.S',
  'sdk/modules/nrfx/mdk/system_nrf52.c',
  'sdk/components/libraries/util/app_error.c',
  'sdk/components/libraries/util/app_util_platform.c',
  'sdk/components/ble/common/ble_advdata.c',
  'sdk/components/ble/nrf_ble_gatt/nrf_ble_gatt.c',
  'sdk/components/softdevice/common/nrf_sdh.c',
  'sdk/components/softdevice/common/nrf_sdh_ble.c',
  'sdk/components/libraries/experimental_section_vars/nrf_section_iter.c',
]

nrf_sdk_inc = include_directories(
  '.',
  'sdk/components/',
  'sdk/components/libraries/util/',
  'sdk/components/libraries/log/',
  'sdk/components/libraries/log/src/',
  'sdk/components/libraries/strerror/',
  'sdk/components/libraries/experimental_section_vars/',
  'sdk/components/softdevice/common/',
  'sdk/components/softdevice/s132/headers/',
  'sdk/components/softdevice/s132/headers/nrf52/',
  'sdk/components/toolchain/cmsis/include/',
  'sdk/integration/nrfx/',
  'sdk/modules/nrfx/',
  'sdk/modules/nrfx/mdk/',
  'sdk/modules/nrfx/hal/',
  'sdk/config/nrf52832/config/',
)

add_global_arguments(
  '-DUSE_APP_CONFIG',
  '-DNRF_SD_BLE_API_VERSION=7',
  '-DSOFTDEVICE_PRESENT',
  '-DS132',
  language : 'c'
)

add_global_link_arguments(
  '-L' + meson.current_source_dir(),
  '-L' + meson.current_source_dir() + '/sdk/modules/nrfx/mdk/',
  language : 'c'
)

nrf_sdk = static_library('nrf-sdk', nrf_sdk_src, include_directories : nrf_sdk_inc)
