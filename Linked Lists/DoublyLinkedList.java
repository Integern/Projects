class DoublyLinkedList {
    private ListNode2 head = null;
    private ListNode2 tail = null;
    private int n = 0; // list size

    // TODO implement this...

    /**
     * Prints out the elements in the list from the first to the last (front to back) and then from the last to the first
     * (back to front). This is useful to test whether the list nodes are connected correctly with next and prev references.
     */
    public void print() {
        // no elements to print for empty list
        if (head == null) {
            System.out.println("list empty.");
            return;
        }

        // follow next references to list elements from the front to the back of the list
        System.out.print("front to back: ");
        ListNode2 node = head;
        System.out.print(node.element + " ");
        while (node.next != null) {
            node = node.next;
            System.out.print(node.element + " ");
        }

        // follow prev references to list elements from the back to the front of the list
        System.out.print("-- and back to front: ");
        while (node != null) {
            System.out.print(node.element + " ");
            node = node.prev;
        }
        System.out.println();
    }

    public void addFirst(String s) {

        ListNode2 newNode = new ListNode2(s, null, head);

        if (n>0) //Head exists
            head.prev = newNode;
        head = newNode;

        n++;
    }

    public Object get(int i) {
        if (i<0 || i>=n) ; // …error!
        ListNode2 node = head;
        for (int j=0; j<i; j++)
            node = node.next;
        return node.element;
    }

    public void remove(int i) {

        if (i<0 || i>=n) ; // …error!
        if (i==0) { // special case
            head = head.next;

            if (head != null)
                head.prev = null;

            n--;
            return;
        }

        ListNode2 node = head;
        ListNode2 lastNode = node;
        for (int j=0; j<i; j++) {
            lastNode = node; //lastNode is always 1 behind node.
            node = node.next;
        }

        if (i==n-1) { //We want to remove the last object (the tail). lastNode is our new tail.
            lastNode.next = null;
            tail = lastNode; //This becomes our new last object with no pointer and defined as the tail.
        } else {
            ListNode2 before = lastNode;
            ListNode2 after = node.next;
            before.next = after;
            after.prev = before;
        }

        n--;
    }

    public void insert(String s, int i) {

        if (i<0 || i>n) ; // …error!
        if (i==0) { addFirst(s); return;}

        ListNode2 node = head;
        for (int j=0; j<i-1; j++)
            node = node.next; //Here node is the node that we want to come just before our new node.


        ListNode2 before = node;
        ListNode2 after = node.next;

        ListNode2 newNode = new ListNode2(s, node, node.next);
        before.next = newNode;
        if (after != null)
            after.prev = newNode;


        if (i==n) { //We're inserting at the end so have to define the new tail.
            tail = node.next;
        }

        n++;
    }

}
