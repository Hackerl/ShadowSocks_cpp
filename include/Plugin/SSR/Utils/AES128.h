//
// Created by hackerl on 10/21/18.
//

#ifndef SSRPLUGIN_AES128_H
#define SSRPLUGIN_AES128_H

#include <openssl/aes.h>
#include <iostream>
#include "Encryptor.h"

class CAES128 : public CEncryptor
{
public:
    CAES128()
    {
        m_KeyLen = 16;
        m_IVLen = 16;
    }

public:
    void CBCEncrypt(u_char * encrypt, u_char * out_data, size_t length)
    {
        u_char iv[16] = { };

        AES_KEY aes;
        AES_set_encrypt_key(m_CipherKey.data(), 128, &aes);
        AES_cbc_encrypt((const u_char *)encrypt, out_data, length, &aes, iv, AES_ENCRYPT);
    }
};

#endif //SSRPLUGIN_AES128_H
