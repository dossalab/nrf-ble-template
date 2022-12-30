add_global_arguments(
  '-mthumb',
  '-mcpu=cortex-m4',
  '-DNRF52832_XXAA',
  language : 'c'
)

add_global_link_arguments(
  '-mthumb',
  '-mcpu=cortex-m4',
  '-specs=nano.specs',
  '-L' + meson.current_source_dir(),
  '-Tnrf52832.ld',
  language : 'c'
)
