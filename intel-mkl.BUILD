package(default_visibility = ["//visibility:public"])

cc_library(
    name = "intel-mkl",
    hdrs = glob(["include/*.h"]),
    linkopts = [
        "-Wl,--start-group /opt/intel/oneapi/mkl/latest/lib/intel64/libmkl_intel_lp64.a /opt/intel/oneapi/mkl/latest/lib/intel64/libmkl_sequential.a /opt/intel/oneapi/mkl/latest/lib/intel64/libmkl_core.a -Wl,--end-group",
        "-pthread",
        "-ldl",
    ],
    strip_include_prefix = "include",
)
