# copy from https://github.com/xlong88/sqlpp11-connector-mysql/blob/master/tests/CMakeLists.txt
macro (build_and_run arg)
    # Add headers to sources to enable file browsing in IDEs
    add_executable(StaticTree${arg} ${arg}.cpp ${lgds_headers})

    add_test(${arg} StaticTree${arg})
endmacro ()

build_and_run(static_tree_test)
build_and_run(linear_sampler_test)