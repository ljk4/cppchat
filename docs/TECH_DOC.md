# 技术文档 (Technical Documentation)

本文档说明了 CppChat 项目的技术选型、原因及适用场景。

## 1. 编程语言
- **C++17**：使用现代 C++ 特性（如 `std::optional`, `std::variant`, `std::string_view` 等），提高代码可读性和安全性。

## 2. 构建系统
- **CMake**：跨平台构建工具，支持模块化管理，易于集成第三方库（如 Boost, GTest）。

## 3. 网络库
- **Boost.Asio & Boost.Beast**：业界标准的 C++ 异步网络库及 HTTP/WebSocket 库。
  - **原因**：高性能、跨平台、支持异步 I/O。使用 WebSocket 协议可以方便地与 Web 浏览器进行全双工通信。
  - **场景**：WebSocket 长连接，实时消息推送。

## 4. 数据序列化
- **nlohmann/json**：现代 C++ JSON 库。
  - **原因**：语法直观，易于使用，支持与 STL 容器的无缝转换。JSON 格式天然兼容 Web 前端的 JavaScript。
  - **场景**：客户端（Web/C++）与服务端之间的消息协议（注册、登录、聊天消息）。

## 5. 数据库
- **SQLite3**：轻量级嵌入式关系型数据库。
  - **原因**：无需单独部署数据库服务器，适合小型项目和快速原型开发。数据存储在本地文件中，易于备份和迁移。
  - **场景**：存储用户信息（用户名、密码）。

## 6. 测试框架
- **Google Test (GTest) & Google Mock (GMock)**：C++ 单元测试框架。
  - **原因**：功能强大，支持断言、测试夹具、Mock 对象，易于集成到 CMake 中。
  - **场景**：测试消息解析、数据库操作、业务逻辑。

## 7. 容器化与部署
- **Docker & Docker Compose**：
  - **原因**：保证开发、测试、生产环境的一致性。使用多阶段构建（Multi-stage build）减小最终镜像体积。提供基础镜像（Base Image）加速 CI/CD 构建过程。使用 Nginx 容器托管 Web 前端。
  - **场景**：本地开发环境搭建、云端部署。

## 8. CI/CD
- **GitHub Actions**：
  - **原因**：与 GitHub 仓库无缝集成，配置简单，支持自动化构建、测试和发布。
  - **场景**：
    - **CI (持续集成)**：每次 Push 或 Pull Request 时自动运行单元测试，确保代码质量。
    - **CD (持续部署)**：当代码合并到 `main` 分支时，自动构建 Docker 镜像并推送到 Docker Hub，然后通过 SSH 连接到云服务器，拉取最新镜像并重启服务。

## 9. 架构设计
- **B/S & C/S 混合架构**：支持 Web 浏览器客户端（B/S）和 C++ 终端客户端（C/S）。
- **异步事件驱动**：服务端使用 Boost.Asio 的 `io_context` 处理并发连接，避免多线程锁竞争带来的性能开销。
- **消息协议**：基于 JSON 的应用层协议，通过 WebSocket 传输。包含消息类型（Type）、发送者（Sender）、内容（Content）等字段。WebSocket 协议本身解决了 TCP 粘包问题，因此无需在应用层添加长度头部。
