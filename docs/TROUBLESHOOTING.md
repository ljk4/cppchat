# 问题排查 (Troubleshooting)

本文档记录了 CppChat 项目在开发、部署和使用过程中可能遇到的问题及解决方案。

## 1. 编译错误
### 1.1 找不到 Boost 库
**问题描述**：CMake 报错 `Could NOT find Boost (missing: system thread)`。
**解决方案**：
- 确保已安装 Boost 库。在 Ubuntu 上运行 `sudo apt-get install libboost-all-dev`。
- 如果使用 Docker，请确保基础镜像中包含了 Boost 库。

### 1.2 找不到 SQLite3 库
**问题描述**：CMake 报错 `Could NOT find SQLite3`。
**解决方案**：
- 确保已安装 SQLite3 开发库。在 Ubuntu 上运行 `sudo apt-get install libsqlite3-dev`。

## 2. 运行时错误
### 2.1 端口被占用
**问题描述**：服务端启动失败，报错 `bind: Address already in use`。
**解决方案**：
- 检查是否有其他程序占用了 `9090` 或 `80` 端口。运行 `lsof -i :9090` 或 `netstat -tuln | grep 9090`。
- 停止占用端口的程序，或在 `docker-compose.yml` 中修改映射端口。

### 2.2 数据库连接失败
**问题描述**：服务端启动时报错 `Failed to open database`。
**解决方案**：
- 检查服务端是否有权限在当前目录下创建或写入 `chat.db` 文件。
- 如果使用 Docker，请确保挂载了正确的数据卷（Volume）。

## 3. 客户端连接问题
### 3.1 Web 页面无法访问
**问题描述**：浏览器访问 `http://<IP>` 提示无法连接。
**解决方案**：
- 检查云服务器的安全组/防火墙是否放行了 `80` 端口。
- 检查 Nginx 容器是否正常运行：`docker compose ps`。

### 3.2 WebSocket 连接失败 (注册/登录无反应)
**问题描述**：在 Web 页面点击注册或登录按钮没有反应，浏览器控制台报错 `WebSocket connection to 'ws://...' failed`。
**解决方案**：
- 检查云服务器的安全组/防火墙是否放行了 WebSocket 使用的端口（当前配置为 `9090`）。
- 检查服务端容器是否正常运行：`docker compose logs server`。
- 确保 `web/index.html` 中的 WebSocket 连接地址与服务端实际监听的端口一致。

### 3.3 消息发送失败
**问题描述**：客户端发送消息后，服务端未收到或报错。
**解决方案**：
- 检查网络连接是否正常。
- 检查消息格式是否符合 JSON 协议要求。
- 查看服务端的日志输出，排查具体错误原因。

## 4. Docker 相关问题
### 4.1 镜像构建缓慢
**问题描述**：每次运行 `docker build` 都需要很长时间下载依赖。
**解决方案**：
- 使用基础镜像（Base Image）预先安装好所有依赖，并在应用镜像中基于基础镜像进行构建。
- 利用 Docker 的构建缓存（Build Cache），将不常变动的指令（如安装依赖）放在 Dockerfile 的前面。

### 4.2 容器无法启动
**问题描述**：运行 `docker-compose up` 后，容器立即退出。
**解决方案**：
- 查看容器日志：`docker-compose logs server` 或 `docker-compose logs client`。
- 检查启动命令是否正确，是否有必要的环境变量或配置文件缺失。
