# EBNFDataGenerator
Simple tool for generate random texts by EBNF rule. 
To make project you can use QtCreator or any other IDE. 
Or just run qmake in src dir.

Default input read from file named "input" and output results stored to "output" in local. Generator trying to generate 150strings of text for 500 attempt(see args of function Parser::generate). If use big rules generating will be slow.

Tests have dependency to QtTest.

!!!Project isn't full ready now and can have bugs.!!!
