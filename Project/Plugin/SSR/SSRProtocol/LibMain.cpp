//
// Created by hackerl on 10/21/18.
//

#include "AuthChainLocal.h"
#include <Utils/Binascii.h>
#include <Utils/Encryptor.h>
#include <Utils/RC4.h>

int main()
{
    std::string RC4Key = "hIW+9GrFkhsUo4aJnap9/A==X4vBv3sMrKjWgabazmuOEQ==";

    CRC4 RC4Utils;
    RC4Utils.Init((u_char *)RC4Key.c_str(), RC4Key.size());

    u_char RawData[2] = {0xb4, 0x05};

    std::vector<u_char> OneDataStream = RC4Utils.Encrypt(RawData, 2);

    std::vector<u_char> TestData = CBinascii::UnHexlify("485454502f312e30203430302042616420726571756573740d0a43616368652d436f6e74726f6c3a206e6f2d63616368650d0a436f6e6e656374696f6e3a20636c6f73650d0a436f6e74656e742d547970653a20746578742f68746d6c0d0a0d0a3c68746d6c3e3c626f64793e3c68313e3430302042616420726571756573743c2f68313e0a596f75722062726f777365722073656e7420616e20696e76616c696420726571756573742e0a3c2f626f64793e3c2f68746d6c3e0a");

    std::vector<u_char> DataStream = RC4Utils.Encrypt(TestData.data(), TestData.size());

    std::vector<u_char> RawDataStream  = RC4Utils.Decrypt(DataStream.data(), DataStream.size());

    CServerInfo ServerInfo;

    std::string PassWord = "Liu13870675448";

    CEncryptor _0_;
    _0_.Init((u_char *)PassWord.data(), PassWord.size());
    ServerInfo.Key = _0_.GetCipherKey();

    CAuthChainLocal AuthChainLocal;

    AuthChainLocal.SetServerInfo(ServerInfo);

    CAuthHeader AuthHeader = AuthChainLocal.PackAuthData();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}