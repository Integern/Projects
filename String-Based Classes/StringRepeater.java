public class StringRepeater {

    public StringBuffer repeatString(String s, int n){
        StringBuffer newStr = new StringBuffer();
        for(int i=0; i<n; i++) {
            newStr.append(s);
        }
        return newStr;
    }


    public String unoptimisedRepeatString(String s, int n){
        String result = "";
        for(int i=0; i<n; i++) {
            result = result + s;
        }
        return result;
    }

}
