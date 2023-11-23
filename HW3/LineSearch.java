import java.io.*;
import java.lang.Math;

public class LineSearch {
    static final double epsilon = 0.000001;
    public static void main(String[] args) throws IOException {
        PrintWriter pw = new PrintWriter("LineSearch.out");
        double x1 = 0.0, x2 = 0.0;
        int k = 0;
        double[] d = {0.0, 0.0};

        pw.printf(" %2s | %10s | %10s | %13s | %13s | %22s %n", "k", "x1", "x2", "d[0]", "d[1]", "f(x)");
        pw.printf("--------------------------------------------------------------------------------------%n");
        pw.printf(" %2d | % 10f | % 10f | % 13e | % 13e | % 22.15e %n", k, x1 ,x2, d[0], d[1], f(x1, x2));
        while (!threshold(x1, x2)) {
            double L, minL, LF, minLF;
            L = minL = 0.0;
            LF = minLF = Double.POSITIVE_INFINITY;
            pw.printf("--------------------------------------------------------------------------------------%n");
            pw.printf(" %2s | %11s%-44s |%n", "", "", "Look for new step size lambda (L)");
            pw.printf(" %2s | %10s | %10s | %13s | %13s |%n","", "f(minL)", "f(L)", "minL", "L");
            pw.printf(" %2s | % 10f | % 10f | % 13e | % 13e |%n", "", minLF, LF, minL, L);
            while (LF <= minLF) {
                minLF = LF;
                minL = L;
                L += 0.1;
                LF = fLambda(x1, x2, L);
                pw.printf(" %2s | % 10f | % 10f | % 13e | % 13e |%n", "", minLF, LF, minL, L);
            }
            d = gradF(x1, x2);
            x1 -= L*d[0];
            x2 -= L*d[1];
            k++;
            pw.printf("--------------------------------------------------------------------------------------%n");
            pw.printf(" %2d | % 10f | % 10f | % 13e | % 13e | % 22.15e %n", k, x1 ,x2, d[0], d[1], f(x1, x2));
        }
        pw.printf("%nOptimal solution x = (%f, %f)\t f(x) = % 22.15e%n", x1, x2, f(x1, x2));
        pw.close();
    }
    
    private static double f (double x1, double x2) {
        return (x1*(x1 - 11) + Math.pow(x2, 2) + x1*(x2 + 9));
    }

    private static double[] gradF (double x1, double x2) {
        double[] result = new double[2];
        result[0] = 2*x1 + x2 - 2;
        result[1] = x1 + 2*x2;
        return result;
    }

    private static boolean threshold (double x1, double x2) {
        return (Math.sqrt(5*Math.pow(x1,2) + 5*Math.pow(x2,2) + 8*x1*x2 - 8*x1 - 4*x2 + 4) < epsilon);
    }

    private static double fLambda (double x1, double x2, double lambda) {
        double[] grad = gradF(x1, x2);
        return f(x1 - lambda * grad[0], x2 - lambda * grad[1]);
    }
}