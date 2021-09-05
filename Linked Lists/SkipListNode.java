public class SkipListNode {

    public String element;
    public SkipListNode[] next;
    public int maxLanes = 5;

    public SkipListNode(String e) {
        element = e;
        next = new SkipListNode[maxLanes]; //Create our next array defining the maximum amount of lanes.
    }

}