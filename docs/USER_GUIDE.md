# 用户指南 (User Guide)

本文档面向项目使用者，指导你如何使用 CppChat 聊天室。

## 1. 准备工作
你需要一台安装了 Docker 和 Docker Compose 的机器。
- [安装 Docker](https://docs.docker.com/get-docker/)
- [安装 Docker Compose](https://docs.docker.com/compose/install/)

## 2. 启动服务
在项目根目录下运行以下命令：
```bash
docker compose up -d
```
这会在后台启动服务端容器（监听 `9090` 端口）和 Web 前端容器（监听 `80` 端口）。

## 3. 使用聊天室 (Web 浏览器)
服务启动后，你可以直接在浏览器中使用聊天室。

1. 打开浏览器，访问 `http://localhost`（如果你在本地运行）或 `http://<你的服务器IP>`。
2. 你会看到 CppChat 的登录界面。

### 3.1 注册 (Register)
在输入框中输入你想要的用户名和密码，然后点击 **Register** 按钮。如果用户名未被占用，系统会提示注册成功。

### 3.2 登录 (Login)
输入你的用户名和密码，点击 **Login** 按钮。登录成功后，界面会切换到聊天窗口。

### 3.3 聊天 (Chat)
在底部的输入框中输入消息，按回车键或点击 **Send** 按钮发送。所有在线用户都会实时收到你的消息。

## 4. 使用聊天室 (C++ 终端客户端 - 可选)
如果你想在终端中测试，也可以启动 C++ 客户端：
```bash
docker compose run --rm client
```
按照终端提示进行注册、登录和聊天。输入 `/quit` 退出。

## 5. 停止服务
如果你想停止服务，可以在项目根目录下运行：
```bash
docker compose down
```
这会停止并删除相关的容器。
DOCKERHUB_USERNAME: 你的 Docker Hub 用户名。
DOCKERHUB_TOKEN: 你的 Docker Hub 访问令牌 (Access Token)。
SERVER_HOST: 你的云服务器的 IP 地址或域名。
SERVER_USERNAME: 登录云服务器的用户名 (例如 ubuntu 或 root)。
SERVER_SSH_KEY: 登录云服务器的 SSH 私钥 (Private Key)。