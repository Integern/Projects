/**
 * Executable class to test the StringStack abstract data type.
 */
public class StringStackTest {
    public static void main(String[] args) {
        StringStack s = new StringStack(0);
        System.out.println(s.pop() + " should be null");
        System.out.println(s.push("full") + " should be false");
        System.out.println(s.count() + " should be 0");
        System.out.println();

        s = new StringStack(1);
        System.out.println(s.push("banana") + " should be true");
        System.out.println(s.count() + " should be 1");
        System.out.println(s.pop() + " should be banana");
        System.out.println(s.pop() + " should be null");
        System.out.println(s.push("banana") + " should be true");
        System.out.println(s.push("apple") + " should be false");
        System.out.println();

        s = new StringStack(10);
        System.out.println(s.push("banana") + " should be true");
        System.out.println(s.push("apple") + " should be true");
        System.out.println(s.push("pear") + " should be true");
        System.out.println(s.pop() + " should be pear");
        System.out.println(s.push("plum") + " should be true");
        System.out.println(s.pop() + " should be plum");
        System.out.println(s.pop() + " should be apple");
        System.out.println(s.pop() + " should be banana");
        System.out.println(s.pop() + " should be null");
    }
}
