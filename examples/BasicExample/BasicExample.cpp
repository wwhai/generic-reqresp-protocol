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

#include <Arduino.h>
#include "ByteParser.h"
GenericByteParser *parser;

void setup()
{
    Serial.begin(9600); // 初始化串口通信，波特率为 9600

    // 初始化包头和包尾
    PacketEdger edger = {{0xAB, 0xCD}, {0xEF, 0x01}};

    // 创建解析器
    parser = NewGenericByteParser(SimpleDataChecker, edger);

    if (parser == NULL)
    {
        while (1)
        {
            Serial.println("Failed to create parser");
            delay(1000);
        }
    }
}

void loop()
{
    static uint8_t buffer[128]; // 缓冲区，存储从串口读取的数据
    static size_t buffer_index = 0;

    if (Serial.available() > 0)
    {
        buffer[buffer_index++] = Serial.read();

        // 假设最大数据包长度为 128 字节
        if (buffer_index >= 128)
        {
            buffer_index = 0;
        }

        size_t result_length;
        uint8_t *packetData = ParseBytes(parser, buffer, buffer_index, &result_length);

        if (packetData != NULL)
        {
            Serial.print("Received packet: ");
            for (size_t i = 0; i < result_length; i++)
            {
                Serial.print(packetData[i], HEX);
                Serial.print(" ");
            }
            Serial.println();

            // 处理接收到的数据包，例如解析其内容
            // 这里可以添加你的数据包处理逻辑

            // 释放解析结果的内存
            free(packetData);

            // 移动未处理的数据到缓冲区开头
            size_t remaining_length = buffer_index - (result_length + PACKET_HEADER_SIZE + PACKET_TAIL_SIZE);
            if (remaining_length > 0)
            {
                memmove(buffer, buffer + result_length + PACKET_HEADER_SIZE + PACKET_TAIL_SIZE, remaining_length);
            }
            buffer_index = remaining_length;
        }
    }

    delay(10);
}