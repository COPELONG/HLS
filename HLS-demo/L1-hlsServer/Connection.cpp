//
// Created by bxc on 2023/1/7.
//

#include "Connection.h"
#include <iostream>
#include <string>
#include <thread>
#include "Utils/Log.h"
#include "Utils/Utils.h"

#pragma warning(disable: 4996)
#include <WinSock2.h>
#include <WS2tcpip.h>

char buf[1500000];

Connection::Connection(int clientFd):mClientFd(clientFd) {
    LOGI("clientFd=%d", clientFd);
}
Connection::~Connection() {
    LOGI("clientFd=%d", mClientFd);
    closesocket(mClientFd);

}
int Connection::start() {

    char bufRecv[2000] = { 0 };
    int bufRecvSize = recv(mClientFd, bufRecv, 2000, 0);
    LOGI("bufRecvSize=%d,bufRecv=%s", bufRecvSize, bufRecv);

    char uri[100] = { 0 };// /, /index0.ts, /index1.ts ,,,

    const char* sep = "\n";
    char* line = strtok(bufRecv, sep);
    while (line) {
        if (strstr(line, "GET")) {
            if (sscanf(line, "GET %s HTTP/1.1\r\n", &uri) != 1) {
                LOGE("parse uri error");

            }
        }
        line = strtok(NULL, sep);
    }
    printf("uri=%s\n", uri);

    std::string filename = "../data/test" + std::string(uri);
    FILE* fp;
    fp = fopen(filename.data(), "rb");
    if (!fp) {
        LOGE("fopen %s error", filename.data());
        return -1;
    }

    int bufLen = fread(buf, 1, sizeof(buf), fp);

    LOGI("bufLen=%d", bufLen);
    if (fp) {
        fclose(fp);
    }

    char http_headers[2000];

    if (0 == strcmp("/index.m3u8", uri)) {
        sprintf(http_headers, "HTTP/1.1 200 OK\r\n"
            "Access-Control-Allow-Origin: * \r\n"
            "Connection: keep-alive\r\n"
            "Content-Length: %d\r\n"
            "Content-Type: application/vnd.apple.mpegurl; charset=utf-8\r\n"
            "Keep-Alive: timeout=30, max=100\r\n"
            "Server: hlsServer\r\n"
            "\r\n",
            bufLen);
    }
    else {
        sprintf(http_headers, "HTTP/1.1 200 OK\r\n"
            "Access-Control-Allow-Origin: * \r\n"
            "Connection: close\r\n"
            "Content-Length: %d\r\n"
            "Content-Type: video/mp2t; charset=utf-8\r\n"
            "Keep-Alive: timeout=30, max=100\r\n"
            "Server: hlsServer\r\n"
            "\r\n",
            bufLen);
    }

    int http_headers_len = strlen(http_headers);
    LOGI("http_headers_len=%d", http_headers_len);

    send(mClientFd, http_headers, http_headers_len, 0);
    send(mClientFd, buf, bufLen, 0);

    Sleep(10);

    return 0;

}
