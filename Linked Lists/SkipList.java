public class SkipList {

    private int n = 0; // list size
    public int maxLanes;

    private SkipListNode head;
    private SkipListNode tail;


    public SkipList() { //Initialisation

        head = new SkipListNode("//HEAD");
        tail = new SkipListNode("//TAIL");

        maxLanes = head.maxLanes; //Max lanes is defined in SkipListNode.java

        for (int i=0; i<maxLanes; i++){
            head.next[i] = tail; //Define each row for the head to point to the tail
        }

    }


    public void createTestList() {

       SkipListNode Anne = new SkipListNode("Anne");
       SkipListNode Ben = new SkipListNode("Ben");
       SkipListNode Charlie = new SkipListNode("Charlie");
       SkipListNode Don = new SkipListNode("Don");
       SkipListNode Ernie = new SkipListNode("Ernie");
       n = 5;

       //Bottom Row
       head.next[0] = Anne; Anne.next[0] = Ben; Ben.next[0] = Charlie;
       Charlie.next[0] = Don; Don.next[0] = Ernie; Ernie.next[0] = tail;

       //First Row
       head.next[1] = Anne; Anne.next[1] = Charlie; Charlie.next[1] = Ernie; Ernie.next[1] = tail;

       //Second Row
       head.next[2] = Anne; Anne.next[2] = Ernie; Ernie.next[2] = tail;

    }

    public void print() {
        for (int i=maxLanes-1; i>=0; i--){

            StringBuffer str = new StringBuffer();
            SkipListNode node = head;

            while (true) {

                if (node.next[i] != null && node.next[i] != tail) { //If we have a valid node next in the sequence.
                    node = node.next[i];
                    str.append(node.element + ",");
                } else { break; } //When we've looped through the whole row.

            }

            System.out.println("Row " + i + ": " + str);

        }
    }

    public boolean inList(String s) {

        SkipListNode currentNode = head;
        SkipListNode prevNode = head; //As the code gets going prevNode will genuinely be the previous node.

        for (int i=maxLanes-1; i>=0; i--){ //This loops through each lane, starting at the top.
            if (head.next[i] != tail) { //Starting at the top lane, we'll search lanes that have an entry.

                if (currentNode == head) currentNode = head.next[i]; //First time we get to this bit of code, define our current node.

                for (int j = 0; j<= n; j++){

                    int score = s.compareTo(currentNode.element); // 0 = same; 1 = 's' is higher in the alphabet; -1 = lower

                    if (score==0) return true; //Found our exact match!

                    if (score<0 || currentNode == tail) { //We've gone too far in our search, back up once and go down.
                        currentNode = prevNode;
                        break;
                    } else if (score>0) { //We need to keep looking further along the current row.
                        prevNode = currentNode;
                        currentNode = currentNode.next[i];
                    }

                }

            }
        }

        return false;

    }


    private SkipListNode[] findNodeBelow(String s){ //This function will return the node that our new node should be placed after.

        SkipListNode currentNode = head;
        SkipListNode prevNode = head; //As the code gets going prevNode will genuinely be the previous node.

        SkipListNode reply[] = new SkipListNode[maxLanes]; //This stores all of the last acceptable nodes on each row so that
                                                           //it's easy to add our node to all the rows if needs be.

        for (int i=maxLanes-1; i>=0; i--){ //This loops through each lane, starting at the top.
            if (head.next[i] != tail) { //Starting at the top lane, we'll search lanes that have an entry.

                if (currentNode == head) currentNode = head.next[i]; //First time we get to this bit of code, define our current node.

                for (int j = 0; j<= 2*n; j++){

                    int score = s.compareTo(currentNode.element); // 0 = same; 1 = 's' is higher in the alphabet; -1 = lower

                    if (score<0 || currentNode == tail) { //We've gone too far in our search, back up once and go down.
                        currentNode = prevNode;
                        break;
                    } else if (score>0) { //We need to keep looking further along the current row.
                        prevNode = currentNode;
                        currentNode = currentNode.next[i];
                    }

                }
                reply[i] = currentNode; //This is the node furthest to the right on the search of our row that is
                                        //alphabetically just before our string.


            }
        }

        return reply;

    }

    public void insert(String s) {

        //STEP 1: Find the nodes in each row to insert after. Find the node just before using a similar method to inList
        //STEP 2: Create our node, then calculate probabilities to insert it into other rows
        //STEP 3: Insert it in all the required rows, updating the next values of adjacent nodes.

        SkipListNode lastNodes[] = findNodeBelow(s); //This gives us the nodes just before. (Step 1)
        SkipListNode newNode = new SkipListNode(s); // (Step 2)

        for (int i = 0; i<maxLanes; i++){

            SkipListNode nodeBefore = lastNodes[i]; //Gives us the last node searched on this row
            if (nodeBefore == null) continue; //Failsafe
            SkipListNode nodeAfter = nodeBefore.next[i];

            nodeBefore.next[i] = newNode; //Insert it in!
            newNode.next[i] = nodeAfter;

            if (Math.random() > 0.5) break; //With a 50% probability this will go onto the next row (acts recursively
                                            //such that on the 3rd loop theres p=1/8)

        }



    }

}
