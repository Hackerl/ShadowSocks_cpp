//
// Created by hackerl on 10/28/18.
//

#include "HTTPSimple.h"
#include <cstring>
#include <Common/Utils/Binascii.h>

#define HTTP_LINE_SPLIT "\r\n"
#define HTTP_HEADER_END "\r\n\r\n"
#define HTTP_HEADER_END_LEN 4

CHTTPSimple::CHTTPSimple()
{
    m_HasSendHeader = false;
    m_HasRecvHeader = false;
}

std::string CHTTPSimple::GetHTTPHeader(const u_char *Buffer, size_t Length)
{
    std::string HTTPHeader;

    HTTPHeader.append("GET /");

    std::string HexData = CBinascii::Hexlify(Buffer, Length);

    for (int i = 0; i < HexData.length(); i += 2)
    {
        HTTPHeader.append("%");
        HTTPHeader.append(HexData.data() + i, 2);
    }

    HTTPHeader.append(" HTTP/1.1");
    HTTPHeader.append(HTTP_LINE_SPLIT);

    HTTPHeader.append("Host: bing.com");
    HTTPHeader.append(HTTP_LINE_SPLIT);

    HTTPHeader.append("User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:40.0) Gecko/20100101 Firefox/44.0");
    HTTPHeader.append(HTTP_LINE_SPLIT);

    HTTPHeader.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    HTTPHeader.append(HTTP_LINE_SPLIT);

    HTTPHeader.append("Accept-Language: en-US,en;q=0.8");
    HTTPHeader.append(HTTP_LINE_SPLIT);

    HTTPHeader.append("Accept-Encoding: gzip, deflate");
    HTTPHeader.append(HTTP_LINE_SPLIT);

    HTTPHeader.append("DNT: 1");
    HTTPHeader.append(HTTP_LINE_SPLIT);

    HTTPHeader.append("Connection: keep-alive");
    HTTPHeader.append(HTTP_LINE_SPLIT);

    HTTPHeader.append(HTTP_LINE_SPLIT);

    return HTTPHeader;
}

std::vector<u_char> CHTTPSimple::ClientPack(const u_char *Buffer, size_t Length)
{
    std::vector<u_char> EncodeStream;

    if (m_HasSendHeader)
    {
        EncodeStream.insert(EncodeStream.end(), Buffer, Buffer + Length);
        return EncodeStream;
    }

    m_HasSendHeader = true;

    std::string HTTPHeader = GetHTTPHeader(Buffer, Length);

    EncodeStream.insert(EncodeStream.end(), HTTPHeader.begin(), HTTPHeader.end());

    return EncodeStream;
}

std::vector<u_char> CHTTPSimple::ClientUnPack(const u_char *Buffer, size_t Length)
{
    std::vector<u_char> DecodeStream;

    if (m_HasRecvHeader)
    {
        DecodeStream.insert(DecodeStream.end(), Buffer, Buffer + Length);
        return DecodeStream;
    }

    m_HasRecvHeader = true;

    int HTTPHeaderEnd = -1;

    for(int i = 0; i < Length - HTTP_HEADER_END_LEN; i++)
    {
        if (memcmp(Buffer + i, HTTP_HEADER_END, HTTP_HEADER_END_LEN) != 0)
            continue;

        HTTPHeaderEnd = i + HTTP_HEADER_END_LEN;
        break;
    }

    if (HTTPHeaderEnd >= 0)
    {
        DecodeStream.insert(DecodeStream.end(), Buffer + HTTPHeaderEnd, Buffer + Length);
    }

    return DecodeStream;
}
