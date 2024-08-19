package(default_visibility = ["//visibility:public"])

cc_library(
    name = "intel-mkl-sequential",
    srcs = [
        "lib/intel64/libmkl_core.a",
        "lib/intel64/libmkl_intel_lp64.a",
        "lib/intel64/libmkl_sequential.a",
    ],
    hdrs = glob(["include/*.h"]),
    linkopts = ["-Wl,--start-group /opt/intel/oneapi/mkl/latest/lib/intel64/libmkl_intel_lp64.a /opt/intel/oneapi/mkl/latest/lib/intel64/libmkl_sequential.a /opt/intel/oneapi/mkl/latest/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm -ldl"],
    strip_include_prefix = "include",
)
