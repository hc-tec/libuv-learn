//
// Created by titto on 2021/3/18.
// Parse HTTP Request Header
//

#ifndef LIBUV_TEST_HTTP_PARSER_H
#define LIBUV_TEST_HTTP_PARSER_H

// default value size of request headers' value field
#define DEFAULT_VALUE_SIZE 100

// use it when the type of field value as 'text/plain, text/html' or 'en,zh'
#define RECV_ITEMS \
    char* items[DEFAULT_VALUE_SIZE]; \
    short count;

// use it when the type of field value as 'no-cache' or 'close'
#define RECV_SINGLE_ITEM_CHAR \
    char* content;

// use it when the type of field value as 10 or 2313
#define RECV_SINGLE_ITEM_INT \
    int number;

#define split_result struct split_result_s

#define http_accept struct http_accept_s
#define http_accept_charset struct http_accept_charset_s
#define http_accept_encoding struct http_accept_encoding_s
#define http_accept_lang struct http_accept_lang_s
#define http_accept_ranges struct http_accept_ranges_s
#define http_authorization struct http_authorization_s
#define http_connection struct http_connection_s
#define http_content_length struct http_content_length_s
#define http_content_type struct http_content_type_s
#define http_host struct http_host_s
#define http_user_agent struct http_user_agent_s
#define http_query_params struct http_query_params_s
#define http_request struct http_request_s

struct split_result_s {
    char* arr[DEFAULT_VALUE_SIZE];
};

struct http_accept_s {
    RECV_ITEMS;
};

struct http_accept_charset_s {
    RECV_ITEMS;
};

struct http_accept_encoding_s {
    RECV_ITEMS;
};

struct http_accept_lang_s {
    RECV_ITEMS;
};

struct http_accept_ranges_s {
    RECV_ITEMS;
};

struct http_authorization_s {
    RECV_SINGLE_ITEM_CHAR;
};

struct http_connection_s {
    RECV_SINGLE_ITEM_CHAR;
};

struct http_content_length_s {
    RECV_SINGLE_ITEM_INT;
};

struct http_content_type_s {
    RECV_SINGLE_ITEM_CHAR;
};

struct http_host_s {
    RECV_SINGLE_ITEM_CHAR;
};

struct http_user_agent_s {
    RECV_SINGLE_ITEM_CHAR;
};

struct http_query_params_s {
    RECV_ITEMS;
};

struct http_request_s {
    char* method; // request method
    char* path;
    char* family;
    struct http_accept_s accept;
    struct http_accept_charset_s accept_charset;
    struct http_accept_encoding_s accept_encoding;
    struct http_authorization_s authorization;
    struct http_connection_s connection;
    struct http_content_type_s content_type;
    struct http_host_s host;
    struct http_user_agent_s user_agent;
    struct http_query_params_s query_params;
};
// raw_data copy to copy array
void copy_char_pointer(char* copy, const char* raw_data, unsigned len);

// read content before flag char
void read_until_flag(char* cur, const char* buf, char flag);

// read content before space
void read_until_space(char* cur, const char* buf);

// read content before new line
void read_until_line(char* cur, const char* buf);

// skip space
void skip_space(const char* buf);

struct http_request_s http_parser(const char* recv_buf);

void parse_req_info(struct http_request_s* req, char* line, unsigned len);

void parse_req_body(struct http_request_s* req, char* line, unsigned len);

struct split_result_s split(const char* line, unsigned len, char flag, unsigned count);

#endif //LIBUV_TEST_HTTP_PARSER_H
