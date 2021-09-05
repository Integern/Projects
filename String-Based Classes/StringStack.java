/** A stack abstract data type that contains Strings. */
public class StringStack {
	// TODO add variables for data here
	String stringArray[];
    int stringCapacity;
    int currentLength;

	/**
	* Constructor for creating a new StringStack with a certain capacity.
	* @param capacity the maximum number of strings the stack can hold
	*/
	public StringStack(int capacity) {
		stringArray = new String[capacity];
        stringCapacity = capacity;
        currentLength = 0;
	}

	
	/**
	* Puts the given String on top of the stack (if there is enough space).
	* @param s the String to add to the top of the stack
	* @return false if there was not enough space in the stack to add the string;
	*         otherwise true
	*/
	public boolean push(String s) {
	    if (currentLength < stringCapacity) {
            stringArray[currentLength] = s; //As our index begins at 0
            currentLength = currentLength + 1;
            return true;
        }
        return false;
	}
	
	/**
	* Removes the String on top of the stack from the stack and returns it.
	* @return the String on top of the stack, or null if the stack is empty.
	*/
	public String pop() {
		String temp;
		if (currentLength > 0) {
			temp = stringArray[currentLength-1];
			stringArray[currentLength-1] = null;
			currentLength = currentLength - 1;
		} else {
			return null;
		}
		return temp;
	}
	
	/**
	* Returns the number of Strings in the stack.
	* @return the number of Strings in the stack
	*/
	public int count() {
		return currentLength;
	}
}
