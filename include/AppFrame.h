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

#ifndef APPFRAME_H
#define APPFRAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 应用层帧结构体
typedef struct
{
    uint16_t Length;
    uint8_t *Payload;
    uint16_t Crc16;
} AppLayerFrame;

// 消息结构体
typedef struct
{
    uint16_t Type;
    uint8_t *Data;
} Message;

// 解码数据字节数组为 AppLayerFrame 结构
AppLayerFrame DecodeAppLayerFrame(const uint8_t *data, size_t data_length);

// 将 AppLayerFrame 编码为字节数组
uint8_t *EncodeAppLayerFrame(const AppLayerFrame *f, size_t *encoded_length);

// 将消息编码为字节数组
uint8_t *EncodeMessage(const Message *m, size_t *encoded_length);

// 将字节数组解码为消息
Message DecodeMessage(const uint8_t *data, size_t data_length);

// 释放 AppLayerFrame 结构体中的 Payload 指针
void FreeAppLayerFrame(AppLayerFrame *frame);

// 释放 Message 结构体中的 Data 指针
void FreeMessage(Message *m);

#endif