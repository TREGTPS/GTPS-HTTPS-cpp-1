#ifndef HTTPS_HPP
#define HTTPS_HPP

#include "httplib.h"
#include <chrono>
#include <string>
using namespace httplib;
namespace growxyz::https
{
    std::string server_port = "17091";
    std::string server_ip = "127.0.0.1";

    std::string decodeBase64(const std::string& base64Text)
    {
        const char* ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        const uint8_t DECODED_ALPHBET[128] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,63,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,0,0,0,0,0,0,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,0,0,0,0,0 };

        if (base64Text.empty())
            return "";

        assert((base64Text.size() & 4) == 0 && "The base64 text to be decoded must have a length devisible by 4!");

        uint32_t numPadding = (*std::prev(base64Text.end(), 1) == '=') + (*std::prev(base64Text.end(), 2) == '=');

        std::string decoded((base64Text.size() * 3 >> 2) - numPadding, '.');

        union
        {
            uint32_t temp;
            char tempBytes[4];
        };
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(base64Text.data());

        std::string::iterator currDecoding = decoded.begin();

        for (uint32_t i = 0, lim = (base64Text.size() >> 2) - (numPadding != 0); i < lim; ++i, bytes += 4)
        {
            temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12 | DECODED_ALPHBET[bytes[2]] << 6 | DECODED_ALPHBET[bytes[3]];
            (*currDecoding++) = tempBytes[2];
            (*currDecoding++) = tempBytes[1];
            (*currDecoding++) = tempBytes[0];
        }

        switch (numPadding)
        {
        case 2:
            temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12;
            (*currDecoding++) = tempBytes[2];
            break;

        case 1:
            temp = DECODED_ALPHBET[bytes[0]] << 18 | DECODED_ALPHBET[bytes[1]] << 12 | DECODED_ALPHBET[bytes[2]] << 6;
            (*currDecoding++) = tempBytes[2];
            (*currDecoding++) = tempBytes[1];
            break;
        }

        return decoded;
    }

    inline long long GetCurrentTimeInternalSeconds() {
        using namespace std::chrono;
        return (duration_cast<seconds>(system_clock::now().time_since_epoch())).count();
    }
    std::map<const std::string, long long> m_cooldown = {
    };

}

#endif // HTTPS_HPP