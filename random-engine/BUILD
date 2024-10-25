cc_library(
    name = "app",
    hdrs = ["main/App.hpp"],
    srcs = ["main/App.cpp"],
    deps = [
        "@bazel_tools//tools/cpp/runfiles",
        "@glm//:glm",
        "@glew//:glew_static",
        "//resources:resources",
        "//external/glfw",
        "//lib:lib",
    ]
)

cc_binary(
    name = "main",
    srcs = ["main/main.cpp"],
    deps = [
        "//:app"
    ]
)