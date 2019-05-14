#include "ncode_base64.h"
#include "bytestream.h"
#include "lib_rsaref.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <openssl/bn.h>

/*
    [ ���α׷� ��ǥ ]
     1. ���Ͽ� ����� �� �Ҽ��� �� pq=N(modulus), ���Ű : d(private exponent), ����Ű : e(public exponent) �б�.
     2. RSA ����ü�� ���� ������ ä���ְ� ��/��ȣȭ �����ϱ�.
*/
/*
    [ �ΰ����� : RSA ��� ]
     1. �� �Ҽ� p, q�� �غ�
     2. p-1, q-1�� ���� ���μ��� ���� e�� �غ�
     3. e*d�� (p-1)(q-1)�� ���� �������� 1�� �ǵ��� �ϴ� d�� ã��
     4. N=pq�� ����� �� N�� e�� ���� (����Ű), d�� ����Ű�� ���
     5. p, q, (p-1), (q-1)�� ���Ȼ� ������ �ǹǷ� ����

    [ ��ȣȭ�� ���� �߰� ���� ]
     1. dP = d mod (p-1)
     2. dQ = d mod (q-1)
     3. qInv = q^-1 mod p
     -> ���귮�� ���̱� ���� CRT(�߱����� ������ ����) �� ����Ϸ��� �� ���� �߰��� �ʿ�.
        rasref ���̺귯���� CRT�� ����ϱ� ������ ���Ͽ� �����ص� �� ���� �ҷ��ͼ� ����ؾ� �Ѵ�.

    [ PKCS1 RFC3477 ���� ���� ]
     - https://tools.ietf.org/html/rfc3447#section-7.2
    
    [ ��ȣȭ ��Ű�� ]
     1) RSAES-PKCS-V1.5 : ������ ȣȯ���� ���� ����ϰ�, �ű� �������δ� �������� ����.
        - ���� �Ϻΰ� ����ǰų�, ���� ����� ������� ��ȣ �ص� ���ݿ� �������
        - �ּ� 8byte �̻��� ������ ����(0x00����)�� ���(PS)�� ����־ �⺻���� ���ȿ� Ȱ��
        - 00 || 02 || PS || 00 || M(��)
         |-----11~255byte-----|
     
     2) RSAES-OAEP      : �ű� ���뿡 ����.
        - (RSAES-OAEP = RSAEP + RSADP + EME-OAEP) == (IEEE-P1363 = IFEP-RSA + IFDP-RSA + EME-OAEP)
        - �ִ� �޽��� ���� : (k - 2 - 2hLen)Byte
          + k    : length in octets of the modulus (2048bit -> 256byte)
          + hLen : output length in octets of hash function output for EME-OAEP
        - ������ ���ؼ��� RSAES-PKCS-V1.5�� ȥ���Ͽ� ����ϸ� �ȵ�
        - EME-OAEP : �ؽ��Լ�(����:MD2/5,SHA-1)�� ����ŷ�����Լ�(����:MGF1)�� ����Ͽ� �޽��� ���� ���
        - maskedSeed(seed xor seedMask) || maskedDB(DB xor dbMask)
          seedMask: MGF(maskedDB, hLen), dbmask = MGF(seed, emLen-hLen)
          seed: hLen����Ʈ�� ������ ����, DB = pHash(�ؽ��Լ�(���ڵ� �Ķ����P)) || PS || 01 || M(��)
          PS: Generate an octet string PS consisting of emLen-||M||-2hLen-1 zero. octets. The length of PS may be 0.

    [ Ű ���� ]
     1) DER
     2) PEM
     3) X509
*/

#define RSA_BIT 2048
#define SZ_BUF_MAX 1024

static void PrintStream(char *pStream, int szStream)
{
    int szBuf = szStream * 8 + 1;
    char aOutBuf[szBuf];
    int i;

    for (i = 0; i < szStream; ++i)
    {
        char c = pStream[i];
        int offset = i * 8;

        aOutBuf[offset    ] = ((c & 0x80) >> 7 == 0 ? '0' : '1'); // 1000 0000
        aOutBuf[offset + 1] = ((c & 0x40) >> 6 == 0 ? '0' : '1'); // 0100 0000
        aOutBuf[offset + 2] = ((c & 0x20) >> 5 == 0 ? '0' : '1'); // 0010 0000
        aOutBuf[offset + 3] = ((c & 0x10) >> 4 == 0 ? '0' : '1'); // 0001 0000
        aOutBuf[offset + 4] = ((c & 0x08) >> 3 == 0 ? '0' : '1'); // 0000 1000
        aOutBuf[offset + 5] = ((c & 0x04) >> 2 == 0 ? '0' : '1'); // 0000 0100
        aOutBuf[offset + 6] = ((c & 0x02) >> 1 == 0 ? '0' : '1'); // 0000 0010
        aOutBuf[offset + 7] = ((c & 0x01)      == 0 ? '0' : '1'); // 0000 0001
    }

    aOutBuf[szBuf - 1] = '\0';

    fprintf(stdout, "%s\n", aOutBuf);
}

static void GenRandom(char *pBuf, int szBuf, unsigned char min, unsigned char max)
{
    int i;

    if (min == max)
    {
        memset(pBuf, min, szBuf);
        return;
    }

    if (min > max)
    {
        unsigned char temp = min;
        min = max;
        max = temp;
    }

    for (i = 0; i < szBuf; ++i)
    {
        pBuf[i] = (unsigned char)(rand() % (max - min))  + min;
    }
}

int GetExpModPrimeFromFile(char *pExpFileName, char *pModFileName, char *pPrimeFileName,
                           char *pExpBuf_E, char *pExpBuf_D, char *pModBuf, char *pBuf_P, char *pBuf_Q,
                           char *pBuf_DP, char *pBuf_DQ, char *pBuf_InvQ)
{
    FILE    *pExpFile = NULL, *pModFile = NULL, *pPrimeFile = NULL;
    char    arLineBuf[SZ_BUF_MAX];
    int     nLineCnt, i;

    if (pExpFileName == NULL || pModFileName == NULL || pPrimeFileName == NULL)
    {
        fprintf(stdout, "[ERR] One of 'pExpFileName', 'pModFileName', 'pPrimeFileName' filename is NULL!\n");
        return -1;
    }

    // Open Exp File
    if ((pExpFile = fopen(pExpFileName, "r")) == NULL)
    {
        fprintf(stdout, "[ERR] Cannot open file '%s'.\n", pExpFileName);
        return -1;
    }

    // Open Mod File
    if ((pModFile = fopen(pModFileName, "r")) == NULL)
    {
        fprintf(stdout, "[ERR] Cannot open file '%s'.\n", pModFileName);
        return -1;
    }

    // Open Prime File
    if ((pPrimeFile = fopen(pPrimeFileName, "r")) == NULL)
    {
        fprintf(stdout, "[ERR] Cannot open file '%s'.\n", pPrimeFileName);
        return -1;
    }

    // Read Exp File
    nLineCnt = 0;
    memset(arLineBuf, 0x00, SZ_BUF_MAX);
    while (fgets(arLineBuf, SZ_BUF_MAX, pExpFile) != NULL)
    {
        nLineCnt == 0 ? strcpy(pExpBuf_E, arLineBuf) : strcpy(pExpBuf_D, arLineBuf);
        
        if ((++nLineCnt) == 2) break;
        
        memset(arLineBuf, 0x00, SZ_BUF_MAX);
    }

    // Read Mod File
    memset(arLineBuf, 0x00, SZ_BUF_MAX);
    while (fgets(arLineBuf, SZ_BUF_MAX, pModFile) != NULL)
    {
        strcpy(pModBuf, arLineBuf);
        break;
    }

    // Read Prime File
    nLineCnt = 0;
    memset(arLineBuf, 0x00, SZ_BUF_MAX);
    while (fgets(arLineBuf, SZ_BUF_MAX, pPrimeFile) != NULL)
    {
        if (nLineCnt == 0)      { strcpy(pBuf_P,    arLineBuf);        }
        else if (nLineCnt == 1) { strcpy(pBuf_Q,    arLineBuf);        }
        else if (nLineCnt == 2) { strcpy(pBuf_DP,   arLineBuf);        }
        else if (nLineCnt == 3) { strcpy(pBuf_DQ,   arLineBuf);        }
        else if (nLineCnt == 4) { strcpy(pBuf_InvQ, arLineBuf); break; }
        
        ++nLineCnt;
        memset(arLineBuf, 0x00, SZ_BUF_MAX);
    }

    // Remove '\n' character
    for (i = strlen(pExpBuf_E) - 1; i > -1; --i) // 'e'
    {
        pExpBuf_E[i] == '\n' ? pExpBuf_E[i] = '\0' : 0;
    }

    for (i = strlen(pExpBuf_D) - 1; i > -1; --i) // 'd'
    {
        pExpBuf_D[i] == '\n' ? pExpBuf_D[i] = '\0' : 0;
    }

    for (i = strlen(pModBuf) - 1; i > -1; --i) // 'N'
    {
        pModBuf[i] == '\n' ? pModBuf[i] = '\0' : 0;
    }

    for (i = strlen(pBuf_P) - 1; i > -1; --i) // 'p'
    {
        pBuf_P[i] == '\n' ? pBuf_P[i] = '\0' : 0;
    }

    for (i = strlen(pBuf_Q) - 1; i > -1; --i) // 'q'
    {
        pBuf_Q[i] == '\n' ? pBuf_Q[i] = '\0' : 0;
    }

    for (i = strlen(pBuf_DP) - 1; i > -1; --i) // 'dp'
    {
        pBuf_DP[i] == '\n' ? pBuf_DP[i] = '\0' : 0;
    }

    for (i = strlen(pBuf_DQ) - 1; i > -1; --i) // 'dq'
    {
        pBuf_DQ[i] == '\n' ? pBuf_DQ[i] = '\0' : 0;
    }

    for (i = strlen(pBuf_InvQ) - 1; i > -1; --i) // 'Inv-q'
    {
        pBuf_InvQ[i] == '\n' ? pBuf_InvQ[i] = '\0' : 0;
    }

    fclose(pExpFile);
    fclose(pModFile);
    fclose(pPrimeFile);

    return 0;
}

int EnDecryptRSA(int isEncryption, char *pInStr, char *pOutStr, char *pE, char *pD, char *pN,
                 char *pP, char *pQ, char *pDP, char *pDQ, char *pInvQ)
{
    char            arInBuf[SZ_BUF_MAX], arOutBuf[SZ_BUF_MAX];
    unsigned char   arExp_E[MAX_RSA_MODULUS_LEN], arExp_D[MAX_RSA_MODULUS_LEN], arMod[MAX_RSA_MODULUS_LEN];
    unsigned char   ar_P[MAX_RSA_PRIME_LEN], ar_Q[MAX_RSA_PRIME_LEN];
    unsigned char   ar_DP[MAX_RSA_PRIME_LEN], ar_DQ[MAX_RSA_PRIME_LEN], ar_InvQ[MAX_RSA_PRIME_LEN];
    int             szInput = -1, rtLen = -1;

    memset(arInBuf, 0x00, SZ_BUF_MAX);
    memset(arOutBuf, 0x00, SZ_BUF_MAX);
    memset(arExp_E, 0x00, MAX_RSA_MODULUS_LEN);
    memset(arExp_D, 0x00, MAX_RSA_MODULUS_LEN);
    memset(arMod, 0x00, MAX_RSA_MODULUS_LEN);
    memset(ar_P, 0x00, MAX_RSA_PRIME_LEN);
    memset(ar_Q, 0x00, MAX_RSA_PRIME_LEN);
    memset(ar_DP, 0x00, MAX_RSA_PRIME_LEN);
    memset(ar_DQ, 0x00, MAX_RSA_PRIME_LEN);
    memset(ar_InvQ, 0x00, MAX_RSA_PRIME_LEN);

    strcpy(arInBuf, pInStr);

    // String to BIGNUM, BIGNUM to binary
    {
        BIGNUM *pBN = NULL;
        int szVal = 0;

        // Exponent 'e' (Public key)
        if (pE != NULL && isEncryption)
        {
            if (!BN_dec2bn(&pBN, pE) || !(rtLen = BN_bn2bin(pBN, arExp_E)))
            {   
                fprintf(stdout, "[ERR] BN_dec2bn/BN_bn2bin(pE) error.\n");
                if (pBN != NULL) BN_free(pBN);
                goto END;
            }

            // fprintf(stdout, "E(Hex):\n%s\n", BN_bn2hex(pBN)); // test

            if ((szVal = MAX_RSA_MODULUS_LEN - rtLen) > 0)
            {
                memcpy(&arExp_E[szVal], arExp_E, rtLen);
                memset(arExp_E, 0x00, szVal);
            }

            BN_free(pBN);   pBN = NULL;

            // fprintf(stdout, "\nE(Bin):\n"); PrintStream(arExp_E, MAX_RSA_MODULUS_LEN); fprintf(stdout, "\n"); // test
        }
        // Exponent 'd' (Private key)
        if (pD != NULL && !isEncryption)
        {
            if (!BN_dec2bn(&pBN, pD) || !(rtLen = BN_bn2bin(pBN, arExp_D)))
            {
                fprintf(stdout, "[ERR] BN_dec2bn/BN_bn2bin(pD) error.\n");
                if (pBN != NULL) BN_free(pBN);
                goto END;
            }

            if ((szVal = MAX_RSA_MODULUS_LEN - rtLen) > 0)
            {
                memcpy(&arExp_D[szVal], arExp_D, rtLen);
                memset(arExp_D, 0x00, szVal);
            }

            BN_free(pBN);   pBN = NULL;
        }
        // Modulus 'N' (p * q)
        if (pN != NULL)
        {
            if (!BN_dec2bn(&pBN, pN) || !(rtLen = BN_bn2bin(pBN, arMod)))
            {
                fprintf(stdout, "[ERR] BN_dec2bn/BN_bn2bin(pN) error.\n");
                if (pBN != NULL) BN_free(pBN);
                goto END;
            }

            if ((szVal = MAX_RSA_MODULUS_LEN - rtLen) > 0)
            {
                memcpy(&arMod[szVal], arMod, rtLen);
                memset(arMod, 0x00, szVal);
            }

            BN_free(pBN);   pBN = NULL;
        }
        // Prime number 'P'
        if (pP != NULL && !isEncryption)
        {
            if (!BN_dec2bn(&pBN, pP) || !(rtLen = BN_bn2bin(pBN, ar_P)))
            {
                fprintf(stdout, "[ERR] BN_dec2bn/BN_bn2bin(pP) error.\n");
                if (pBN != NULL) BN_free(pBN);
                goto END;
            }

            if ((szVal = MAX_RSA_PRIME_LEN - rtLen) > 0)
            {
                memcpy(&ar_P[szVal], ar_P, rtLen);
                memset(ar_P, 0x00, szVal);
            }

            BN_free(pBN);   pBN = NULL;
        }
        // Prime number 'Q'
        if (pQ != NULL && !isEncryption)
        {
            if (!BN_dec2bn(&pBN, pQ) || !(rtLen = BN_bn2bin(pBN, ar_Q)))
            {
                fprintf(stdout, "[ERR] BN_dec2bn/BN_bn2bin(pQ) error.\n");
                if (pBN != NULL) BN_free(pBN);
                goto END;
            }

            if ((szVal = MAX_RSA_PRIME_LEN - rtLen) > 0)
            {
                memcpy(&ar_Q[szVal], ar_Q, rtLen);
                memset(ar_Q, 0x00, szVal);
            }

            BN_free(pBN);   pBN = NULL;
        }
        // Prime 'DP'
        if (pDP != NULL && !isEncryption)
        {
            if (!BN_dec2bn(&pBN, pDP) || !(rtLen = BN_bn2bin(pBN, ar_DP)))
            {
                fprintf(stdout, "[ERR] BN_dec2bn/BN_bn2bin(pDP) error.\n");
                if (pBN != NULL) BN_free(pBN);
                goto END;
            }

            if ((szVal = MAX_RSA_PRIME_LEN - rtLen) > 0)
            {
                memcpy(&ar_DP[szVal], ar_DP, rtLen);
                memset(ar_DP, 0x00, szVal);
            }

            BN_free(pBN);   pBN = NULL;
        }
        // Prime 'DQ'
        if (pDQ != NULL && !isEncryption)
        {
            if (!BN_dec2bn(&pBN, pDQ) || !(rtLen = BN_bn2bin(pBN, ar_DQ)))
            {
                fprintf(stdout, "[ERR] BN_dec2bn/BN_bn2bin(pDQ) error.\n");
                if (pBN != NULL) BN_free(pBN);
                goto END;
            }

            if ((szVal = MAX_RSA_PRIME_LEN - rtLen) > 0)
            {
                memcpy(&ar_DQ[szVal], ar_DQ, rtLen);
                memset(ar_DQ, 0x00, szVal);
            }

            BN_free(pBN);   pBN = NULL;
        }
        // Inverse prime 'InvQ'
        if (pInvQ != NULL && !isEncryption)
        {
            if (!BN_dec2bn(&pBN, pInvQ) || !(rtLen = BN_bn2bin(pBN, ar_InvQ)))
            {
                fprintf(stdout, "[ERR] BN_dec2bn/BN_bn2bin(pInvQ) error.\n");
                if (pBN != NULL) BN_free(pBN);
                goto END;
            }

            if ((szVal = MAX_RSA_PRIME_LEN - rtLen) > 0)
            {
                memcpy(&ar_InvQ[szVal], ar_InvQ, rtLen);
                memset(ar_InvQ, 0x00, szVal);
            }

            BN_free(pBN);   pBN = NULL;
        }
    }

    // Base64 ��ȣ���� ���̳ʸ� ��ȣ������ ��ȯ
    if (!isEncryption)
    {
        char                    arBase64Buf[SZ_BUF_MAX];
        int                     szB64;
        NCODE_TYPE_BASE64_ARGS  stArg;

        stArg.isEncode = 0;
        stArg.pbInStream = arInBuf;
        stArg.szInStream = strlen(arInBuf);
        stArg.pbOutBuf = arBase64Buf;
        stArg.szOutBuf = SZ_BUF_MAX;

        szB64 = Decode_Base64(&stArg);
        memset(arInBuf, 0x00, SZ_BUF_MAX);
        memcpy(arInBuf, arBase64Buf, szB64);
        szInput = szB64;
    }

    // �Ϻ�ȣȭ ����
    if (isEncryption)
    {
        R_RSA_PUBLIC_KEY stPubKey;
        unsigned char    arRandom[MAX_RSA_MODULUS_LEN];
        int              szLen = -1;
        
        memset(&stPubKey, 0x00, sizeof(R_RSA_PUBLIC_KEY));
        stPubKey.bits = RSA_BIT;
        memcpy(stPubKey.modulus, arMod, MAX_RSA_MODULUS_LEN);
        memcpy(stPubKey.exponent, arExp_E, MAX_RSA_MODULUS_LEN);
        GenRandom(arRandom, MAX_RSA_MODULUS_LEN, 0x01, 0xff);
        szInput = strlen(arInBuf);

        if ((rtLen = RSAPublicEncrypt(arOutBuf, &szLen, arInBuf, szInput, &stPubKey, arRandom)) != 0)
        {
            fprintf(stdout, "[ERR] RSAPublicEncrypt() failed. (Status:%d)\n", rtLen);
            goto END;
        }

        rtLen = szLen;
    }
    else
    {
        R_RSA_PRIVATE_KEY   stPriKey;
        int                 szLen = -1;

        memset(&stPriKey, 0x00, sizeof(R_RSA_PRIVATE_KEY));
        stPriKey.bits = RSA_BIT;
        memcpy(stPriKey.modulus, arMod, MAX_RSA_MODULUS_LEN);
        memcpy(stPriKey.prime[0], ar_P, MAX_RSA_PRIME_LEN);
        memcpy(stPriKey.prime[1], ar_Q, MAX_RSA_PRIME_LEN);
        memcpy(stPriKey.primeExponent[0], ar_DP, MAX_RSA_PRIME_LEN);
        memcpy(stPriKey.primeExponent[1], ar_DQ, MAX_RSA_PRIME_LEN);
        memcpy(stPriKey.coefficient, ar_InvQ, MAX_RSA_PRIME_LEN);

        if ((rtLen = RSAPrivateDecrypt(arOutBuf, &szLen, arInBuf, szInput, &stPriKey)) != 0)
        {
            fprintf(stdout, "[ERR] RSAPrivateDecrypt() failed. (Status:%d)\n", rtLen);
            goto END;
        }

        rtLen = szLen;
    }

    // ���̳ʸ� ��ȣ���� Base64 ��ȣ������ ��ȯ
    if (isEncryption)
    {
        char                    arBase64Buf[SZ_BUF_MAX];
        int                     szB64;
        NCODE_TYPE_BASE64_ARGS  stArg;

        stArg.isEncode = 1;
        stArg.pbInStream = arOutBuf;
        stArg.szInStream = rtLen;
        stArg.pbOutBuf = arBase64Buf;
        stArg.szOutBuf = SZ_BUF_MAX;

        szB64 = Encode_Base64(&stArg);
        memset(arOutBuf, 0x00, SZ_BUF_MAX);
        memcpy(arOutBuf, arBase64Buf, szB64);
        rtLen = szB64;
    }

    // �ƿ�ǲ �ּҷ� ����� ����
    strcpy(pOutStr, arOutBuf);

END:
    return rtLen;
}

void PrintUsage()
{
    fprintf(stdout, "modexp_rsa.exe [-Op] [Str]\n"                   );
    fprintf(stdout, "1. [-Op]\n"                                     );
    fprintf(stdout, " 1) -e : Encryption\n"                          );
    fprintf(stdout, " 2) -d : Decryption\n"                          );
    fprintf(stdout, " 3) -k : Generate mod/exp from modexp.exe\n"    );
    fprintf(stdout, "2. [-Str] : Input string length under 245byte\n");
}

int main(int argc, char **argv)
{
    char arExpBuf_E[SZ_BUF_MAX], arExpBuf_D[SZ_BUF_MAX], arModBuf[SZ_BUF_MAX];
    char arBuf_P[SZ_BUF_MAX], arBuf_Q[SZ_BUF_MAX], arBuf_DP[SZ_BUF_MAX], arBuf_DQ[SZ_BUF_MAX], arBuf_InvQ[SZ_BUF_MAX];
    char arPlainBuf[SZ_BUF_MAX], arCipherBuf[SZ_BUF_MAX];
    char *pOption = NULL;

    if (argc > 1)
    {
        pOption = argv[1];
    }
    else
    {
        PrintUsage();
        return -1;
    }

    if (argc == 2 && !strcmp(argv[1], "-k"))
    {
        system("modexp.exe");
        fprintf(stdout, "Generate 'exp, mod' complete!\n");
        return 0;
    }
    else if (argc != 3)
    {
        PrintUsage();
        return -1;
    }

    srand(time(NULL));

    // Read exp, mod values from file
    if (GetExpModPrimeFromFile("exponent.txt", "modulus.txt", "prime.txt",
                               arExpBuf_E, arExpBuf_D, arModBuf, arBuf_P, arBuf_Q,
                               arBuf_DP, arBuf_DQ, arBuf_InvQ) == -1)
    {
        fprintf(stdout, "[ERR] GetExpModFromFile() failed.\n");
        return -1;
    }

    fprintf(stdout, "\n[e]:\n%s\n\n[d]:\n%s\n\n[N]:\n%s\n", arExpBuf_E, arExpBuf_D, arModBuf);
    fprintf(stdout, "\n[p]:\n%s\n\n[q]:\n%s\n\n[dp]:\n%s\n\n[dq]:\n%s\n\n[inv-q]:\n%s\n",
            arBuf_P, arBuf_Q, arBuf_DP, arBuf_DQ, arBuf_InvQ);

    // En/Decryption
    {
        char    *pIn            = arPlainBuf;
        char    *pOut           = arCipherBuf;
        int     isEncryption    = 0;

        isEncryption = (strcmp(pOption, "-e") == 0 ? 1 : 0);

        if (isEncryption)
        {
            strcpy(arPlainBuf, argv[2]);
            memset(arCipherBuf, 0x00, sizeof(arCipherBuf));
            pIn  = arPlainBuf;
            pOut = arCipherBuf;
        }
        else
        {
            strcpy(arCipherBuf, argv[2]);
            memset(arPlainBuf, 0x00, sizeof(arPlainBuf));
            pIn  = arCipherBuf;
            pOut = arPlainBuf;
        }

        // En/Decrypt RSA
        if (EnDecryptRSA(isEncryption, pIn, pOut, arExpBuf_E, arExpBuf_D, arModBuf,
                         arBuf_P, arBuf_Q, arBuf_DP, arBuf_DQ, arBuf_InvQ) == -1)
        {
            fprintf(stdout, "[ERR] EnDecryptRSA() failed.\n");
            return -1;
        }

        // Print result
        if (isEncryption)
        {
            fprintf(stdout, "\n[Plain Text]:\n%s\n", arPlainBuf);
            fprintf(stdout, "\n[Cipher Text]:\n%s\n", arCipherBuf);    
        }
        else
        {
            fprintf(stdout, "\n[Cipher Text]:\n%s\n", arCipherBuf);
            fprintf(stdout, "\n[Plain Text]:\n%s\n", arPlainBuf);
        }
    }

    return 0;
}