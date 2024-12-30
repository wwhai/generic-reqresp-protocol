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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ByteParser.h"

// 创建一个新的解析器
GenericByteParser *NewGenericByteParser(DataChecker checker, PacketEdger edger)
{
    GenericByteParser *parser = (GenericByteParser *)malloc(sizeof(GenericByteParser));
    if (parser == NULL)
    {
        // 内存分配失败
        return NULL;
    }
    parser->edger = edger;
    parser->checker = checker;
    return parser;
}
// 提取包头和包尾查找逻辑到单独的函数中
int FindPacketEdges(const uint8_t *b, size_t b_length, const PacketEdger *edger, size_t *startIdx, size_t *endIdx)
{
    *startIdx = -1;
    // 查找包头
    for (size_t i = 0; i < b_length - 1; i++)
    {
        if (b[i] == edger->Head[0] && b[i + 1] == edger->Head[1])
        {
            *startIdx = i;
            break;
        }
    }

    // 如果没有找到包头
    if (*startIdx == -1)
    {
        return -1;
    }

    *endIdx = -1;
    // 查找包尾
    for (size_t i = *startIdx + 2; i < b_length - 1; i++)
    {
        if (b[i] == edger->Tail[0] && b[i + 1] == edger->Tail[1])
        {
            *endIdx = i + 2; // 包尾的位置（包含包尾的字节）
            break;
        }
    }

    // 如果没有找到包尾
    if (*endIdx == -1)
    {
        return -1;
    }

    return 0;
}

// 修改 ParseBytes 函数，使用新的 FindPacketEdges 函数
uint8_t *ParseBytes(GenericByteParser *parser, const uint8_t *b, size_t b_length, size_t *result_length)
{
    size_t startIdx, endIdx;
    if (FindPacketEdges(b, b_length, &parser->edger, &startIdx, &endIdx) != 0)
    {
        return NULL;
    }

    // 计算包体数据长度
    size_t packetDataLength = endIdx - startIdx - PACKET_HEADER_SIZE - PACKET_TAIL_SIZE;
    // 分配内存存储包体数据
    uint8_t *packetData = (uint8_t *)malloc(packetDataLength);
    if (packetData == NULL)
    {
        // 内存分配失败
        return NULL;
    }
    // 复制包体数据（包含包头和包尾之间的部分）
    memcpy(packetData, b + startIdx + PACKET_HEADER_SIZE, packetDataLength);

    // 使用用户自定义的校验器验证数据
    if (parser->checker(packetData, packetDataLength) != 0)
    {
        free(packetData);
        return NULL;
    }

    *result_length = packetDataLength;
    return packetData;
}

// 释放解析器内存
void FreeParser(GenericByteParser *parser)
{
    free(parser);
}

// 示例数据检查函数，可根据需求修改
int SimpleDataChecker(const uint8_t *data, size_t length)
{
    // 这里可以添加具体的数据检查逻辑，例如检查数据的长度、范围、内容等
    // 暂时只做简单示例，检查数据长度是否大于 0
    if (length == 0)
    {
        return 1;
    }
    return 0;
}
