# Wind/Tempest Tree

> ![WARNING]
> This tree is not final and will probably change as the project evolves.

```
/
├── Applications
│   ├── app1 -> ...
│   ├── app2 -> ...
│   └── app3 -> ...
├── System
│   ├── lib
│   ├── boot
│   ├── mount
│   ├── devices
│   ├── temp
│   ├── drivers
│   ├── kernel
│   ├── logs
│   ├── services
│   └── config
├── Home
│   ├── user1
│   │   ├── .config
│   │   ├── Desktop
│   │   ├── Documents
│   │   └── Downloads
│   ├── user2
│   │   ├── .config
│   │   ├── Desktop
│   │   ├── Documents
│   │   └── Downloads
│   └── user3
│       ├── .config
│       ├── Desktop
│       ├── Documents
│       └── Downloads
```

# Why is not POSIX?

> **yes.**

Because POSIX tree is too big and complex. It's a mess.
Wind/Tempest is designed for humans, not for 40-year-old standards made for UNIX servers.