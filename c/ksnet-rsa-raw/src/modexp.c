#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>

/*
    [ ���α׷� ��ǥ ]

    1. �� �Ҽ��� �� pq=N(modulus) ���ϱ�.
    2. ����Ű : e(public exponent) ���ϱ�.
    3. ���Ű : d(private exponent) ���ϱ�.

    + ����Ű�� ���������� ���� ����ϴ� �Ҽ� 65537�� ���.
*/
/*
    [ �ΰ����� : RSA ��� ]

    1. �� �Ҽ� p, q�� �غ�
    2. p-1, q-1�� ���� ���μ��� ���� e�� �غ�
    3. e*d�� (p-1)(q-1)�� ���� �������� 1�� �ǵ��� �ϴ� d�� ã��
    4. N=pq�� ����� �� N�� e�� ���� (����Ű), d�� ����Ű�� ���
    5. p, q, (p-1), (q-1)�� ���Ȼ� ������ �ǹǷ� ����

    [ �߰� ���� ]
     1. dP = d mod (p-1)
     2. dQ = d mod (q-1)
     3. qInv = q^-1 mod p
     -> CRT(�߱����� ������ ����) �� ����ϱ� ���� �߰� ����.
*/

static const int RSA_BIT = 2048;

int GetRSA_Val(char *pN, char *pE, char *pD, char *pP, char *pQ, char *pDP, char *pDQ, char *pInvQ,
               int rsaBit, int makeNewKey)
{
    static RSA      *pRSA       = NULL;
    BIGNUM          *pBN_PubKey = NULL;
    unsigned long   pubExp      = 0x10001UL; // Public Exponent (65537)
    char            *pTempStr   = NULL;
    int             rtVal       = -1;
 
    /* Generate RSA private key */
    if (makeNewKey)
    {
        if (pRSA != NULL) RSA_free(pRSA); // Remove old one

        pBN_PubKey = BN_new();
        rtVal = BN_set_word(pBN_PubKey, pubExp);
        if (rtVal != 1) { fprintf(stdout, "RSA set public key error!\n"); goto END; }
    
        pRSA = RSA_new();
        rtVal = RSA_generate_key_ex(pRSA, rsaBit, pBN_PubKey, NULL);
        if (rtVal != 1) { fprintf(stdout, "RSA private key generate error!\n"); goto END; }
    }

    if (pRSA == NULL)
    {
        fprintf(stdout, "First, generate RSA struct. (set parameter 'makeNewKey = 1')\n");
        rtVal = -1;
        goto END;
    }

    /* Return RSA key value */
    if (pN != NULL)
    {
        pTempStr = BN_bn2dec(RSA_get0_n(pRSA)); // Multiply of p*q 'N'
        strcpy(pN, pTempStr);
        fprintf(stdout, "p*q modulus(N) : %s\n\n", pN);
    }

    if (pE != NULL)
    {
        pTempStr = BN_bn2dec(RSA_get0_e(pRSA)); // Public key exponent 'e'
        strcpy(pE, pTempStr);
        fprintf(stdout, "Public key exponent(Public key 'e') : %s\n\n", pE);
    }

    if (pD != NULL)
    {
        pTempStr = BN_bn2dec(RSA_get0_d(pRSA)); // Private key exponent 'd'
        strcpy(pD, pTempStr);
        fprintf(stdout, "Private key exponent(Private key 'd') : %s\n\n", pD);
    }

    if (pP != NULL)
    {
        pTempStr = BN_bn2dec(RSA_get0_p(pRSA)); // Prime number 'p'
        strcpy(pP, pTempStr);
        fprintf(stdout, "1St Prime number ('p') : %s\n\n", pP);
    }
    
    if (pQ != NULL)
    {
        pTempStr = BN_bn2dec(RSA_get0_q(pRSA)); // Prime number 'q'
        strcpy(pQ, pTempStr);
        fprintf(stdout, "2nd Prime Number ('q') : %s\n\n", pQ);
    }
    
    if (pDP != NULL)
    {
        pTempStr = BN_bn2dec(RSA_get0_dmp1(pRSA)); // Prime number exponent 'dp'
        strcpy(pDP, pTempStr);
        fprintf(stdout, "Prime Number ('dp') : %s\n\n", pDP);
    }

    if (pDQ != NULL)
    {
        pTempStr = BN_bn2dec(RSA_get0_dmq1(pRSA)); // Prime number exponent 'dq'
        strcpy(pDQ, pTempStr);
        fprintf(stdout, "Prime Number ('dq') : %s\n\n", pDQ);
    }

    if (pInvQ != NULL)
    {
        pTempStr = BN_bn2dec(RSA_get0_iqmp(pRSA)); // Inverse prime number 'inv-q'
        strcpy(pInvQ, pTempStr);
        fprintf(stdout, "Prime Number Inverse ('inv-q') : %s\n\n", pInvQ);
    }

    rtVal = 0; // No errors
 
END:
    /* Free temporal memory */
    if (pBN_PubKey != NULL) BN_free(pBN_PubKey);

    return rtVal;
}

int MakeModulusFile(char *pFileName, int makeNewKey)
{
    FILE    *pOutFile = NULL;
    char    ar_N_Str[1024];

    if ((pOutFile = fopen(pFileName, "w")) == NULL)
    {
        fprintf(stdout, "Fail to create modulus file '%s'...\n", pFileName);
        return -1;
    }

    if (GetRSA_Val(ar_N_Str, NULL, NULL, NULL, NULL, NULL, NULL, NULL, RSA_BIT, makeNewKey) == -1)
    {
        fprintf(stdout, "Fail to generate RSA private key...\n");
        return -1;
    }

    fwrite(ar_N_Str, sizeof(char), strlen(ar_N_Str), pOutFile);
    fclose(pOutFile);

    return 0;
}

int MakeExponentFile(char *pFileName, int makeNewKey)
{
    FILE    *pOutFile = NULL;
    char    arPubStr[1024];
    char    arPriStr[1024];

    if ((pOutFile = fopen(pFileName, "w")) == NULL)
    {
        fprintf(stdout, "Fail to create exponent file '%s'...\n", pFileName);
        return -1;
    }

    if (GetRSA_Val(NULL, arPubStr, arPriStr, NULL, NULL, NULL, NULL, NULL, RSA_BIT, makeNewKey) == -1)
    {
        fprintf(stdout, "Fail to generate RSA private key...\n");
        return -1;
    }

    fwrite(arPubStr, sizeof(char), strlen(arPubStr), pOutFile);
    fprintf(pOutFile, "\n");
    fwrite(arPriStr, sizeof(char), strlen(arPriStr), pOutFile);
    fclose(pOutFile);

    return 0;
}

int MakePrimeFile(char *pFileName, int makeNewKey)
{
    FILE    *pOutFile = NULL;
    char    arStr_P[1024];
    char    arStr_Q[1024];
    char    arStr_DP[1024];
    char    arStr_DQ[1024];
    char    arStr_InvQ[1024];

    if ((pOutFile = fopen(pFileName, "w")) == NULL)
    {
        fprintf(stdout, "Fail to create prime file '%s'...\n", pFileName);
        return -1;
    }

    if (GetRSA_Val(NULL, NULL, NULL, arStr_P, arStr_Q, arStr_DP, arStr_DQ, arStr_InvQ, RSA_BIT, makeNewKey) == -1)
    {
        fprintf(stdout, "Fail to generate RSA private key...\n");
        return -1;
    }

    fwrite(arStr_P, sizeof(char), strlen(arStr_P), pOutFile);
    fprintf(pOutFile, "\n");
    fwrite(arStr_Q, sizeof(char), strlen(arStr_Q), pOutFile);
    fprintf(pOutFile, "\n");
    fwrite(arStr_DP, sizeof(char), strlen(arStr_DP), pOutFile);
    fprintf(pOutFile, "\n");
    fwrite(arStr_DQ, sizeof(char), strlen(arStr_DQ), pOutFile);
    fprintf(pOutFile, "\n");
    fwrite(arStr_InvQ, sizeof(char), strlen(arStr_InvQ), pOutFile);
    fclose(pOutFile);

    return 0;
}

int main(int argc, char **argv)
{
    int rtVal = 0;

    fprintf(stdout, "\n");

    if ((rtVal = MakeExponentFile("exponent.txt", 1)) == -1) // pub&pri exponent (Make new key)
    {
        fprintf(stdout, "MakeExponentFile() error!\n");
        return -1;
    }

    if ((rtVal = MakeModulusFile("modulus.txt", 0)) == -1) // modulus (Use old key)
    {
        fprintf(stdout, "MakeModulusFile() error!\n");
        return -1;
    }

    if ((rtVal = MakePrimeFile("prime.txt", 0)) == -1) // prime (Use old key)
    {
        fprintf(stdout, "MakePrimeFile() error!\n");
        return -1;
    }

    return 0;
}