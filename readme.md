# 💻 Nexus

**Nexus** is a cross-platform LAN communication and control system built in **C++ with Qt**. It enables real-time interaction between devices on the same local network without internet.

---

## 🚀 Features

- 🗨️ **Real-time LAN Chat** – Peer-to-peer messaging using UDP
- 📋 **Clipboard Sync** – Share copied text across devices with global hotkeys
- 📁 **File Transfer** – Send files directly to any system on the network with delivery status
- 🗂️ **Shared Dropbox** – Host files/folders for public access without transferring them
- 👥 **Auto User Discovery** – Detect and show all active users on the network
- 🔒 **Private and Local** – No internet needed, full control over your data

---

## 🖥️ UI Overview

- **Left Panel:** Floating menu + user list with avatars
- **Right Panel:** Dynamic view (chat, clipboard, dropbox, etc.)
- **Chat View:** Header (avatar + name), message area, input with send/file buttons

---

## 🛠️ Technology Stack

| Component        | Tech Used         |
|------------------|------------------|
| Frontend         | Qt Widgets (C++) |
| Messaging        | UDP Sockets      |
| File Sharing     | TCP Sockets      |
| Clipboard Sync   | Qt Clipboard + Hotkeys |
| Database         | SQLite (Temporary) |
| Build Tool       | qmake            |
| Platform         | Windows, Linux   |

---

## 🧠 Architecture

- **LanDiscovery** – Handles broadcasting and listening for active users
- **ChatManager** – Sends/receives UDP messages
- **ClipboardManager** – Sends clipboard contents and receives shared data
- **FileTransferManager** – TCP-based file transfer system with real-time status
- **DatabaseManager** – SQLite-based history storage (chat, clipboard, file logs)

---

## 📦 Download

| Platform | Installer |
|----------|-----------|
| Windows  | [Download Nexus.exe](downloads/nexus-windows.exe) |
| Linux    | [Download .tar.gz](downloads/nexus-linux.tar.gz) |

---

## 💡 Use Cases

- 💼 Office file and message exchange
- 🧪 Classroom/lab interactions without internet
- 🧠 Clipboard and workflow sync between personal devices
- 🏡 Home automation control hub (future feature)

---

## ✨ Screenshots

![Screenshot 1](https://via.placeholder.com/600x300.png?text=Chat+Interface)
![Screenshot 2](https://via.placeholder.com/600x300.png?text=Clipboard+History)
![Screenshot 3](https://via.placeholder.com/600x300.png?text=File+Transfer+Panel)

---

## 🤝 Contributing

We welcome contributions! Fork the repo, create a branch, submit PRs. You can also suggest new features or report issues.

---

## 👨‍💻 Developers

![siti Logo](https://github.com/GAURAKH-97/Portfolio/blob/main/Image/my/logo.png)
 
**Developed by:** **Gaurakh** 

📧 Contact: [Gmail](gparjapati097@@gmail.com)

🌐 Website: [Gaurakh-codes](https://gaurakh-codes.netlify.app/)

