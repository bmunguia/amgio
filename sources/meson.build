libmeshb7_flags = ['-fPIC']

if (build_machine.system() == 'darwin')
  libmeshb7_flags += ['-DDARWIN', '-DMAC64']
endif

if (build_machine.system() == 'linux')
  libmeshb7_flags += ['-DLINUX', '-DLINUX64']
endif

libmeshb7_include = include_directories(['./'])

libmeshb7 = static_library('meshb7',
                           'libmeshb7.c',
                           install : true,
                           c_args: [default_warning_flags, libmeshb7_flags],
                           include_directories: libmeshb7_include)

libmeshb7_dep = declare_dependency(link_with: libmeshb7,
                                   include_directories: libmeshb7_include)