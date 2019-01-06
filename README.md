## Jajavava

Lexer, LR(1) Parser, Interpreter, Symatic Analyser for MinJava Language.

Project Github: https://github.com/Totoro97/Jajavava

#### Authors: 

Lexer, Parser, Interpreter: WANG Peng https://github.com/totoro97

Symatic Analyser: ZHOU Lin https://github.com/linyuzhou 

#### Platform:

Pure C++. Tested in Windows10 with MSVC. Haven't test it with GCC.

#### Usage:

Step 1. Compile it from CMakeLists.txt, and you will get a executable file "main".

Step 2. To test your MinJava Programme: Rename your MinJava file as "test.java", and place it at the folder where "main" exists. Then run "main".

Step 3. You will see the result in the standard output stream.

#### Example:

Example 1:

test.java:

```java
class Factorial{
    public static void main(String[] a){
	System.out.println(new Fac().ComputeFac(10));
    }
}

class Fac {
    public int ComputeFac(int num){
	int num_aux ;
	if (num < 1)
	    num_aux = 1 ;
	else 
	    num_aux = num * (this.ComputeFac(num-1)) ;
	return num_aux ;
    }
}
```

output:

```
---------------------------- Lexer -----------------------------------

yes
n = 292

---------------------------- Parser ----------------------------------

Construct Parser: Begin
Add Rule: Done
Construct Parser: Done
yes
GOAL
|  MAIN_CLASS
|  |  ID Factorial
|  |  ID a
|  |  STATEMENT_PRINT
|  |  |  EXPRESSION_FUNCTION
|  |  |  |  EXPRESSION_NEW_CLASS
|  |  |  |  |  NEW new
|  |  |  |  |  ID Fac
|  |  |  |  ID ComputeFac
|  |  |  |  PARAS
|  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  INT_LITERAL 10
|  CLASS_DECLARATION
|  |  ID Fac
|  |  METHOD_DECLARATION
|  |  |  TYPE_INT
|  |  |  |  INT int
|  |  |  ID ComputeFac
|  |  |  TYPE_ID_MANY
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID num
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID num_aux
|  |  |  STATEMENT_IF_ELSE
|  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  ID num
|  |  |  |  |  BIOP <
|  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  INT_LITERAL 1
|  |  |  |  STATEMENT_EQ
|  |  |  |  |  ID num_aux
|  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  INT_LITERAL 1
|  |  |  |  STATEMENT_EQ
|  |  |  |  |  ID num_aux
|  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  ID num
|  |  |  |  |  |  BIOP *
|  |  |  |  |  |  EXPRESSION_FUNCTION
|  |  |  |  |  |  |  EXPRESSION_THIS
|  |  |  |  |  |  |  |  THIS this
|  |  |  |  |  |  |  ID ComputeFac
|  |  |  |  |  |  |  PARAS
|  |  |  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID num
|  |  |  |  |  |  |  |  |  BIOP -
|  |  |  |  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  |  |  |  INT_LITERAL 1
|  |  |  RETURN return
|  |  |  EXPRESSION_ID
|  |  |  |  ID num_aux

----------------------- Aemantic Analysis ----------------------------

yes

------------------------- Executed Result ----------------------------

MiniJavaOutput >> 3628800

```



Example 2:

test.java:

```java
class BubbleSort{
    public static void main(String[] a){
	System.out.println(new BBS().Start(10));
    }
}


// This class contains the array of integers and
// methods to initialize, print and sort the array
// using Bublesort
class BBS{

    int[] number ;
    int size ;

    // Invoke the Initialization, Sort and Printing
    // Methods
    public int Start(int sz){
	int aux01 ;
	aux01 = this.Init(sz);
	aux01 = this.Print();
	System.out.println(99999);
	aux01 = this.Sort();
	aux01 = this.Print();
	return 0 ;
    }


    // Sort array of integers using Bublesort method
    public int Sort(){
	int nt ;
	int i ;
	int aux02 ;
	int aux04 ;
	int aux05 ;
	int aux06 ;
	int aux07 ;
	int j ;
	int t ;
	i = size - 1 ;
	aux02 = 0 - 1 ;
	while (aux02 < i) {
	    j = 1 ;
	    //aux03 = i+1 ;
	    while (j < (i+1)){
		aux07 = j - 1 ;
		aux04 = number[aux07] ;
		aux05 = number[j] ;
		if (aux05 < aux04) {
		    aux06 = j - 1 ;
		    t = number[aux06] ;
		    number[aux06] = number[j] ;
		    number[j] = t;
		}
		else nt = 0 ;
		j = j + 1 ;
	    }
	    i = i - 1 ;
	}
	return 0 ;
    }

    // Printing method
    public int Print(){
	int j ;
	j = 0 ;
	while (j < (size)) {
	    System.out.println(number[j]);
	    j = j + 1 ;
	}
	return 0 ;
    }

    // Initialize array of integers
    public int Init(int sz){
	size = sz ;
	number = new int[sz] ;

	number[0] = 20 ;
	number[1] = 7  ;
	number[2] = 12 ;
	number[3] = 18 ;
	number[4] = 2  ;
	number[5] = 11 ;
	number[6] = 6  ;
	number[7] = 9  ;
	number[8] = 19 ;
	number[9] = 5  ;

	return 0 ;
    }

}
```



output:

```
---------------------------- Lexer -----------------------------------

yes
n = 1556

---------------------------- Parser ----------------------------------

Construct Parser: Begin
Add Rule: Done
Construct Parser: Done
yes
GOAL
|  MAIN_CLASS
|  |  ID BubbleSort
|  |  ID a
|  |  STATEMENT_PRINT
|  |  |  EXPRESSION_FUNCTION
|  |  |  |  EXPRESSION_NEW_CLASS
|  |  |  |  |  NEW new
|  |  |  |  |  ID BBS
|  |  |  |  ID Start
|  |  |  |  PARAS
|  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  INT_LITERAL 10
|  CLASS_DECLARATION
|  |  ID BBS
|  |  VAR_DECLARATION
|  |  |  TYPE_ARRAY
|  |  |  |  INT int
|  |  |  |  LSQR [
|  |  |  |  RSQR ]
|  |  |  ID number
|  |  VAR_DECLARATION
|  |  |  TYPE_INT
|  |  |  |  INT int
|  |  |  ID size
|  |  METHOD_DECLARATION
|  |  |  TYPE_INT
|  |  |  |  INT int
|  |  |  ID Start
|  |  |  TYPE_ID_MANY
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID sz
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID aux01
|  |  |  STATEMENT_EQ
|  |  |  |  ID aux01
|  |  |  |  EXPRESSION_FUNCTION
|  |  |  |  |  EXPRESSION_THIS
|  |  |  |  |  |  THIS this
|  |  |  |  |  ID Init
|  |  |  |  |  PARAS
|  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  ID sz
|  |  |  STATEMENT_EQ
|  |  |  |  ID aux01
|  |  |  |  EXPRESSION_FUNCTION
|  |  |  |  |  EXPRESSION_THIS
|  |  |  |  |  |  THIS this
|  |  |  |  |  ID Print
|  |  |  STATEMENT_PRINT
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 99999
|  |  |  STATEMENT_EQ
|  |  |  |  ID aux01
|  |  |  |  EXPRESSION_FUNCTION
|  |  |  |  |  EXPRESSION_THIS
|  |  |  |  |  |  THIS this
|  |  |  |  |  ID Sort
|  |  |  STATEMENT_EQ
|  |  |  |  ID aux01
|  |  |  |  EXPRESSION_FUNCTION
|  |  |  |  |  EXPRESSION_THIS
|  |  |  |  |  |  THIS this
|  |  |  |  |  ID Print
|  |  |  RETURN return
|  |  |  EXPRESSION_INT
|  |  |  |  INT_LITERAL 0
|  |  METHOD_DECLARATION
|  |  |  TYPE_INT
|  |  |  |  INT int
|  |  |  ID Sort
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID nt
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID i
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID aux02
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID aux04
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID aux05
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID aux06
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID aux07
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID j
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID t
|  |  |  STATEMENT_EQ
|  |  |  |  ID i
|  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  ID size
|  |  |  |  |  BIOP -
|  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  INT_LITERAL 1
|  |  |  STATEMENT_EQ
|  |  |  |  ID aux02
|  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  INT_LITERAL 0
|  |  |  |  |  BIOP -
|  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  INT_LITERAL 1
|  |  |  STATEMENT_WHILE
|  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  ID aux02
|  |  |  |  |  BIOP <
|  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  ID i
|  |  |  |  STATEMENT_S
|  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  ID j
|  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  INT_LITERAL 1
|  |  |  |  |  STATEMENT_WHILE
|  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  BIOP <
|  |  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  ID i
|  |  |  |  |  |  |  |  BIOP +
|  |  |  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  |  |  INT_LITERAL 1
|  |  |  |  |  |  STATEMENT_S
|  |  |  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  |  |  ID aux07
|  |  |  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  |  |  BIOP -
|  |  |  |  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  |  |  |  INT_LITERAL 1
|  |  |  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  |  |  ID aux04
|  |  |  |  |  |  |  |  EXPRESSION_INDEX
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID number
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID aux07
|  |  |  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  |  |  ID aux05
|  |  |  |  |  |  |  |  EXPRESSION_INDEX
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID number
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  STATEMENT_IF_ELSE
|  |  |  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID aux05
|  |  |  |  |  |  |  |  |  BIOP <
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID aux04
|  |  |  |  |  |  |  |  STATEMENT_S
|  |  |  |  |  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  |  |  |  |  ID aux06
|  |  |  |  |  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  |  |  |  |  BIOP -
|  |  |  |  |  |  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  |  |  |  |  |  INT_LITERAL 1
|  |  |  |  |  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  |  |  |  |  ID t
|  |  |  |  |  |  |  |  |  |  EXPRESSION_INDEX
|  |  |  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  |  |  ID number
|  |  |  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  |  |  ID aux06
|  |  |  |  |  |  |  |  |  STATEMENT_ARRAY
|  |  |  |  |  |  |  |  |  |  ID number
|  |  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  |  ID aux06
|  |  |  |  |  |  |  |  |  |  EXPRESSION_INDEX
|  |  |  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  |  |  ID number
|  |  |  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  |  |  STATEMENT_ARRAY
|  |  |  |  |  |  |  |  |  |  ID number
|  |  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  |  ID t
|  |  |  |  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  |  |  |  ID nt
|  |  |  |  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  |  |  |  INT_LITERAL 0
|  |  |  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  |  |  BIOP +
|  |  |  |  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  |  |  |  INT_LITERAL 1
|  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  ID i
|  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  ID i
|  |  |  |  |  |  |  BIOP -
|  |  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  |  INT_LITERAL 1
|  |  |  RETURN return
|  |  |  EXPRESSION_INT
|  |  |  |  INT_LITERAL 0
|  |  METHOD_DECLARATION
|  |  |  TYPE_INT
|  |  |  |  INT int
|  |  |  ID Print
|  |  |  VAR_DECLARATION
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID j
|  |  |  STATEMENT_EQ
|  |  |  |  ID j
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 0
|  |  |  STATEMENT_WHILE
|  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  ID j
|  |  |  |  |  BIOP <
|  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  ID size
|  |  |  |  STATEMENT_S
|  |  |  |  |  STATEMENT_PRINT
|  |  |  |  |  |  EXPRESSION_INDEX
|  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  ID number
|  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  ID j
|  |  |  |  |  STATEMENT_EQ
|  |  |  |  |  |  ID j
|  |  |  |  |  |  EXPRESSION_BIOP
|  |  |  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  |  |  ID j
|  |  |  |  |  |  |  BIOP +
|  |  |  |  |  |  |  EXPRESSION_INT
|  |  |  |  |  |  |  |  INT_LITERAL 1
|  |  |  RETURN return
|  |  |  EXPRESSION_INT
|  |  |  |  INT_LITERAL 0
|  |  METHOD_DECLARATION
|  |  |  TYPE_INT
|  |  |  |  INT int
|  |  |  ID Init
|  |  |  TYPE_ID_MANY
|  |  |  |  TYPE_INT
|  |  |  |  |  INT int
|  |  |  |  ID sz
|  |  |  STATEMENT_EQ
|  |  |  |  ID size
|  |  |  |  EXPRESSION_ID
|  |  |  |  |  ID sz
|  |  |  STATEMENT_EQ
|  |  |  |  ID number
|  |  |  |  EXPRESSION_NEW_ARRAY
|  |  |  |  |  INT int
|  |  |  |  |  EXPRESSION_ID
|  |  |  |  |  |  ID sz
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 0
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 20
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 1
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 7
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 2
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 12
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 3
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 18
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 4
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 2
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 5
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 11
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 6
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 6
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 7
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 9
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 8
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 19
|  |  |  STATEMENT_ARRAY
|  |  |  |  ID number
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 9
|  |  |  |  EXPRESSION_INT
|  |  |  |  |  INT_LITERAL 5
|  |  |  RETURN return
|  |  |  EXPRESSION_INT
|  |  |  |  INT_LITERAL 0

----------------------- Aemantic Analysis ----------------------------

yes

------------------------- Executed Result ----------------------------

MiniJavaOutput >> 20
MiniJavaOutput >> 7
MiniJavaOutput >> 12
MiniJavaOutput >> 18
MiniJavaOutput >> 2
MiniJavaOutput >> 11
MiniJavaOutput >> 6
MiniJavaOutput >> 9
MiniJavaOutput >> 19
MiniJavaOutput >> 5
MiniJavaOutput >> 99999
MiniJavaOutput >> 2
MiniJavaOutput >> 5
MiniJavaOutput >> 6
MiniJavaOutput >> 7
MiniJavaOutput >> 9
MiniJavaOutput >> 11
MiniJavaOutput >> 12
MiniJavaOutput >> 18
MiniJavaOutput >> 19
MiniJavaOutput >> 20
MiniJavaOutput >> 0
```



