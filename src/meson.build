# meson modules
gnome = import('gnome')

# project dependencies
gtkdep = dependency('gtkmm-3.0')

src = [
  'main.cc',

  # widgets
  'widgets/processview.cc',
  'widgets/processview.h',
  'widgets/resourceview.cc',
  'widgets/resourceview.h',

  # common
  'common/core.cc',
  'common/core.h',
  'common/cpuinfo.cc',
  'common/cpuinfo.h',
  'common/diskinfo.cc',
  'common/diskinfo.h',
  'common/processinfo.h',
  'common/sysmemoryinfo.h',

  # windows
  'windows/mainwindow.cc',
  'windows/mainwindow.h'
]

resources = gnome.compile_resources(
  'resources',
  'resources.xml',
  source_dir : './',
  c_name : 'as'
)

executable(
  'process-monitor',
  src,
  resources,
  dependencies: gtkdep
)
