#include <pybind11/embed.h>
#include "Server.h"
#include <iostream>

namespace py = pybind11;

int main() {
    // 初始化 Python 解释器（保证在调用任何 Python 代码前初始化）
    py::scoped_interpreter guard{};

    // 创建服务器：监听 8080 端口，使用 4 个线程的线程池处理任务
    Server server(8080, 10);
    std::cout << "Server started. Waiting for connections..." << std::endl;
    server.run();

    return 0;
}
