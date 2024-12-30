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

#include "AppFrame.h"

// 解码数据字节数组为 AppLayerFrame 结构
AppLayerFrame DecodeAppLayerFrame(const uint8_t *data, size_t data_length)
{
    AppLayerFrame frame = {0};
    if (data_length < 4)
    {
        // 数据长度过短无法解码
        return frame;
    }
    frame.Length = (data[0] << 8) | data[1];

    if (data_length < (size_t)frame.Length + 4)
    {
        // 数据长度不匹配
        return frame;
    }
    frame.Payload = (uint8_t *)malloc(frame.Length);
    if (frame.Payload == NULL)
    {
        // 内存分配失败
        return frame;
    }
    memcpy(frame.Payload, data + 2, frame.Length);

    frame.Crc16 = (data[2 + frame.Length] << 8) | data[2 + frame.Length + 1];

    return frame;
}

// 将 AppLayerFrame 编码为字节数组
uint8_t *EncodeAppLayerFrame(const AppLayerFrame *f, size_t *encoded_length)
{
    if (f->Payload == NULL || strlen((const char *)f->Payload) != (size_t)f->Length)
    {
        return NULL;
    }

    *encoded_length = 2 + f->Length + 2;
    uint8_t *data = (uint8_t *)malloc(*encoded_length);
    if (data == NULL)
    {
        // 内存分配失败
        return NULL;
    }

    data[0] = (f->Length >> 8) & 0xFF;
    data[1] = f->Length & 0xFF;

    memcpy(data + 2, f->Payload, f->Length);

    data[*encoded_length - 2] = (f->Crc16 >> 8) & 0xFF;
    data[*encoded_length - 1] = f->Crc16 & 0xFF;

    return data;
}

// 将消息编码为字节数组
uint8_t *EncodeMessage(const Message *m, size_t *encoded_length)
{
    *encoded_length = 2 + strlen((const char *)m->Data);
    uint8_t *buf = (uint8_t *)malloc(*encoded_length);
    if (buf == NULL)
    {
        // 内存分配失败
        return NULL;
    }

    buf[0] = (m->Type >> 8) & 0xFF;
    buf[1] = m->Type & 0xFF;

    memcpy(buf + 2, m->Data, strlen((const char *)m->Data));

    return buf;
}

// 将字节数组解码为消息
Message DecodeMessage(const uint8_t *data, size_t data_length)
{
    Message m = {0};
    if (data_length < 2)
    {
        return m;
    }

    m.Type = (data[0] << 8) | data[1];

    m.Data = (uint8_t *)malloc(data_length - 2);
    if (m.Data == NULL)
    {
        // 内存分配失败
        return m;
    }

    memcpy(m.Data, data + 2, data_length - 2);

    return m;
}

// 释放 AppLayerFrame 结构体中的 Payload 指针
void FreeAppLayerFrame(AppLayerFrame *frame)
{
    if (frame->Payload != NULL)
    {
        free(frame->Payload);
        frame->Payload = NULL;
    }
}

// 释放 Message 结构体中的 Data 指针
void FreeMessage(Message *m)
{
    if (m->Data != NULL)
    {
        free(m->Data);
        m->Data = NULL;
    }
}
