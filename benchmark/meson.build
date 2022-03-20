fmtlib = declare_dependency(
    version: '8.1.1',
    include_directories: '../thirdparty/include',
    sources: [
        '../thirdparty/source/format.cc',
        '../thirdparty/source/os.cc'
    ]
)

sources = [
    'benchmark.cpp',
]

executable('ufmt-benchmark',
            sources, dependencies: [ufmt, fmtlib])
