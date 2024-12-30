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

#include "Transport.h"
#include "ByteParser.h"

// 辅助函数：添加包头和包尾
uint8_t *AddPacketEdges(const PacketEdger *edger, const uint8_t *data, size_t dataLength, size_t *resultLength)
{
    *resultLength = dataLength + 4; // 包头和包尾长度为 4
    uint8_t *newData = (uint8_t *)malloc(*resultLength);
    if (newData == NULL)
    {
        return NULL;
    }
    memcpy(newData, edger->Head, 2);
    memcpy(newData + 2, data, dataLength);
    memcpy(newData + 2 + dataLength, edger->Tail, 2);
    return newData;
}

// 初始化传输结构体
Transport *InitTransport(SerialPort *port, GenericByteParser *parser, void *logger, int writeTimeout, int readTimeout, size_t bufferSize)
{
    Transport *transport = (Transport *)malloc(sizeof(Transport));
    if (transport == NULL)
    {
        return NULL;
    }
    transport->port = port;
    transport->parser = parser;
    transport->logger = logger;
    transport->writeTimeout = writeTimeout;
    transport->readTimeout = readTimeout;
    transport->buffer = (uint8_t *)malloc(bufferSize);
    if (transport->buffer == NULL)
    {
        free(transport);
        return NULL;
    }
    transport->bufferSize = bufferSize;
    return transport;
}

// 释放传输结构体内存
void FreeTransport(Transport *transport)
{
    if (transport == NULL)
    {
        return;
    }
    if (transport->buffer != NULL)
    {
        free(transport->buffer);
    }
    free(transport);
}

// Write 方法从串口写入数据
int Write(Transport *transport, const uint8_t *data, size_t dataLength)
{
    size_t newDataLength;
    uint8_t *newData = AddPacketEdges(&(transport->parser->edger), data, dataLength, &newDataLength);
    if (newData == NULL)
    {
        return -1;
    }

    // 假设这里是你的串口写入函数，根据实际情况修改
    // int writeResult = transport->port->Write(newData, newDataLength);
    // 这里暂时使用 printf 模拟写入
    printf("Writing data: ");
    for (size_t i = 0; i < newDataLength; i++)
    {
        printf("%02X ", newData[i]);
    }
    printf("\n");

    free(newData);

    return 0;
}

// Read 方法从串口读取数据并解析
uint8_t *Read(Transport *transport, size_t *resultLength)
{
    // 假设这里是你的串口读取函数，根据实际情况修改
    // int readResult = transport->port->Read(transport->buffer, transport->bufferSize);
    // 这里暂时使用模拟读取
    size_t n = 10;
    for (size_t i = 0; i < n; i++)
    {
        transport->buffer[i] = i;
    }

    if (n == 0)
    {
        return NULL;
    }

    uint8_t *packetData = ParseBytes(transport->parser, transport->buffer, n, resultLength);
    if (packetData == NULL)
    {
        return NULL;
    }

    return packetData;
}

// 测试代码
int TestTransport()
{
    // 假设这些是你的包头、包尾和解析器
    PacketEdger edger = {{0xAB, 0xCD}, {0xEF, 0x01}};
    GenericByteParser *parser = NewGenericByteParser(SimpleDataChecker, edger);

    Transport *transport = InitTransport(NULL, parser, NULL, 100, 100, 100);

    uint8_t data[] = {0x01, 0x02, 0x03};
    size_t dataLength = sizeof(data);

    if (Write(transport, data, dataLength) == 0)
    {
        printf("Write successful.\n");
    }
    else
    {
        printf("Write failed.\n");
    }

    size_t readLength;
    uint8_t *readData = Read(transport, &readLength);
    if (readData != NULL)
    {
        printf("Read successful. Data: ");
        for (size_t i = 0; i < readLength; i++)
        {
            printf("%02X ", readData[i]);
        }
        printf("\n");
        free(readData);
    }
    else
    {
        printf("Read failed.\n");
    }

    FreeTransport(transport);

    return 0;
}