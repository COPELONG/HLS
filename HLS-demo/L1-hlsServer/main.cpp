//
// Created by bxc on 2023/1/7.
//

#include <stdint.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "Utils/Log.h"
#pragma comment(lib, "ws2_32.lib")
#include "Connection.h"
#include <thread>

int main() {

	/*
	
	ffmpeg命令行生成m3u8切片（1）

	ffmpeg -i input.mp4 -c:v libx264 -c:a copy -f hls -hls_time 10 -hls_list_size 0 -hls_start_number 0 input/index.m3u8

	备注：-hls_time n: 设置每片的长度，默认值为2,单位为秒
	      -hls_list_size n:设置播放列表保存的最多条目，设置为0会保存有所片信息，默认值为5
	      -hls_start_number n:设置播放列表中sequence number的值为number，默认值为0
	      -hls_wrap n:设置多少片之后开始覆盖，如果设置为0则不会覆盖，默认值为0
	          这个选项能够避免在磁盘上存储过多的片，而且能够限制写入磁盘的最多的片的数量

	ffmpeg命令行生成m3u8切片（2）

	ffmpeg -i input.mp4 -fflags flush_packets 
	-max_delay 2 -flags -global_header 
	-hls_time 5 -hls_list_size 0 -vcodec libx264 -acodec aac -r 30 -g 60 index.m3u8

	*/
	//ffplay -i http://127.0.0.1:8080/index.m3u8


	int port = 8080;
	LOGI("1-hlsServer http://127.0.0.1:%d/index.m3u8", port);

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		LOGE("WSAStartup error");
		return -1;
	}

	SOCKET serverFd;
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//server_addr.sin_addr.s_addr = inet_addr("192.168.2.61");
	server_addr.sin_port = htons(port);
	serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (bind(serverFd, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		LOGE("socket bind error");
		return -1;
	}
	if (listen(serverFd, SOMAXCONN) < 0) {
		LOGE("socket listen error");
		return -1;
	}


	while (true)
	{
		LOGI("等待新连接...");
		int len = sizeof(SOCKADDR);
		SOCKADDR_IN accept_addr;
		int clientFd = accept(serverFd, (SOCKADDR*)&accept_addr, &len);
		//const char* clientIp = inet_ntoa(accept_addr.sin_addr);

		if (clientFd == SOCKET_ERROR) {
			LOGE("accept connection error");
			break;
		}
		LOGI("发现新连接 clientFd=%d", clientFd);

		//std::thread t([&]() {
			Connection conn(clientFd);
			conn.start();
		//});

		//t.detach();


	}

	closesocket(serverFd);
	return 0;
}