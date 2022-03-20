project('ufmt', 'cpp',
    license: 'MIT',
    version: '0.1.1',
    default_options: [
        'cpp_std=c++17',
        'warning_level=3'])

headers = [
    'include/ufmt/buffered_file.hpp',
    'include/ufmt/print.hpp',
    'include/ufmt/text.hpp',
    'include/ufmt/text_file.hpp'
]

incdirs = include_directories('./include')

ufmt = declare_dependency(
    version: meson.project_version(),
    include_directories: incdirs,
    sources: headers
)

subdir('benchmark')
subdir('test')

install_headers(headers, subdir: 'ufmt')

pkg = import('pkgconfig')
pkg.generate(
    name: 'ufmt',
    filebase: 'ufmt',
    subdirs: ['ufmt'],
    version: meson.project_version(),
    description: 'C++17 header-only library to format text'
)
