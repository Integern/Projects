public interface SocialNetwork {
    void registerUser(String name);
    void becomeFriends(String name1, String name2);
    boolean areTheyFriends(String name1, String name2);
}
