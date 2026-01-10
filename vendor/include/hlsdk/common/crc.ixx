export module crc;

export struct MD5Context_t
{
	unsigned int buf[4];
	unsigned int bits[2];
	unsigned char in[64];
};

export using CRC32_t = unsigned long;
export void CRC32_Init(CRC32_t *pulCRC);
export CRC32_t CRC32_Final(CRC32_t pulCRC);
export void CRC32_ProcessBuffer(CRC32_t *pulCRC, void *p, int len);
export void CRC32_ProcessByte(CRC32_t *pulCRC, unsigned char ch);
export int CRC_File(CRC32_t *crcvalue, char *pszFileName);

export unsigned char COM_BlockSequenceCRCByte(unsigned char *base, int length, int sequence);

export void MD5Init(MD5Context_t *context);
export void MD5Update(MD5Context_t *context, unsigned char const *buf, unsigned int len);
export void MD5Final(unsigned char digest[16], MD5Context_t *context);
export void Transform(unsigned int buf[4], unsigned int const in[16]);

export int MD5_Hash_File(unsigned char digest[16], char *pszFileName, int bUsefopen, int bSeed, unsigned int seed[4]);
export char *MD5_Print(unsigned char hash[16]);
export int MD5_Hash_CachedFile(unsigned char digest[16], unsigned char *pCache, int nFileSize, int bSeed, unsigned int seed[4]);

export int CRC_MapFile(CRC32_t *crcvalue, char *pszFileName);
