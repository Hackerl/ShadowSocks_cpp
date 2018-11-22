//
// Created by hackerl on 10/21/18.
//

#ifndef SSRPLUGIN_ENCRYPTOR_H
#define SSRPLUGIN_ENCRYPTOR_H

#include <vector>
#include <cstring>
#include <openssl/md5.h>
#include <openssl/evp.h>

#define EVP_BytesToKey_MD5_Len 16
#define MAX_TMP_BUFFER_LEN 128

class CEncryptor
{
public:
    CEncryptor()
    {
        m_KeyLen = 16;
        m_IVLen = 0;
    }

public:
    virtual void Init(u_char * PassWord, size_t PassWordLen)
    {
        if (PassWordLen > MAX_TMP_BUFFER_LEN - EVP_BytesToKey_MD5_Len)
            return;

        uint8_t TMPBuffer[MAX_TMP_BUFFER_LEN] = {};
        uint8_t Md5Buffer[EVP_MAX_KEY_LENGTH + EVP_MAX_IV_LENGTH] = {};

        MD5(PassWord, PassWordLen, TMPBuffer);
        memcpy(Md5Buffer, TMPBuffer, EVP_BytesToKey_MD5_Len);

        for (int i = EVP_BytesToKey_MD5_Len; i < m_KeyLen + m_IVLen; i += EVP_BytesToKey_MD5_Len)
        {
            memcpy(TMPBuffer + EVP_BytesToKey_MD5_Len, PassWord, PassWordLen);
            MD5(TMPBuffer, EVP_BytesToKey_MD5_Len + PassWordLen, TMPBuffer);
            memcpy(Md5Buffer + i, TMPBuffer, EVP_BytesToKey_MD5_Len);
        }

        m_CipherKey.insert(m_CipherKey.end(), Md5Buffer, Md5Buffer + m_KeyLen);
        m_CipherIV.insert(m_CipherIV.end(), Md5Buffer + m_KeyLen, Md5Buffer + m_KeyLen + m_IVLen);
    }

public:
    std::vector<u_char> GetCipherIV()
    {
        return m_CipherIV;
    }

    std::vector<u_char> GetCipherKey()
    {
        return m_CipherKey;
    }

protected:
    size_t m_KeyLen;
    size_t m_IVLen;
    std::vector<u_char> m_CipherIV;
    std::vector<u_char> m_CipherKey;
};


#endif //SSRPLUGIN_ENCRYPTOR_H
