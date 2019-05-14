import java.util.Scanner;

public class BJ_10817 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String inParam = scanner.nextLine();
        String inStr = scanner.nextLine();

        String[] inParamAry = inParam.split(" ");
        String[] inStrAry = inStr.split(" ");

        int count = Integer.parseInt(inParamAry[0]); 
        int less = Integer.parseInt(inParamAry[1]);

        for (int i = 0; i < count; ++i) {
            int inNum = Integer.parseInt(inStrAry[i]);

            if (inNum < less) {
                System.out.print(inNum);

                if (i + 1 < count) {
                    System.out.print(" ");
                }
            }
        }

        scanner.close();
    }
}