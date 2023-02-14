cc = meson.get_compiler('c')

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

nrf_sdk_dep = declare_dependency(
  sources: [
    'ble.c',
    'sdk/modules/nrfx/mdk/gcc_startup_nrf52.S',
    'sdk/modules/nrfx/mdk/system_nrf52.c',
    'sdk/components/libraries/atomic/nrf_atomic.c',
    'sdk/components/libraries/util/app_error.c',
    'sdk/components/libraries/util/app_util_platform.c',
    'sdk/components/ble/common/ble_advdata.c',
    'sdk/components/ble/common/ble_srv_common.c',
    'sdk/components/ble/common/ble_conn_state.c',
    'sdk/components/ble/nrf_ble_gatt/nrf_ble_gatt.c',
    'sdk/components/softdevice/common/nrf_sdh.c',
    'sdk/components/softdevice/common/nrf_sdh_ble.c',
    'sdk/components/libraries/experimental_section_vars/nrf_section_iter.c',
    'sdk/components/libraries/atomic_flags/nrf_atflags.c',
    'sdk/components/ble/ble_services/ble_bas/ble_bas.c',
    'sdk/components/ble/ble_services/ble_dis/ble_dis.c',
  ],
  include_directories : include_directories(
    '.',
    'sdk/components/',
    'sdk/components/libraries/atomic_flags/',
    'sdk/components/libraries/atomic/',
    'sdk/components/libraries/util/',
    'sdk/components/libraries/log/',
    'sdk/components/libraries/log/src/',
    'sdk/components/libraries/strerror/',
    'sdk/components/libraries/experimental_section_vars/',
    'sdk/components/softdevice/common/',
    'sdk/components/softdevice/s132/headers/',
    'sdk/components/softdevice/s132/headers/nrf52/',
    'sdk/components/toolchain/cmsis/include/',
    'sdk/components/ble/common/',
    'sdk/components/ble/ble_services/',
    'sdk/integration/nrfx/',
    'sdk/modules/nrfx/',
    'sdk/modules/nrfx/mdk/',
    'sdk/modules/nrfx/hal/',
    'sdk/config/nrf52832/config/',
    'sdk/modules/nrfx/drivers/include/',
  )
)

nrfx_dep = declare_dependency(
  sources: [
    'sdk/modules/nrfx/drivers/src/nrfx_power.c',
    'sdk/modules/nrfx/drivers/src/nrfx_nfct.c',
    'sdk/modules/nrfx/drivers/src/nrfx_clock.c',
    'sdk/modules/nrfx/drivers/src/nrfx_timer.c',
    'sdk/integration/nrfx/legacy/nrf_drv_clock.c'
  ],
  include_directories : include_directories(
    'sdk/modules/nrfx/drivers/include/',
    'sdk/integration/nrfx/legacy'
  ),
  compile_args: [
    '-DNFC_PLATFORM_ENABLED',
    '-DNRF_CLOCK_ENABLED',
    '-DTIMER_ENABLED',
    '-DNRFX_TIMER_ENABLED',
    '-DNRFX_CLOCK_ENABLED',
    '-DNRFX_NFCT_ENABLED'
  ],
  dependencies: nrf_sdk_dep
)

nrf_nfc_dep = declare_dependency(
  sources: [
    'sdk/components/nfc/ndef/uri/nfc_uri_msg.c',
    'sdk/components/nfc/ndef/uri/nfc_uri_rec.c',
    'sdk/components/nfc/platform/nfc_platform.c',
    'sdk/components/nfc/ndef/generic/message/nfc_ndef_msg.c',
    'sdk/components/nfc/ndef/generic/record/nfc_ndef_record.c'
  ],
  dependencies: [
    nrf_sdk_dep, nrfx_dep,
    cc.find_library('nfc_t4t_lib_gcc_no_fpu',
      dirs : meson.current_source_dir() + '/sdk/components/nfc/t4t_lib/')
  ],
  include_directories : include_directories(
    'sdk/components/nfc/ndef/generic/message/',
    'sdk/components/nfc/ndef/generic/record/',
  ),
  compile_args: [
    '-DTIMER4_ENABLED=1',
    '-DNRFX_TIMER4_ENABLED=1',
    '-DNFC_NDEF_URI_MSG_ENABLED=1',
    '-DNFC_NDEF_MSG_ENABLED=1',
    '-DNFC_NDEF_RECORD_ENABLED=1',
    '-DNFC_NDEF_URI_REC_ENABLED=1' ]
)
