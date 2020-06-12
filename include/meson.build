
cc = meson.get_compiler('c')
cdata = configuration_data()
check_headers = [
  ['HAVE_DLFCN_H', 'dlfcn.h'],
  ['HAVE_ERRNO_H', 'errno.h'],
  ['HAVE_INTTYPES_H', 'inttypes.h'],
  ['HAVE_MEMORY_H', 'memory.h'],
  ['HAVE_STDINT_H', 'stdint.h'],
  ['HAVE_STDLIB_H', 'stdlib.h'],
  ['HAVE_STRINGS_H', 'strings.h'],
  ['HAVE_STRING_H', 'string.h'],
  ['HAVE_SYS_STAT_H', 'sys/stat.h'],
  ['HAVE_SYS_TYPES_H', 'sys/types.h'],
  ['HAVE_UNISTD_H', 'unistd.h'],
]

foreach h : check_headers
  if cc.has_header(h.get(1))
    cdata.set(h.get(0), 1)
  endif
endforeach

check_functions = [
  ['HAVE_GETADDRINFO', 'getaddrinfo', '#include<netdb.h>'],
  ['HAVE_GETOPT_LONG', 'getopt_long', '#include<getopt.h>'],
# check token ['HAVE_GNUTLS']
# check token ['HAVE_LIBWSOCK32']
# check token ['HAVE_OPENSSL']
# check token ['HAVE_STRUCT_STAT_ST_BLKSIZE']
]

foreach f : check_functions
  if cc.has_function(f.get(1), prefix : f.get(2))
    cdata.set(f.get(0), 1)
  endif
endforeach


configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : cdata)

install_headers([
                 'common.h',
                 'finetune.h',
                 'iksemel.h',
                ]
)
