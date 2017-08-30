# Bob-the-Compiler
Bob the Compiler, can he compile it? Bob the Compiler, YES HE CAN!

##Compile
To build the compiler, type in Terminal
>`g++ lex.cpp parse.cpp cst.cpp semantic.cpp code_gen.cpp bobc.cpp -o bobc`


Usage:
>`./bobc [option] testCode.txt`

##Grammar

|Non-Terminals|Terminals|
|---|---|
|Program			|Block **$**|
|Block 				|**{** StatementList **}**|
|StatementList		|Statement StatementList|
|					|Ø|
|Statement  		|PrintStatement|
|					|AssignmentStatement|
|					|VarDecl|
|					|WhileStatement|
|					|IfStatement|
|					|Block|
|PrintStatement 	|**print (** Expr **)**|
|AssignmentStatement|Id **=** Expr|
|VarDecl			|type Id|
|WhileStatement		|**while** BooleanExpr Block|
|IfStatement  		|**if** BooleanExpr Block|
|Expr  				|IntExpr|
|					|StringExpr|
|					|BooleanExpr|
|					|Id|
|IntExpr			|digit intop Expr|
|					|digit|
|StringExpr			|**"** CharList **"**|
|BooleanExpr		|**(** Expr boolop Expr **)**|
|					|boolval|
|Id  				|char|
|CharList			|char CharList|
|					|Ø|
|type				|**int** : **string** : **boolean**|
|char  				|**a** : **b** : **c ... z**|
|space				|the **space** character|
|digit				|**1** : **2** : **3** : **4** : **5** : **6** : **7** : **8** : **9** : **0**|
|boolop				|**==** : **!=**|
|boolval			|**true** : **false**|
|intop				|**+**|
