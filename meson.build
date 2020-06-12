project('iksemel','c',
  version : '0.1',
  default_options : ['warning_level=3'])

if get_option('python')
  python = find_program('python3')
  pythonver = run_command(python, '-c', 'import sys; print("%s.%s" % sys.version_info[:2], end="")').stdout()

  # or via language_version()
  python = import('python')
  pythonver = python.find_installation('python3').language_version()

  python_dep = dependency('python-@0@'.format(pythonver), version: '>=3', required: true)
  subdir('python')
endif

include_dir = include_directories('include')

subdir('include')
subdir('src')
#subdir('tests')


#test('basic', exe)
