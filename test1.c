/*
 * 2021-03-15 By Titto
 * 简单的 HTTP 服务器
 */
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <string.h>

uv_loop_t *loop;
uv_tcp_t server;

char* response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type:text/html;charset=utf-8\r\n"
                           "Content-Length:18\r\n"
                           "\r\n"
                           "Welcome to tinyweb";

void on_new_connection(uv_stream_t* _server, int status);

void after_uv_write(uv_write_t* w, int status);

void after_uv_close(uv_handle_t* handle);

int main() {
    loop = uv_default_loop();
    uv_loop_init(loop);
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 4444, &addr);
    uv_tcp_init(loop, &server);
    uv_tcp_bind(&server, (const struct sockaddr *) &addr, 0);
    uv_listen((uv_stream_t *) &server, 20, on_new_connection);
    uv_run(loop, UV_RUN_DEFAULT);
}

void on_new_connection(uv_stream_t* _server, int status) {
    if(status) {
        fprintf(stderr, "Error\n");
        return;
    }
    uv_tcp_t* client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    uv_accept(_server, (uv_stream_t *) client);

    uv_buf_t buf;
    uv_write_t* writer = (uv_write_t*) malloc(sizeof(uv_write_t));
    buf = uv_buf_init(response, strlen(response));
    writer->data = (void*) response;
    uv_write(writer, (uv_stream_t *) client, &buf, 1, after_uv_write);

}


void after_uv_write(uv_write_t* w, int status) {
    uv_close((uv_handle_t*)w->handle, after_uv_close);
    free(w);
}

void after_uv_close(uv_handle_t* handle) {
    free(handle);
}



