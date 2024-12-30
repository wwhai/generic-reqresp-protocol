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

#ifndef BYTE_PARSER_H
#define BYTE_PARSER_H

#include <stdint.h>

// 定义包头和包尾长度
#define PACKET_HEADER_SIZE 2
#define PACKET_TAIL_SIZE 2

// 定义包头和包尾结构体
typedef struct
{
    uint8_t Head[2];
    uint8_t Tail[2];
} PacketEdger;

// 数据检查函数指针类型
typedef int (*DataChecker)(const uint8_t *data, size_t length);

// 通用字节解析器结构体
typedef struct
{
    PacketEdger edger;
    DataChecker checker;
} GenericByteParser;

// 创建一个新的解析器
GenericByteParser *NewGenericByteParser(DataChecker checker, PacketEdger edger);

// 解析字节流，提取有效数据包
uint8_t *ParseBytes(GenericByteParser *parser, const uint8_t *b, size_t b_length, size_t *result_length);

// 释放解析器内存
void FreeParser(GenericByteParser *parser);

// 示例数据检查函数，可根据需求修改
int SimpleDataChecker(const uint8_t *data, size_t length);

#endif