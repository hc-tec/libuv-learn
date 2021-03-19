//
// Created by titto on 2021/3/18.
//
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "http-parser.h"

unsigned at = 0;
unsigned line_at = 0;
static const char EOL = '\r';

void copy_char_pointer(char* copy, const char* raw_data, unsigned len) {
    for(unsigned i=0;i<len;++i) {
        *(copy+i) = *(raw_data+i);
    }
}

void read_until_flag(char* cur, const char* buf, char flag) {
    skip_space(buf);
    short len = 1;
    while (*(buf+at) != flag) {
        len++;
        at++;
        *(cur+len) = *buf;
    }
    *cur = *(cur+len);
    cur++;
    *cur = '\0';
}

void read_until_space(char* cur, const char* buf) {
    read_until_flag(cur, buf, ' ');
}

void read_until_line(char* cur, const char* buf) {
    skip_space(buf);
    cur = (char*) (buf + at);
    short len = 0;
    while (*(buf + at) != EOL) {
        len++;
        at++;
    }
    *(cur+len) = '\0';
    // skip \r\n to new line
    at += 2;
}

void skip_space(const char* buf) {
    while (isspace(*(buf+at))) at++;
}

void parse_req_info(struct http_request_s* req, char* line, unsigned len) {
    split_result splitResult = split(line, len, ' ', 10);
    req->method = *splitResult.arr;
    req->path = *(splitResult.arr+1);
    req->family = *(splitResult.arr+2);
}

void parse_req_body(struct http_request_s* req, char* line, unsigned len) {
    split_result results = split(line, len, ':', 1);
    char* key = *results.arr;
    (*(results.arr+1))++; // skip the redundant space on the value field
    static char *value;
    value = *(results.arr + 1);
    char fir_c = *key, sec_c = *(key+1);
    unsigned value_len = strlen(value);
    // User-Agent
    if(fir_c == 'U' && sec_c == 's') {
        http_user_agent user_agent;
        user_agent.content = (char*) malloc(value_len);
        copy_char_pointer(user_agent.content, value, value_len);
        req->user_agent = user_agent;
    }
    // Accept
    else if (fir_c == 'A') {
        char distinguish_c = *(key+7);
        // Accept-Charset
        if(distinguish_c == 'C') {
            http_accept_charset charset;
        }
        // Accept-Encoding
        else if (distinguish_c == 'E') {
            split_result encoding_res = split(value, value_len, ',', DEFAULT_VALUE_SIZE);

            http_accept_encoding encoding;
            memset(encoding.items, 0, sizeof(encoding.items));
            short i = 0;
            char* cur_encode = NULL;
            for(;;++i) {
                cur_encode = *(encoding_res.arr+i);
                if(cur_encode == NULL) break;
                // skip space
                if(*cur_encode == ' ') {
                    cur_encode++;
                }
                unsigned code_len = strlen(cur_encode);
                *(encoding.items+i) = (char*) malloc(sizeof(char)*code_len);
                copy_char_pointer(*(encoding.items+i), cur_encode, code_len);
            }
            encoding.count = i;
            req->accept_encoding = encoding;
        }
        // Accept-Language
        else if (distinguish_c == 'L') {
            http_accept_lang lang;
        }
        // Accept-Ranges
        else if (distinguish_c == 'R') {
            http_accept_ranges ranges;
        }
        // Authorization
        else if (distinguish_c == 'z') {
            http_authorization authorization;
            authorization.content = (char*) malloc(sizeof(char)*value_len);
            copy_char_pointer(authorization.content, value, value_len);
            req->authorization = authorization;
        }
        //  Accept
        else {
            http_accept accept;

        }
    }

}

split_result split(const char* line, unsigned len, char flag, unsigned count) {
    split_result result;
    memset(result.arr, 0, sizeof(result.arr));
    short num = 0, _at = 0;
    char mid_word[DEFAULT_VALUE_SIZE];
    memset(mid_word, 0, sizeof(mid_word));
    for (unsigned i=0;i<len;++i) {
        char cur = *(line+i);
        if(i == len-1) {
            mid_word[i] = cur;
        }
        if(cur == flag || i == len-1) {
            result.arr[num] = (char*) malloc(sizeof(mid_word));
            copy_char_pointer(result.arr[num], mid_word, sizeof(mid_word));
            memset(mid_word, 0, sizeof(mid_word));
            if(num == count) break;
            num++;
            _at = 0;
            continue;
        }
        mid_word[_at] = cur;
        _at++;
    }
    return result;
}

struct http_request_s http_parser(const char* recv_buf) {

    http_request* req = (http_request*) malloc(sizeof(http_request));
    unsigned buf_len = strlen(recv_buf);
    unsigned short line = 1; // current line of data
//    char* copy_recv_buf = (char*) malloc(sizeof(char)*buf_len);
//    copy_raw_data(copy_recv_buf, recv_buf, buf_len);

    while (strlen(recv_buf) != 2) {
        read_until_line(NULL, recv_buf);
        char* cur = (char*) malloc(sizeof(char)*at);;
        copy_char_pointer(cur, recv_buf, at);
        if(line == 1) {
            parse_req_info(req, cur, at);
        } else {
            parse_req_body(req, cur, at);
        }
        recv_buf = recv_buf + at;
        at = 0;
        line++;
    }

    return *req;
}

