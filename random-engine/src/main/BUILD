cc_binary(
    name = "main",
    srcs = glob(["*.cpp"]),
    deps = [
        "@glm//:glm",
        "@glew//:glew_static",
        "//external/glfw",
    ],
    visibility = ["//src/test/main:__pkg__"],
)