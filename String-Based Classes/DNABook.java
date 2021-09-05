import java.util.Arrays;

public class DNABook implements SocialNetwork {

    String userList[] = new String[100]; //Have at most 100 users
    int userCount = 0;

    boolean friendMatrix[][] = new boolean[100][100]; //The 100x100 table that stores if users are friends.


    @Override
    public void registerUser(String name) {
        userList[userCount] = name;
        userCount = userCount + 1;
    }



    private int binarySearch(String str, String[] array, int lo, int hi) {

        if (hi<=lo)
            return -1;

        int lookup = (hi + lo)/2;
        int result = array[lookup].compareTo(str);

        if (result>0) {
            return binarySearch(str, array, lo, lookup); //Result is positive => our entry is further along in the alphabet
                                                  //so search in the upper half of our range
        } else if (result<0) {
            return binarySearch(str, array,lookup+1, hi); //We know lookup doesn't contain the entry, so don't include that
                                                        //in the search.
        } else {
            return lookup; //We found a match!
        }
    }

    private int getUserID(String name) {
        String noNullUserList[] = Arrays.copyOfRange(userList, 0, userCount); //This gives us an array of just our strings
                                                                                  //such that there are no null values.

        return binarySearch(name, noNullUserList, 0, userCount); //Returns the userID via binary search, T( log(n) )
    }



    @Override
    public void becomeFriends(String name1, String name2) {

        int user1 = getUserID(name1);
        int user2 = getUserID(name2);

        if (user1 < 0 || user2 < 0) {
            System.out.println("Couldn't find one of the users in the database.");
            return;
        }

        friendMatrix[user1][user2] = true;
        friendMatrix[user2][user1] = true;

    }

    @Override
    public boolean areTheyFriends(String name1, String name2) {
        int user1 = getUserID(name1);
        int user2 = getUserID(name2);
        return friendMatrix[user1][user2];
    }
}
