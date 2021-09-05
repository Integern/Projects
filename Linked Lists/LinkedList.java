class LinkedList {

    private ListNode head = null;
    private ListNode tail = null;
    private int n = 0; // list size


    public void addFirst(Object o) {
        head = new ListNode(o, head);
        n++;
    }

    public void add(Object o) {

        ListNode newNode = new ListNode(o, null);
        if (n>0) {
            tail.next = newNode;
        } else { //We're inserting at the end of an empty list.
            head = newNode;
        }
        tail = newNode;

        n++;

    }

    public Object get(int i) {
        if (i<0 || i>=n) ; // …error!
        ListNode node = head;
        for (int j=0; j<i; j++)
            node = node.next;
        return node.element;
    }

    public void insert(Object o, int i) {

        if (i<0 || i>n) ; // …error!
        if (i==0) { addFirst(o); return;}

        ListNode node = head;
        for (int j=0; j<i-1; j++)
            node = node.next;
        node.next = new ListNode(o,
                node.next);

        if (i==n) { //We're inserting at the end so have to define the new tail.
            tail = node.next;
        }

        n++;
    }

    public void remove(int i) { //Removing object i+1

        if (i<0 || i>=n) ; // …error!
        if (i==0) { // special case
            head = head.next; n--; return; }

        ListNode node = head;
        ListNode lastNode = node;
        for (int j=0; j<i; j++) {
            lastNode = node; //lastNode is always 1 behind node.
            node = node.next;
        } // node is the node we want to remove.

        ListNode nodeBefore = lastNode;
        ListNode nodeAfter = node.next;

        if (i==n-1) { //We want to remove the last object (the tail). lastNode is our new tail.
            nodeBefore.next = null;
            tail = nodeBefore; //This becomes our new last object with no pointer and defined as the tail.
        } else {
        nodeBefore.next = nodeAfter; }

        n--;
    }


    public void showList() {
        System.out.println("// Current List: //");
        ListNode node = head;
        System.out.println(node.element);
        for (int j=0; j<n-1; j++) {
            node = node.next;
            System.out.println(node.element);
        }
        System.out.println("//////////");
    }

}