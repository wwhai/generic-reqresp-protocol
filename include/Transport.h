// Copyright (C) 2024 wwhai
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 假设这是你的串口端口类型，根据实际情况修改
typedef struct SerialPort SerialPort;

// 包头和包尾结构体
typedef struct
{
    uint8_t Head[2];
    uint8_t Tail[2];
} PacketEdger;

// 通用字节解析器结构体，假设已经在 ByteParser.h 中定义
typedef struct GenericByteParser GenericByteParser;

// 传输结构体
typedef struct
{
    SerialPort *port;
    GenericByteParser *parser;
    // 假设这是你的日志结构体，根据实际情况修改
    void *logger;
    int writeTimeout;
    int readTimeout;
    uint8_t *buffer;
    size_t bufferSize;
} Transport;

// 初始化传输结构体
Transport *InitTransport(SerialPort *port, GenericByteParser *parser, void *logger, int writeTimeout, int readTimeout, size_t bufferSize);

// 释放传输结构体内存
void FreeTransport(Transport *transport);

// Write 方法从串口写入数据
int Write(Transport *transport, const uint8_t *data, size_t dataLength);

// Read 方法从串口读取数据并解析
uint8_t *Read(Transport *transport, size_t *resultLength);

// 辅助函数：添加包头和包尾
uint8_t *AddPacketEdges(const PacketEdger *edger, const uint8_t *data, size_t dataLength, size_t *resultLength);

#endif