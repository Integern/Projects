public class Question2 {

    public static void main(String[] args) {

        int nList[] = {1, 100, 1000, 10000};
        StringRepeater stringRepeat = new StringRepeater();

        System.out.println("/// Naive runtime");
        for (int x = 0; x <= 3; x++) {

            long time0 = System.nanoTime();
            stringRepeat.repeatString("hello", nList[x]);
            long time1 = System.nanoTime();

            double duration = (time1-time0);
            System.out.println("T(" + nList[x] + ") = " + duration/1E9 + " seconds");

        }

        /////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////

        int repeats = 10000;
        double timeList[] = new double[4];

        System.out.println("/// Fairly precise runtime");
        for (int x = 0; x <= 3; x++) {

            double total = 0;

            for (int y = 0; y < repeats; y++) {
                long time0 = System.nanoTime();
                stringRepeat.repeatString("hello", nList[x]);
                long time1 = System.nanoTime();

                total = total + (time1-time0);
            }

            System.out.println("T(" + nList[x] + ") = " + (total/repeats)/1E9 + " seconds");

        }
    }
}
