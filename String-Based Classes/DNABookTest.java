/**
 * Executable class to test the DNABook abstract data type.
 */
public class DNABookTest {
    public static void main(String[] args) {
        SocialNetwork s = new DNABook();
        s.registerUser("Alex");
        s.registerUser("Bea");
        s.registerUser("Chris");
        s.registerUser("Daniel");
        s.registerUser("Ed");

        System.out.println(s.areTheyFriends("Alex", "Bea") + " should be false");
        System.out.println(s.areTheyFriends("Daniel", "Bea") + " should be false");
        System.out.println(s.areTheyFriends("Bea", "Chris") + " should be false");
        System.out.println();

        s.becomeFriends("Alex", "Chris");
        s.becomeFriends("Bea", "Daniel");

        System.out.println(s.areTheyFriends("Alex", "Chris") + " should be true");
        System.out.println(s.areTheyFriends("Chris", "Alex") + " should be true");
        System.out.println(s.areTheyFriends("Bea", "Daniel") + " should be true");
        System.out.println();

        System.out.println(s.areTheyFriends("Bea", "Ed") + " should be false");
        System.out.println(s.areTheyFriends("Ed", "Bea") + " should be false");
        System.out.println();
    }
}
