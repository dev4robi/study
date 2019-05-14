#include "ncode_base64.h"
#include "bytestream.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/rsaerr.h>
#include <openssl/err.h>

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
*/

#define SZ_BUF_MAX 1024

int GetExpModFromFile(char *pExpFileName, char *pModFileName, char *pExpBuf_E, char *pExpBuf_D, char *pModBuf)
{
    FILE    *pExpFile = NULL, *pModFile = NULL;
    char    arLineBuf[SZ_BUF_MAX];
    int     i;

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

    // Read Exp File
    int nLineCnt = 0;
    while (fgets(arLineBuf, SZ_BUF_MAX, pExpFile) != NULL)
    {
        nLineCnt == 0 ? strcpy(pExpBuf_E, arLineBuf) : strcpy(pExpBuf_D, arLineBuf);
        if ((++nLineCnt) == 2) break;
    }

    // Read Mod File
    while (fgets(arLineBuf, SZ_BUF_MAX, pModFile) != NULL)
    {
        strcpy(pModBuf, arLineBuf);
        break;
    }

    // Remove '\n' character
    for (i = strlen(pExpBuf_E) - 1; i > -1; --i)
    {
        pExpBuf_E[i] == '\n' ? pExpBuf_E[i] = '\0' : 0;
    }

    for (i = strlen(pExpBuf_D) - 1; i > -1; --i)
    {
        pExpBuf_D[i] == '\n' ? pExpBuf_D[i] = '\0' : 0;
    }

    for (i = strlen(pModBuf) - 1; i > -1; --i)
    {
        pModBuf[i] == '\n' ? pModBuf[i] = '\0' : 0;
    }

    fclose(pExpFile);
    fclose(pModFile);

    return 0;
}

int EnDecryptRSA(int isEncryption, char *pInStr, char *pOutStr, char *pE, char *pD, char *pN)
{
    RSA     *pRSA = NULL;
    BIGNUM  *pBN_E = NULL, *pBN_D = NULL, *pBN_N = NULL;
    char    arInBuf[SZ_BUF_MAX], arOutBuf[SZ_BUF_MAX];
    int     szInput = -1, rtLen = -1;

    memset(arInBuf, 0x00, SZ_BUF_MAX);
    memset(arOutBuf, 0x00, SZ_BUF_MAX);

    // ��ǲ ����
    strcpy(arInBuf, pInStr);

    // BIGNUM ����ü ����
    pBN_N = BN_new();
    pBN_E = BN_new();
    pBN_D = BN_new();

    if (!BN_dec2bn(&pBN_N, pN) || !BN_dec2bn(&pBN_E, pE) || !BN_dec2bn(&pBN_D, pD))
    {
        fprintf(stdout, "[ERR] BIGNUM(N, E, D) create failed.\n");
        goto END_BN_CLR;
    }

    // RSA ����ü ����
    pRSA = RSA_new();

    if (RSA_set0_key(pRSA, pBN_N, pBN_E, pBN_D) == 0)
    {
        fprintf(stdout, "[ERR] RSA_set0_key() failed.\n");
        goto END_BN_CLR;
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
        szInput = strlen(arInBuf);
        
        if ((rtLen = RSA_public_encrypt(szInput, arInBuf, arOutBuf, pRSA, RSA_PKCS1_PADDING)) == -1)
        {
            fprintf(stdout, "[ERR] RSA_public_encrypt() failed.\n");
            goto END;
        }
    }
    else
    {
        if ((rtLen = RSA_private_decrypt(szInput, arInBuf, arOutBuf, pRSA, RSA_PKCS1_PADDING)) == -1)
        {
            fprintf(stdout, "[ERR] RSA_private_decrypt() failed.\n");
            goto END;
        }
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

    goto END;

END_BN_CLR:
    if (pBN_N != NULL) BN_free(pBN_N);
    if (pBN_E != NULL) BN_free(pBN_E);
    if (pBN_D != NULL) BN_free(pBN_D);

END:
    if (pRSA != NULL)
    {
        if (rtLen == -1) ERR_print_errors_fp(stdout);
        RSA_free(pRSA);
    }

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

    // Read exp, mod values from file
    if (GetExpModFromFile("exponent.txt", "modulus.txt", arExpBuf_E, arExpBuf_D, arModBuf) == -1)
    {
        fprintf(stdout, "[ERR] GetExpModFromFile() failed.\n");
        return -1;
    }

    fprintf(stdout, "\n[e]:\n%s\n\n[d]:\n%s\n\n[N]:\n%s\n", arExpBuf_E, arExpBuf_D, arModBuf);

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
        if (EnDecryptRSA(isEncryption, pIn, pOut, arExpBuf_E, arExpBuf_D, arModBuf) == -1)
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

            PrintStreamToHexa(arPlainBuf, strlen(arPlainBuf));
        }
    }

    return 0;
}