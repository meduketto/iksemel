py_sources=[
            'document.c',
            'exceptions.c',
            'jid.c',
            'node.c',
            'pyiksemel.c',
            'reference.c',
            'stream.c',
           ]
py_headers = [
              'document.h',
              'exceptions.h',
              'jid.h',
              'node.h',
              'reference.h',
              'stream.h',
             ]

pymod = import('python')
python = pymod.find_installation(get_option('python'))

if python.language_version().version_compare('< 3.0') and host_machine.system() == 'windows'
 error('Python 2 on Windows no longer supported since 3.35. Use Python 3 instead.')
endif

python_dep = python.dependency()

pyext_c_args = ['-DPY_SSIZE_T_CLEAN']


if host_machine.system() == 'windows'
 python_ext_dep = python_dep
else
 python_ext_dep = python_dep.partial_dependency(compile_args: true)
endif

pyiksemel = python.extension_module('pyiksemel',
                        py_sources,
                        dependencies : [python_ext_dep],
                        include_directories : [include_dir, '.'],
                        link_with : libiksemel,
                        install : true,
                        c_args: pyext_c_args)
