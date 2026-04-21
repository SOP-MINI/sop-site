---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

## Chat server

Build and run the simple TCP chat server based on `select()`:

```shell
make select_chat && ./select_chat
```
[select_chat.c]({{< github_url "select_chat.c" >}})

Connect a few clients and send some messages:

```shell
nc localhost 8090
```

Now try `epoll()` version:

```shell
make epoll_chat && ./epoll_chat
```
[epoll_chat.c]({{< github_url "epoll_chat.c" >}})
```