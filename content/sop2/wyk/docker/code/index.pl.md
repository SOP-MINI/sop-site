---
title: "Code"
weight: 99
---

[View on GitHub]({{< github_url >}})

### Host example

```bash
make demo
./demo --sleep
```
[demo.c]({{< github_url "demo.c" >}})

```shell
ps aux | grep demo
```

Display process namespace association:

```shell
ls -l /proc/$(pidof demo)/ns
```

Compare namespaces with your shell:

```shell
ls -l /proc/$$/ns
```

### Build docker image

```shell
docker build -t demo .
```

```shell
docker image list demo
```

```shell
docker run demo
docker run -it --name demo_sleep --rm demo --sleep
```

Note PID = 1 here. However, from host perspective there is a regular process `demo` with a high PID:

```shell
ps aux | grep demo
```

`docker top` lists processes within a single container:

```shell
docker top demo_sleep
```

`docker ps` lists containers:

```shell
docker ps -a
```

Inspect containerized process namespace association:

```shell
HOST_PID=$(docker inspect -f '{{.State.Pid}}' demo_sleep)
echo $HOST_PID
sudo ls -l /proc/$HOST_PID/ns/
```

Note the differences from what we've seen before.

### PID Namespace

Containerized processes _see_ only other processes within the same container.

Try running forking process and observe PIDs:

```shell
docker run -it --rm demo --fork
```

Try running it multiple times in parallel.

Containers cannot access host PIDs. 

```shell
HOST_PID=$$
echo $HOST_PID
docker run -it --rm demo --kill $HOST_PID
```

#### DIY PID namespace

```shell
make pidns
```
[pidns.c]({{< github_url "pidns.c" >}})

```shell
sudo ./pidns
```

Note that child PID from perspective of parent is different from what child sees.

### NET namespace

```shell
./demo --if
```

Try listing container interfaces:

```shell
docker run -it --rm demo --if
```

Now bind port 8080 within host and then within the container:

```shell
nc -u -l 8080
```

```shell
docker run -it --rm --name=demo_udp demo --udp
```

Send datagram to host:

```shell
echo "Hello Host" | nc -u -w1 127.0.0.1 8080
```

And to the container:

```shell
CONTAINER_IP=$(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' demo_udp)
echo "Container IP: $CONTAINER_IP"
echo "Hello Container" | nc -u -w1 $CONTAINER_IP 8080
```

See interfaces from the container:

```shell
docker exec demo_udp ip a
```

Note network namespaces created by docker live in a different directory compared to the standard ones created by
`ip netns` tool. You can find them with:

```shell
SANDBOX_KEY=$(docker inspect -f '{{.NetworkSettings.SandboxKey}}' demo_udp)
echo $SANDBOX_KEY
```

#### Port exporting

```shell
docker run -it --rm --name=demo_cpu -p 80:8080/udp --cpus="0.1" demo --udp
```

Show `iptables` rules:

```shell
sudo iptables -t nat -L DOCKER -n
```

Show auxiliary process:

```shell
ps aux | grep docker-proxy
```

Send something:

```shell
echo "Hello!" | nc -u -w1 localhost 80
```

### MNT Namespace

```shell
docker run -it --name demo_fs demo --write
```

```shell
ls -l /tmp/container_secret.txt
```

```shell
HOST_PID=$(docker inspect -f '{{.State.Pid}}' demo_fs)
sudo cat /proc/$HOST_PID/mountinfo | grep '/ / '
```

```shell
HOST_PID=$(docker inspect -f '{{.State.Pid}}' demo_fs)
UPPER_DIR=$(sudo cat /proc/$HOST_PID/mountinfo | grep '/ / ' | grep -o 'upperdir=[^,]*' | cut -d= -f2)
LOWER_DIR=$(sudo cat /proc/$HOST_PID/mountinfo | grep '/ / ' | grep -o 'lowerdir=[^,]*' | cut -d= -f2)
echo $UPPER_DIR
echo $LOWER_DIR
```

```shell
HOST_PID=$(docker inspect -f '{{.State.Pid}}' demo_fs)
UPPER_DIR=$(sudo cat /proc/$HOST_PID/mountinfo | grep '/ / ' | grep -o 'upperdir=[^,]*' | cut -d= -f2)
sudo tree $UPPER_DIR
```

```shell
docker container rm demo_fs
```

### DIY virtual root

```shell
make mntns
```

```shell
sudo ./mntns ./root
```

#### Bind mounts

```shell
docker run -it --name demo_bind -v ./data:/data demo --write /data/trwaly_sekret.txt
```

```shell
HOST_PID=$(docker inspect -f '{{.State.Pid}}' demo_bind)
sudo cat /proc/$HOST_PID/mountinfo | grep '/data'
```

```shell
stat ./data/trwaly_sekret.txt
cat ./data/trwaly_sekret.txt
```

```shell
sudo rm -Rf ./data
```

```shell
docker rm -f demo_bind
```

### Control Groups

```shell
docker run -it --rm -m 50m demo --mem
```

```shell
CONTAINER_ID=$(docker inspect -f '{{.Id}}' demo_sleep)
echo $CONTAINER_ID
ls /sys/fs/cgroup/system.slice/docker-$CONTAINER_ID.scope/
```

```shell
docker run -it --rm --name=demo_cpu --cpus="0.1" demo --cpu
```

```shell
HOST_PID=$(docker inspect -f '{{.State.Pid}}' demo_cpu)
htop -p $HOST_PID
```

```shell
CONTAINER_ID=$(docker inspect -f '{{.Id}}' demo_cpu)
echo $CONTAINER_ID
cat /sys/fs/cgroup/system.slice/docker-$CONTAINER_ID.scope/cpu.max
```

### Cleanup

Ensure there are no leftovers:

```shell
docker container ps
```

Stop if anything is still running with `docker kill`.

```shell
docker container prune
```
