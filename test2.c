//
// Created by titto on 2021/3/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <string.h>
//#include "lib/http-parser/http_parser.h"
#include "http-parser.h"

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

void on_uv_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

void on_uv_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf);

//int on_message_begin(struct http_parser* parser);
//
//int on_url(struct http_parser* parser, const char* at, size_t len);
//
//int on_header_field(struct http_parser* parser, const char* at, size_t len);
//
//int on_header_value(struct http_parser* parser, const char* at, size_t len);
//
//int on_body(struct http_parser* parser, const char* at, size_t len);

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
    uv_read_start((uv_stream_t*)client, on_uv_alloc, on_uv_read);
}


void after_uv_write(uv_write_t* w, int status) {
    uv_close((uv_handle_t*)w->handle, after_uv_close);
    free(w);
}

void after_uv_close(uv_handle_t* handle) {
    free(handle);
}

void on_uv_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    *buf = uv_buf_init(malloc(suggested_size), suggested_size);
}

void on_uv_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {

    http_request req = http_parser(buf->base);


    /*
    struct http_parser_settings parser_set;
    // 设置回调函数
    // 坑：「每个回调函数都需要设置」，即便为 NULL
    parser_set.on_message_begin = on_message_begin;
    parser_set.on_url = on_url;
    parser_set.on_header_field = on_header_field;
    parser_set.on_header_value = on_header_value;
    parser_set.on_body = on_body;
    parser_set.on_headers_complete = NULL;
    parser_set.on_message_complete = NULL;

    size_t parsed_status;
    http_parser* parser;
    parser = (http_parser*) malloc(sizeof(http_parser));
    // 初始化 parser
    http_parser_init(parser, HTTP_REQUEST);
    // 开始解析
    parsed_status = http_parser_execute(parser, &parser_set, buf->base, strlen(buf->base));
    free(parser);
    */

    // 响应
    uv_buf_t res_buf;
    uv_write_t* writer = (uv_write_t*) malloc(sizeof(uv_write_t));
    res_buf = uv_buf_init(response, strlen(response));
    writer->data = (void*) response;
    uv_write(writer, client, &res_buf, 1, after_uv_write);

}

//int on_message_begin(struct http_parser* parser) {
//    return 0;
//}
//
//int on_url(struct http_parser* parser, const char* at, size_t len) {
//    printf("Url: %s\nlen=%zu\n", at, len);
//    return 0;
//}
//
//int on_header_field(struct http_parser* parser, const char* at, size_t len) {
//    printf("Header Field: %s\n", at);
//    return 0;
//}
//
//int on_header_value(struct http_parser* parser, const char* at, size_t len) {
//    printf("Header Value: %s\n", at);
//    return 0;
//}
//
//int on_body(struct http_parser* parser, const char* at, size_t len) {
//    printf("Body: %s\n", at);
//    return 0;
//}
