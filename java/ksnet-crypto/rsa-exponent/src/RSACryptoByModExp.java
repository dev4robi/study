import java.io.*;
import java.security.*;
import java.security.spec.*;
import java.math.BigInteger;
import java.util.Base64;
import java.util.Base64.Encoder;
import java.util.Base64.Decoder;
import java.util.Map;
import java.util.HashMap;
import javax.crypto.*;

public class RSACryptoByModExp {

    private static String RSA_OPTION = "RSA/ECB/PKCS1Padding"; // "RSA/ECB/NoPadding";

    public static String streamToHexa(byte[] inByte) {
        StringBuilder sb = new StringBuilder();

        for (byte b : inByte) {
            sb.append(String.format("%02X ", b));
        }

        sb.setLength(sb.length() - 1);

        return sb.toString();
    }

    public static void GetExpModFromFile(Map<String, String> expModMap) {
        String modStr = null, expStr_E = null, expStr_D = null;
        FileReader modFr = null, expFr = null;
        BufferedReader modBr = null, expBr = null;

        try {                
            modFr = new FileReader(new File("./modulus.txt"));
            expFr = new FileReader(new File("./exponent.txt"));
            modBr = new BufferedReader(modFr);
            expBr = new BufferedReader(expFr);

            modStr = modBr.readLine();   // �� �Ҽ��� �� (modulus)
            expStr_E = expBr.readLine(); // 1��° �� (����Ű)
            expStr_D = expBr.readLine(); // 2��° �� (���Ű)
        }
        catch (Exception e2) {
            e2.printStackTrace();
        }
        finally {
            try {
                modFr.close();
                expFr.close();
                modBr.close();
                expBr.close();
            }
            catch (Exception e3) {
                e3.printStackTrace();
            }
        }

        expModMap.put("e", expStr_E);
        expModMap.put("d", expStr_D);
        expModMap.put("mod", modStr);
    }

    public static String Encrypt(String plainStr) {
        // ��ȣȭ Ű(����Ű)�� ���Ϸκ��� ����
        RSAPublicKeySpec pubKeySpec = null;
        String cipherBase64 = null;

        try {
            Map<String, String> expModMap = new HashMap<String, String>();            
            GetExpModFromFile(expModMap);
            pubKeySpec = new RSAPublicKeySpec(new BigInteger(expModMap.get("mod")), new BigInteger(expModMap.get("e")));

            System.out.println("\n�� �Ҽ��� �� (modulus : N=pq) : " + pubKeySpec.getModulus());
            System.out.println("\n����Ű (e) : " + pubKeySpec.getPublicExponent());

            // ��ȣȭ ����
            Encoder b64Encoder = null;
            KeyFactory keyFactory = KeyFactory.getInstance("RSA");
            PublicKey pubKey = keyFactory.generatePublic(pubKeySpec);

            Cipher cipher = Cipher.getInstance(RSA_OPTION);
            cipher.init(Cipher.ENCRYPT_MODE, pubKey);
            
            byte[] cipherByte = cipher.doFinal(plainStr.getBytes("EUC-KR"));
            
            b64Encoder = Base64.getEncoder();
            cipherBase64 = new String(b64Encoder.encode(cipherByte), "EUC-KR");
        }
        catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException |
               IllegalBlockSizeException | BadPaddingException | UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        finally {
            return cipherBase64;
        }
    }

    public static String Decrypt(String cipherStr) {
        // ��ȣȭ Ű(���Ű)�� ���Ϸκ��� ����
        RSAPrivateKeySpec priKeySpec = null;
        String plainStr = null;

        try {
            Map<String, String> expModMap = new HashMap<String, String>();            
            GetExpModFromFile(expModMap);
            priKeySpec = new RSAPrivateKeySpec(new BigInteger(expModMap.get("mod")), new BigInteger(expModMap.get("d")));

            System.out.println("\n�� �Ҽ��� �� (modulus : N=pq) : " + priKeySpec.getModulus());
            System.out.println("\n���Ű (d) : " + priKeySpec.getPrivateExponent());
            
            // Base64���ڿ��� ����Ʈ�� ��ȯ �� ��ȣȭ ����
            KeyFactory keyFactory = KeyFactory.getInstance("RSA");
            PrivateKey priKey = keyFactory.generatePrivate(priKeySpec);

            byte[] cipherByte = Base64.getDecoder().decode(cipherStr.getBytes("EUC-KR"));
            
            Cipher cipher = Cipher.getInstance(RSA_OPTION);
            cipher.init(Cipher.DECRYPT_MODE, priKey);

            byte[] plainByte = cipher.doFinal(cipherByte);
            plainStr = new String(plainByte, "EUC-KR");
        }
        catch (NoSuchAlgorithmException | NoSuchPaddingException | InvalidKeyException |
               IllegalBlockSizeException | BadPaddingException | UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        finally {
            return plainStr;
        }
    }

    public static void printUsage() {
        System.out.println("\n> RSACrypto Usage: java RSACrypto [Op] [Str]"      );
        System.out.println(" 1. [Op]"                                            );
        System.out.println("  1) -e : Encrypt"                                   );
        System.out.println("  2) -d : Decrypt"                                   );
        System.out.println("  3) -k : Generate mod and exp value from modexp.exe");
        System.out.println(" 2. [Str] : \"PlainText or Base64CipherText.\""      );
    }

    public static void main(String[] args) {
        try {
            // ���� üũ
            if (!args[0].equals("-k") && args.length != 2) {
                System.out.println("> args.lenght Error!");
                printUsage();
                return;
            }

            // ��� ����
            if (args[0].equals("-k")) { // modexp.exe ������Ѽ� exp, mod�� ����
                Process proc;
                Runtime rt = Runtime.getRuntime();
                proc = rt.exec("./modexp.exe");
                proc.waitFor();
                System.out.println("mod, exp ���� �Ϸ�.");
            }
            else if (args[0].equals("-e")) { // ��ȣȭ
                String cipherStr = Encrypt(args[1]);
                System.out.println("\n[ RSA Encrypt ]");
                System.out.println("\n<��ȣ��> : " + cipherStr);
                System.out.println("\n< HEX > : " + streamToHexa(cipherStr.getBytes("EUC-KR")));
            }
            else if (args[0].equals("-d")) { // ��ȣȭ
                String plainStr = Decrypt(args[1]);
                System.out.println("\n[ RSA Decrypt ]");
                System.out.println("\n<��ȣ��> : " + plainStr);
                System.out.println("\n< HEX > : " + streamToHexa(plainStr.getBytes("EUC-KR")));
            }
            else { // ����
                System.out.println("> Unknown args[0] Error!");
                printUsage();
                return;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}