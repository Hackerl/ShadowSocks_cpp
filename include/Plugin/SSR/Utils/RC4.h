//
// Created by hackerl on 10/27/18.
//

#ifndef SSRPLUGIN_RC4_H
#define SSRPLUGIN_RC4_H

#include "Encryptor.h"
#include <sys/types.h>
#include <openssl/rc4.h>

class CRC4 : public CEncryptor
{
public:
    CRC4() : m_EncryptKey(), m_DecryptKey()
    {
        m_KeyLen = 16;
        m_IVLen = 0;
    }

public:
    void Init(u_char * PassWord, size_t PassWordLen) override
    {
        CEncryptor::Init(PassWord, PassWordLen);

        RC4_set_key(&m_EncryptKey, int(m_CipherKey.size()), m_CipherKey.data());
        RC4_set_key(&m_DecryptKey, int(m_CipherKey.size()), m_CipherKey.data());
    }

public:
    std::vector<u_char> Encrypt(const u_char *Buffer, size_t Length)
    {
        return RC4Stream(Buffer, Length, m_EncryptKey);
    }

    std::vector<u_char> Decrypt(u_char * Buffer, size_t Length)
    {
        return RC4Stream(Buffer, Length, m_DecryptKey);
    }

    std::vector<u_char> RC4Stream(const u_char *Buffer, size_t Length, RC4_KEY &RC4Key)
    {
        auto TMP = new u_char[Length]();

        RC4(&RC4Key, Length, Buffer, TMP);

        std::vector<u_char> CipherStream;

        CipherStream.insert(CipherStream.end(), TMP, TMP + Length);

        delete [] TMP;

        return CipherStream;
    }

private:
    RC4_KEY m_EncryptKey;
    RC4_KEY m_DecryptKey;
};

#endif //SSRPLUGIN_RC4_H
