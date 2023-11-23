import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Random;

public class SA {
    static final double epsilon = 0.0001, t=0.001;
    static final int maxIter = 100;
    static final double Tstart = 1;
    public static void main(String[] args) throws FileNotFoundException {
        PrintWriter pw = new PrintWriter("SA.out");
        long seed = (new Random()).nextLong(), bestseed = seed;
        // long seed = -3818999476582591151L;
        Random rng = new Random(seed);
        double x1 = 7.0, x2 = 17.0;
        double x1rng = x1, x2rng = x2;
        double fbest = f(x1,x2), fbesti = Double.POSITIVE_INFINITY, fbestrng = fbest;

        for (int r=0; r<20; r++) {
            x1 = 7.0;
            x2 = 17.0;
            fbest = f(x1,x2);
            fbesti = Double.POSITIVE_INFINITY;
            double x1best = x1, x2best = x2, d = 1.0;
            double change1, change2, fk = fbest, fkplus1;
            int k=0, i=0;
            double T = Tstart;
            pw.printf("%2s| %2s| %5s| %10s| %10s| %13s| %7s| %7s| %7s%n", "i", "k", "T", "x1", "x2", "d", "fk", "fbest", "fbesti");
            pw.printf("----------------------------------------------------------------------------%n");
            pw.printf("%2d| %2s| %.3f| %10f| %10f| % 13e| %04.2f| %04.2f| %04.2f%n", i, " ", T, x1 ,x2, d, fk, fbest, fbesti);
            while (!threshold(fbest, fbesti)) {
                if (fbesti < fbest) {
                    fbest = fbesti;
                    fbesti = Double.POSITIVE_INFINITY;
                }
                for (k=0; k<maxIter; k++) {
                    while (true) {
                        change1 = rng.nextDouble()*2;
                        change2 = rng.nextDouble()*2;
                        if (change1*2 < 0.5) {
                            change1 = -change1;
                        }
                        if (change2*2 < 0.5) {
                            change2 = -change2;
                        }
                        if (x1 + change1 >= 0 && x1 + change1 <= 20 &&
                            x2 + change2 >= 0 && x2 + change2 <= 20) {
                            break;
                        }
                    }
                    
                    fk = f(x1, x2);
                    if (fk < fbesti) {
                        fbesti = fk;
                        x1best = x1;
                        x2best = x2;
                    }
                    pw.printf("%2d| %2d| %.3f| %10f| %10f| % 13e| %04.2f| %04.2f| %04.2f%n", i, k, T, x1 ,x2, d, fk, fbest, fbesti);
                    
                    fkplus1 = f(x1 + change1, x2 + change2);
                    d = fkplus1 - fk;
                    if (accept(d, T)) {
                        x1 += change1;
                        x2 += change2;
                        pw.printf("%2d| %2d| %.3f| %10f| %10f| % 13e| %04.2f| %04.2f| %04.2f%n", i, k, T, x1 ,x2, d, fkplus1, fbest, fbesti);
                    }
                }
                if (T-t > 0) T -= t;
                x1 = x1best;
                x2 = x2best;
                pw.printf("%2d| %2s| %.3f| %10f| %10f| % 13e| %04.2f| %04.2f| %04.2f%n", ++i, " ", T, x1 ,x2, d, fk, fbest, fbesti);
                pw.printf("-----------------------------------------------------------------------------%n");
            }
            if (fbest < fbestrng) {
                bestseed = seed;
                fbestrng = fbest;
                x1rng = x1best;
                x2rng = x2best;
            }
            seed = (new Random()).nextLong();
            rng = new Random(seed);
            pw.printf("%2s| %2s| %5s| %10f| %10f| %13s| %7s| %04.2f| %7s%n", " ", " ", " ", x1best ,x2best, " ", " ", fbest, " ");
            pw.printf("%40sRNG seed: %d%n", " ",seed);
        }
        pw.printf("-----------------------------------------------------------------------------%n");
        pw.printf("-----------------------------------------------------------------------------%n");
        pw.printf("%2s| %2s| %5s| %10f| %10f| %13s| %7s| %04.2f| %7s%n", " ", " ", " ", x1rng ,x2rng, " ", " ", fbestrng, " ");
        pw.printf("%40sRNG bestseed: %d%n", " ",bestseed);
        pw.close();
    }

    public static double f(double x1, double x2) {
        return (x1*(x1-5) + Math.pow(x2,3) - x1*(x2+11));
    }

    public static boolean accept(double d, double T) {
        if (d > 0) {
            return (Math.random() < Math.exp(-(d/T)));
        } else {
            return true;
        }
    }

    public static boolean threshold(double fbest, double fbesti) {
        return (Math.abs(fbest - fbesti) < epsilon);
    }
}