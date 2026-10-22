# =========================================================================
#CMakeLists.txt：告诉编译器怎么把一堆 .cpp 文件拼装成一个可执行程序
# =========================================================================

# 规定当前工程必须使用 3.10 或更高版本的 CMake 软件（防止老版本报错）
cmake_minimum_required(VERSION 3.10)

# 定义项目名叫 study，使用的语言是 CXX（代表 C++）
project(study CXX)

# 【现代规范】强制编译器使用 C++17 标准（这样你才能用 auto、初始化列表等新特性）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 【找头文件】告诉编译器：你去 include 这个文件夹里找所有的 .h 头文件
include_directories(include)

# 【生成程序】把后面这三个源文件揉在一起，编译成一个叫 "run" 的可执行文件
add_executable(run
    src/main.cpp
    src/Task.cpp
    src/Manager.cpp
)

# 【代码质检】开启所有警告（-Wall）和额外警告（-Wextra）。
# 只要代码写得有一丁点不规范（比如变量定义了没用），它就会给你爆红字
target_compile_options(run PRIVATE -Wall -Wextra)