import asyncio
import os
import time
import sys
from prometheus_client import start_http_server, Gauge, Counter, Histogram

TARGET_CONNECTIONS = int(sys.argv[1]) if len(sys.argv) > 1 else 1000
CMD = f"BURN 300"

affinity = os.sched_getaffinity(0)

if len(affinity) > 1:
    print(f"⚠️ WARNING: Pinned to multiple cores!")

primary_core = list(affinity)[0]

INSTANCE_ID = str(primary_core)
METRICS_PORT = 9000 + primary_core

CLIENTS_CONNECTED = Gauge('python_clients_connected', 'Connected TCP clients', ['instance_id'])
REQUESTS_SENT_TOTAL = Counter('python_client_requests_sent_total', 'Sent requests', ['instance_id', 'command'])
REQUEST_ERRORS = Counter('python_client_request_errors_total', 'Request errors', ['instance_id', 'type'])
CONNECTION_ERRORS = Counter('python_client_connection_errors_total', 'Connection establishment errors', ['instance_id'])
LATENCY_HISTOGRAM = Histogram(
    'python_client_request_latency_seconds',
    'Request latency',
    ['instance_id'],
    buckets=[0.001, 0.005, 0.01, 0.05, 0.1, 0.5, 1.0, 2.0, 5.0]
)

async def monitor_task():
    while True:
        await asyncio.sleep(3)
        # TODO

async def client_task():
    cmd = f"{CMD}\n".encode()

    while True:
        writer = None
        try:
            reader, writer = await asyncio.wait_for(asyncio.open_connection('127.0.0.1', 8090), timeout=3.0)
            CLIENTS_CONNECTED.labels(instance_id=INSTANCE_ID).inc()

            while True:
                start_req = time.perf_counter()
                REQUESTS_SENT_TOTAL.labels(instance_id=INSTANCE_ID, command='STREAM').inc()

                async def do_request():
                    writer.write(cmd)
                    await writer.drain()

                    response_line = await reader.readline()

                    if not response_line:
                        raise ConnectionError("Server closed connection (EOF) unexpectedly")

                    response_str = response_line.decode('utf-8').strip()

                    if not response_str.startswith("RESULT"):
                        raise ValueError(f"Server returned wrong response: '{response_str}'")

                try:
                    await asyncio.wait_for(do_request(), timeout=3.0)

                    duration = time.perf_counter() - start_req
                    LATENCY_HISTOGRAM.labels(instance_id=INSTANCE_ID).observe(duration)

                    await asyncio.sleep(0.1)

                except asyncio.TimeoutError:
                    REQUEST_ERRORS.labels(instance_id=INSTANCE_ID, type='timeout').inc()
                    break

                except Exception as req_e:
                    REQUEST_ERRORS.labels(instance_id=INSTANCE_ID, type='io_error').inc()
                    break

        except Exception as conn_e:
            CONNECTION_ERRORS.labels(instance_id=INSTANCE_ID).inc()

        finally:
            if writer is not None:
                CLIENTS_CONNECTED.labels(instance_id=INSTANCE_ID).dec()
                try:
                    writer.close()
                    await writer.wait_closed()
                except Exception:
                    pass

        await asyncio.sleep(1.0)


async def main():
    start_http_server(METRICS_PORT)
    print(f"[Core #{INSTANCE_ID}] Target Connections: {TARGET_CONNECTIONS} Command: '{CMD}' Metrics port: {METRICS_PORT}")

    import resource
    resource.setrlimit(resource.RLIMIT_NOFILE, (65535, 65535))

    tasks = [asyncio.create_task(client_task()) for _ in range(TARGET_CONNECTIONS)]
    await asyncio.gather(*tasks, return_exceptions=True)

if __name__ == "__main__":
    asyncio.run(main())
