#include "hash.h"
std::array<uint32_t, 8> gautiHash(string t)
{
    std::array<uint32_t, 8> hash = {
        0x243F6A88u,
        0x85A308D3u,
        0x13198A2Eu,
        0x03707344u,
        0xA4093822u,
        0x299F31D0u,
        0x082EFA98u,
        0xEC4E6C89u
    };

    for (size_t i = 0; i < t.size(); i++)
    {
        uint32_t baitas =
            static_cast<unsigned char>(t[i]);

        uint32_t indeksas =
            static_cast<uint32_t>(i) ^
            static_cast<uint32_t>(
                static_cast<uint64_t>(i) >> 32
            );

        size_t pozicija = i % 8;

        uint32_t maisymas =
            baitas +
            0x9E3779B9u * (indeksas + 1u);

        hash[pozicija] ^=
            maisymas +
            rotl32(hash[(pozicija + 7) % 8], 5);

        hash[pozicija] =
            rotl32(
                hash[pozicija],
                7u + static_cast<unsigned>(i % 19)
            );

        hash[(pozicija + 1) % 8] +=
            hash[pozicija] ^ 0x85EBCA6Bu;

        hash[(pozicija + 3) % 8] ^=
            rotl32(hash[pozicija] + maisymas, 11);

        hash[(pozicija + 5) % 8] +=
            rotl32(
                hash[pozicija] ^
                hash[(pozicija + 1) % 8],
                17
            );
    }

    uint64_t ilgis = t.size();

    hash[0] ^= static_cast<uint32_t>(ilgis);
    hash[1] ^= static_cast<uint32_t>(ilgis >> 32);

    // Galutinis maišymas
    for (unsigned raundas = 0; raundas < 8; raundas++)
    {
        auto ankstesnis = hash;

        for (unsigned j = 0; j < 8; j++)
        {
            uint32_t a = ankstesnis[j];
            uint32_t b = ankstesnis[(j + 1) % 8];
            uint32_t c = ankstesnis[(j + 3) % 8];
            uint32_t d = ankstesnis[(j + 6) % 8];

            uint32_t x =
                a +
                (b ^
                (0x9E3779B9u +
                 raundas * 0x7F4A7C15u +
                 j));

            x = rotl32(
                x,
                5u + ((j * 3u + raundas) % 23u)
            );

            hash[j] =
                (x ^ c) + rotl32(d, 13);
        }
    }

    return hash;
}
uint32_t rotl32(uint32_t x, unsigned r)
{
    r &= 31u;

    if (r == 0)
        return x;

    return (x << r) | (x >> (32 - r));
}