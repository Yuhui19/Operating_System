import static java.lang.Long.MAX_VALUE;
import static java.lang.Long.parseLong;

public class BtoI {

    public static void main(String[] args){
        long test =parseLong("0403deadbeef0201",16);
        System.out.println(test);

//        long test2=parseLong("efdebead04030201",16);
//        System.out.println(test2);

        long test3=parseLong("0102030400000000",16);
        System.out.println(test3);

        long test4=parseLong("0403020100000000",16);
        System.out.println(test4);

        System.out.println(MAX_VALUE);
    }
}
