//
// Created by hackerl on 10/21/18.
//

#ifndef SSRPLUGIN_HMAC_H
#define SSRPLUGIN_HMAC_H

#include <cstring>
#include <openssl/hmac.h>

#define HMAC_HASH_BYTES 16

class CHMAC
{
public:
    static void Md5Hash(u_char * Hash, u_char * Data, size_t DataLen, u_char * Key, size_t KeyLen)
    {
        u_char TMP[HMAC_HASH_BYTES] = { };

        HMAC(EVP_md5(), Key, int(KeyLen), Data, int(DataLen), TMP, nullptr);

        memcpy(Hash, TMP, HMAC_HASH_BYTES);
    }

    static void Sha1Hash(u_char * Hash, u_char * Data, size_t DataLen, u_char * Key, size_t KeyLen)
    {
        u_char TMP[HMAC_HASH_BYTES] = { };

        HMAC(EVP_sha1(), Key, int(KeyLen), Data, int(DataLen), TMP, nullptr);

        memcpy(Hash, TMP, HMAC_HASH_BYTES);
    }
};

#endif //SSRPLUGIN_HMAC_H
