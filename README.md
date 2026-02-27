# CppChat

CppChat 是一个基于 C++17 和 Boost.Asio 实现的实时聊天室项目。支持用户注册、登录和群聊功能。

## 项目特点
- **工程级标准**：使用 CMake 构建，模块化设计。
- **Docker 化开发**：提供基础镜像和多阶段构建，加速开发和部署。
- **单元测试**：集成 Google Test 和 Google Mock。
- **CI/CD**：集成 GitHub Actions，实现自动化构建、测试，并自动部署到云服务器。
- **实时通信**：基于 Boost.Asio 的异步 TCP 网络编程。
- **数据持久化**：使用 SQLite3 存储用户信息。

## 快速开始

### 面向小白用户
1. 确保你的机器上安装了 Docker 和 Docker Compose。
2. 克隆本项目：`git clone <repo_url>`
3. 进入项目目录：`cd cppchat`
4. 启动服务：`docker compose up -d`
5. 打开浏览器，访问 `http://localhost` (或你的服务器 IP 地址)。
6. 在网页上进行注册、登录和聊天。

### 面向开发者
1. 确保安装了 Docker。
2. 构建基础镜像：`cd docker && docker build -t cppchat-base -f Dockerfile.base .`
3. 在容器中开发或使用 VS Code Remote Containers 连接到容器。
4. 编译项目：
   ```bash
   mkdir build && cd build
   cmake ..
   make -j4
   ```
5. 运行测试：`ctest --output-on-failure`

## 项目结构
- `src/`：源代码目录
  - `server/`：服务端代码 (C++, Boost.Beast WebSocket)
  - `client/`：客户端代码 (C++, Boost.Beast WebSocket)
  - `common/`：公共组件（消息协议等）
- `web/`：Web 前端代码 (HTML, JS)
- `tests/`：单元测试代码
- `docker/`：Docker 相关配置文件
- `docs/`：项目文档
- `.github/workflows/`：CI/CD 配置文件

## 详细文档
- [用户指南 (USER_GUIDE.md)](docs/USER_GUIDE.md)
- [技术文档 (TECH_DOC.md)](docs/TECH_DOC.md)
- [问题排查 (TROUBLESHOOTING.md)](docs/TROUBLESHOOTING.md)
