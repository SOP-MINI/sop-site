#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <cstring>
#include <coroutine>
#include <format>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sched.h>
#include <signal.h>

constexpr int PORT = 8090;
constexpr int MAX_EVENTS = 10000;
constexpr int MAX_FDS = 65536;

// ==========================================
// MOCK LOGIKI BIZNESOWEJ
// ==========================================
long long recursive_burn(long long depth) {
    if (depth <= 0) return 1;
    return depth + recursive_burn(depth - 1);
}

// ==========================================
// KONTEKST WORKERA
// ==========================================
struct WorkerCtx {
    int id;
    int epoll_fd;
    std::thread thread_obj;
};

// ==========================================
// INFRASTRUKTURA KORUTYN (C++20)
// ==========================================

// Task (Fire-and-Forget) – pamięć jest automatycznie zwalniana po dojściu do co_return
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// 1. Prosty Awaiter dla Gniazda Nasłuchującego (Zawsze usypia)
struct AsyncWait {
    int epoll_fd;
    int fd;
    uint32_t events;

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> h) {
        struct epoll_event ev;
        ev.events = events | EPOLLONESHOT;
        ev.data.ptr = h.address(); // Natywny adres korutyny leci do Linuksa!

        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1 && errno == ENOENT) {
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
        }
    }
    void await_resume() const noexcept {}
};

// 2. Inteligentny Awaiter Odczytu (Zamaskowany read() i Fast-Path)
struct AsyncRead {
    int epoll_fd;
    int fd;
    void* buf;
    size_t count;
    ssize_t result = 0;

    bool await_ready() {
        result = read(fd, buf, count);
        // Jeśli mamy dane lub twardy błąd (ale nie EAGAIN) - nie usypiaj korutyny!
        if (result >= 0 || (result == -1 && errno != EAGAIN && errno != EWOULDBLOCK)) return true;
        return false;
    }

    void await_suspend(std::coroutine_handle<> h) {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLONESHOT;
        ev.data.ptr = h.address();
        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1 && errno == ENOENT) {
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
        }
    }

    ssize_t await_resume() {
        // Obudzili nas, więc czytamy właściwe dane
        if (result == -1) result = read(fd, buf, count);
        return result; // Zwracane wprost pod operator co_await
    }
};

// 3. Inteligentny Awaiter Zapisu (Zamaskowany write() i Fast-Path)
struct AsyncWrite {
    int epoll_fd;
    int fd;
    const void* buf;
    size_t count;
    ssize_t result = 0;

    bool await_ready() {
        result = write(fd, buf, count);
        if (result >= 0 || (result == -1 && errno != EAGAIN && errno != EWOULDBLOCK)) return true;
        return false;
    }

    void await_suspend(std::coroutine_handle<> h) {
        struct epoll_event ev;
        ev.events = EPOLLOUT | EPOLLONESHOT;
        ev.data.ptr = h.address();
        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1 && errno == ENOENT) {
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
        }
    }

    ssize_t await_resume() {
        if (result == -1) result = write(fd, buf, count);
        return result;
    }
};

// ==========================================
// FUNKCJE POMOCNICZE
// ==========================================
bool make_socket_non_blocking(int sfd) {
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) return false;
    return fcntl(sfd, F_SETFL, flags | O_NONBLOCK) != -1;
}

void close_client(int epoll_fd, int fd) {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
    close(fd);
}

// ==========================================
// LOGIKA KLIENTA (Czysta, abstrakcyjna pętla)
// ==========================================
Task handle_client(WorkerCtx* worker, int client_fd) {
    char buf[1024];
    size_t read_len = 0;

    try {
        while (true) {
            // Zamaskowane wywołanie systemowe! Zero ręcznych flag i wywołań read()
            ssize_t count = co_await AsyncRead{worker->epoll_fd, client_fd, buf + read_len, sizeof(buf) - 1 - read_len};

            // Jedyne EAGAIN jakie mogło tu dotrzeć to fałszywy alarm z jądra
            if (count == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) continue;

            // Koniec danych (0) lub krytyczny błąd sieci (-1)
            if (count <= 0) break;

            read_len += count;
            buf[read_len] = '\0';

            if (char* newline = strchr(buf, '\n')) {
                *newline = '\0';
                std::string cmd(buf);

                if (cmd.starts_with("PING")) {
                    co_await AsyncWrite{worker->epoll_fd, client_fd, "PONG\n", 5};
                }
                else if (cmd.starts_with("BURN")) {
                    long long depth = (cmd.length() > 5) ? std::stoll(cmd.substr(5)) : 0;
                    long long sum = recursive_burn(depth);

                    std::string resp = "RESULT " + std::to_string(sum) + "\n";
                    co_await AsyncWrite{worker->epoll_fd, client_fd, resp.c_str(), resp.length()};
                }
                else if (cmd.starts_with("STREAM")) {
                    long bytes_to_send = 65536;
                    if (cmd.length() > 7) bytes_to_send = std::stol(cmd.substr(7));

                    char chunk[16384];
                    memset(chunk, 'A', sizeof(chunk));

                    while (bytes_to_send > 0) {
                        size_t to_write = std::min((long)sizeof(chunk), bytes_to_send);
                        ssize_t written = co_await AsyncWrite{worker->epoll_fd, client_fd, chunk, to_write};

                        if (written > 0) {
                            bytes_to_send -= written;
                        } else if (written <= 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                            break; // Twardy błąd podczas streamowania
                        }
                    }

                    co_await AsyncWrite{worker->epoll_fd, client_fd, "DONE\n", 5};
                }
                read_len = 0;
            }
        }
    } catch (...) { }

    close_client(worker->epoll_fd, client_fd);
    co_return;
}

// ==========================================
// KORUTYNA AKCEPTORA
// ==========================================
Task acceptor_coroutine(WorkerCtx* worker, int listener_fd) {
    try {
        while (true) {
            // Usypia do momentu pojawienia się sygnału na gnieździe
            co_await AsyncWait{worker->epoll_fd, listener_fd, EPOLLIN};

            while (true) {
                struct sockaddr_in client_addr;
                socklen_t addrlen = sizeof(client_addr);
                int client_fd = accept(listener_fd, (struct sockaddr*)&client_addr, &addrlen);

                if (client_fd == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                    break;
                }

                if (client_fd >= MAX_FDS) {
                    close(client_fd);
                    continue;
                }

                make_socket_non_blocking(client_fd);

                // Uruchamiamy korutynę klienta (zostanie wpięta do epolla automatycznie!)
                handle_client(worker, client_fd);
            }
        }
    } catch (...) { }

    co_return;
}

// ==========================================
// ZUNIFIKOWANA PĘTLA REAKTORA
// ==========================================
void worker_loop(WorkerCtx* worker) {
    int listener_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listener_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(listener_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)); // Rozdział ruchu w jądrze!
    make_socket_non_blocking(listener_fd);

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(listener_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind failed");
        return;
    }
    listen(listener_fd, MAX_FDS);

    std::cout << std::format("Worker {} processes events (epoll_fd={})\n", worker->id, worker->epoll_fd);;

    // Odpalamy nasłuchiwacz jako zwykłą korutynę
    acceptor_coroutine(worker, listener_fd);

    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int nfds = epoll_wait(worker->epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++) {
            // Odtwarzamy uchwyt korutyny z surowego wskaźnika pamięci Linuksa
            auto coro = std::coroutine_handle<>::from_address(events[i].data.ptr);

            if (coro && !coro.done()) {
                coro.resume(); // Wybudzenie bez zbędnych if'ów!
            }
        }
    }
}

// ==========================================
// ENTRY POINT
// ==========================================
int main() {
    signal(SIGPIPE, SIG_IGN);

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    if (sched_getaffinity(0, sizeof(cpu_set_t), &cpuset) == -1) {
        perror("sched_getaffinity");
        return EXIT_FAILURE;
    }

    int num_cores = CPU_COUNT(&cpuset);
    std::vector<WorkerCtx> workers(num_cores);

    for (int i = 0; i < num_cores; ++i) {
        workers[i].id = i;
        workers[i].epoll_fd = epoll_create1(0);

        workers[i].thread_obj = std::thread([&workers, i]() {
            worker_loop(&workers[i]);
        });
    }

    for (auto& w : workers) {
        if (w.thread_obj.joinable()) {
            w.thread_obj.join();
        }
    }

    return 0;
}
