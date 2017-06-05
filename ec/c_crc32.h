/*!
\file c_crc32.h
\brief crc32

ec library is free C++ library.

\author	 kipway@outlook.com
*/
#ifndef C_CRC32_H
#define C_CRC32_H

namespace ec
{
    /*
    * CRC-32-ANSI
    * Poly                       : 1EDC6F41
    * Initialization             : 0xffffffff
    * Reflect Input byte         : true
    * Reflect Output CRC         : true
    * Xor constant to output CRC : 0xffffffff
    * Output for "123456789"     : CBF43926
    */
    static unsigned int crc32_table[256] = {
        0X00000000,0X77073096,0XEE0E612C,0X990951BA,0X076DC419,0X706AF48F,0XE963A535,0X9E6495A3,
        0X0EDB8832,0X79DCB8A4,0XE0D5E91E,0X97D2D988,0X09B64C2B,0X7EB17CBD,0XE7B82D07,0X90BF1D91,
        0X1DB71064,0X6AB020F2,0XF3B97148,0X84BE41DE,0X1ADAD47D,0X6DDDE4EB,0XF4D4B551,0X83D385C7,
        0X136C9856,0X646BA8C0,0XFD62F97A,0X8A65C9EC,0X14015C4F,0X63066CD9,0XFA0F3D63,0X8D080DF5,
        0X3B6E20C8,0X4C69105E,0XD56041E4,0XA2677172,0X3C03E4D1,0X4B04D447,0XD20D85FD,0XA50AB56B,
        0X35B5A8FA,0X42B2986C,0XDBBBC9D6,0XACBCF940,0X32D86CE3,0X45DF5C75,0XDCD60DCF,0XABD13D59,
        0X26D930AC,0X51DE003A,0XC8D75180,0XBFD06116,0X21B4F4B5,0X56B3C423,0XCFBA9599,0XB8BDA50F,
        0X2802B89E,0X5F058808,0XC60CD9B2,0XB10BE924,0X2F6F7C87,0X58684C11,0XC1611DAB,0XB6662D3D,
        0X76DC4190,0X01DB7106,0X98D220BC,0XEFD5102A,0X71B18589,0X06B6B51F,0X9FBFE4A5,0XE8B8D433,
        0X7807C9A2,0X0F00F934,0X9609A88E,0XE10E9818,0X7F6A0DBB,0X086D3D2D,0X91646C97,0XE6635C01,
        0X6B6B51F4,0X1C6C6162,0X856530D8,0XF262004E,0X6C0695ED,0X1B01A57B,0X8208F4C1,0XF50FC457,
        0X65B0D9C6,0X12B7E950,0X8BBEB8EA,0XFCB9887C,0X62DD1DDF,0X15DA2D49,0X8CD37CF3,0XFBD44C65,
        0X4DB26158,0X3AB551CE,0XA3BC0074,0XD4BB30E2,0X4ADFA541,0X3DD895D7,0XA4D1C46D,0XD3D6F4FB,
        0X4369E96A,0X346ED9FC,0XAD678846,0XDA60B8D0,0X44042D73,0X33031DE5,0XAA0A4C5F,0XDD0D7CC9,
        0X5005713C,0X270241AA,0XBE0B1010,0XC90C2086,0X5768B525,0X206F85B3,0XB966D409,0XCE61E49F,
        0X5EDEF90E,0X29D9C998,0XB0D09822,0XC7D7A8B4,0X59B33D17,0X2EB40D81,0XB7BD5C3B,0XC0BA6CAD,
        0XEDB88320,0X9ABFB3B6,0X03B6E20C,0X74B1D29A,0XEAD54739,0X9DD277AF,0X04DB2615,0X73DC1683,
        0XE3630B12,0X94643B84,0X0D6D6A3E,0X7A6A5AA8,0XE40ECF0B,0X9309FF9D,0X0A00AE27,0X7D079EB1,
        0XF00F9344,0X8708A3D2,0X1E01F268,0X6906C2FE,0XF762575D,0X806567CB,0X196C3671,0X6E6B06E7,
        0XFED41B76,0X89D32BE0,0X10DA7A5A,0X67DD4ACC,0XF9B9DF6F,0X8EBEEFF9,0X17B7BE43,0X60B08ED5,
        0XD6D6A3E8,0XA1D1937E,0X38D8C2C4,0X4FDFF252,0XD1BB67F1,0XA6BC5767,0X3FB506DD,0X48B2364B,
        0XD80D2BDA,0XAF0A1B4C,0X36034AF6,0X41047A60,0XDF60EFC3,0XA867DF55,0X316E8EEF,0X4669BE79,
        0XCB61B38C,0XBC66831A,0X256FD2A0,0X5268E236,0XCC0C7795,0XBB0B4703,0X220216B9,0X5505262F,
        0XC5BA3BBE,0XB2BD0B28,0X2BB45A92,0X5CB36A04,0XC2D7FFA7,0XB5D0CF31,0X2CD99E8B,0X5BDEAE1D,
        0X9B64C2B0,0XEC63F226,0X756AA39C,0X026D930A,0X9C0906A9,0XEB0E363F,0X72076785,0X05005713,
        0X95BF4A82,0XE2B87A14,0X7BB12BAE,0X0CB61B38,0X92D28E9B,0XE5D5BE0D,0X7CDCEFB7,0X0BDBDF21,
        0X86D3D2D4,0XF1D4E242,0X68DDB3F8,0X1FDA836E,0X81BE16CD,0XF6B9265B,0X6FB077E1,0X18B74777,
        0X88085AE6,0XFF0F6A70,0X66063BCA,0X11010B5C,0X8F659EFF,0XF862AE69,0X616BFFD3,0X166CCF45,
        0XA00AE278,0XD70DD2EE,0X4E048354,0X3903B3C2,0XA7672661,0XD06016F7,0X4969474D,0X3E6E77DB,
        0XAED16A4A,0XD9D65ADC,0X40DF0B66,0X37D83BF0,0XA9BCAE53,0XDEBB9EC5,0X47B2CF7F,0X30B5FFE9,
        0XBDBDF21C,0XCABAC28A,0X53B39330,0X24B4A3A6,0XBAD03605,0XCDD70693,0X54DE5729,0X23D967BF,
        0XB3667A2E,0XC4614AB8,0X5D681B02,0X2A6F2B94,0XB40BBE37,0XC30C8EA1,0X5A05DF1B,0X2D02EF8D
    };    

    inline unsigned int crc32(const void *pData, unsigned int dwSize)
    {
        register unsigned int	crc = 0xffffffff;
        register unsigned int	len;
        register const unsigned char	*buffer = (const unsigned char*)pData;

        len = dwSize;
        while (len--)
            crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];
        return crc ^ 0xffffffff;
    };

    /*
    * name                       :CRC-16-ANSI (aka CRC-16-IBM, CRC-16/ARC)
    * Width                      : 16 bit
    * Poly                       : 8005 (x^16 + x^15 + x^2 + 1)
    * Initialization             : 0000
    * Reflect Input byte         : true
    * Reflect Output CRC         : true
    * Xor constant to output CRC : 0000
    * Output for "123456789"     : BB3D
    */
    static unsigned short crc16_tab_ansi[256] = {
        0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
        0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
        0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
        0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
        0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
        0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
        0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
        0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
        0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
        0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
        0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
        0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
        0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
        0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
        0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
        0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
        0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
        0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
        0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
        0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
        0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
        0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
        0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
        0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
        0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
        0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
        0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
        0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
        0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
        0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
        0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
        0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040
    };

    unsigned short crc16_ansi(const void *buf, int size)
    {
        unsigned short crc = 0;
        int n = size;
        const unsigned char *p = (unsigned char *)buf;
        while (n--)
            crc = crc16_tab_ansi[(crc ^ (*p++)) & 0xFF] ^ (crc >> 8);
        return crc;
    }

    /*
    * Name                       : "XMODEM", also known as "ZMODEM", "CRC-16/ACORN"
    * Width                      : 16 bit
    * Poly                       : 1021 (That is actually x^16 + x^12 + x^5 + 1)
    * Initialization             : 0000
    * Reflect Input byte         : False
    * Reflect Output CRC         : False
    * Xor constant to output CRC : 0000
    * Output for "123456789"     : 31C3
    */
    static const unsigned short crc16_tab_xmodem[256] = {
        0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
        0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
        0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
        0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
        0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
        0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
        0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
        0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
        0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
        0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
        0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
        0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
        0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
        0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
        0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
        0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
        0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
        0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
        0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
        0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
        0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
        0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
        0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
        0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
        0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
        0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
        0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
        0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
        0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
        0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
        0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
        0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
    };

    
    unsigned short crc16_xmodem(const void *buf, int size) {
        unsigned short crc = 0;
        int n = size;
        const unsigned char *p = (unsigned char *)buf;
        while (n--)
            crc = crc16_tab_xmodem[((crc >> 8) ^ *p++) & 0xFF] ^ (crc << 8);
        return crc;        
    }


    /*
    * Name                       : "CRC-16-modbus"
    * Width                      : 16 bit
    * Poly                       : 8005 (x^16 + x^15 + x^2 + 1)
    * Initialization             : FFFF
    * Reflect Input byte         : true
    * Reflect Output CRC         : true
    * Xor constant to output CRC : 0000
    * Output for "123456789"     : 4B37
    */
    static unsigned char auchCRCHi[] = {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40
    };

    /* ��λ�ֽڵ�CRC ֵ*/
    static unsigned char auchCRCLo[] = {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
        0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
        0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
        0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
        0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
        0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
        0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
        0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
        0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
        0x40
    };

    
    inline unsigned short crc16_modbus(const void* buf, int len)
    {
        const unsigned char *p = (unsigned char *)buf;
        unsigned char uchCRCHi = 0xFF; 
        unsigned char uchCRCLo = 0xFF;
        unsigned uIndex;
        while (len--) 
        {
            uIndex = uchCRCLo ^ *p++; 
            uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
            uchCRCHi = auchCRCLo[uIndex];
        }
        return (uchCRCHi << 8 | uchCRCLo);
    }

}; // ec
#endif // C_CRC32_H

