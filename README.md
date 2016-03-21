# EBNFDataGenerator
Simple tool for generate random texts by EBNF rule. 
Example of EBNF syntax https://en.wikipedia.org/wiki/Extended_Backus-Naur_Form
supported symbols:

definition		=
concatenation	,
termination	 	;
termination	 	.
alternation		|
optional		[ ... ]
repetition		{ ... }
grouping		( ... )
terminal string	" ... "
comment			# ... #


To build use cmake or QtCreator;


example of use in example.cpp

cd .../EBNFDataGenerator/src
[1]
mkdir build
cd build
cmake ..
make (or run visual studio)
[2]
open qt creator pro file .pro in EBNFDataGenerator/src

to build tests 
cd .../EBNFDataGenerator/tests
and [1]

to build base use 
cd .../EBNFDataGenerator
and [1]

simple build in CMakeFiles.txt in root directory creates programm that read ebnf from input, settings from settings and store results to output
settings file each string must contain single command one of

probabilities <string:tokenName> <number:operationsNumber> {<number:probability>}
#set probabilities of adding elements in <tokenName> tree in <operationsNumber> or operations (1|2|3|...) to <probability> respectively

probability <string:tokenName> <number:operationsNumber> <number:probability>
#set probabilities of adding elements in <tokenName> tree in <operationsNumber> [] operations to <probability>

main <tokenName> 
#set top token name(default = grammar)

dictionary <string:tokenName> "set" {<string:word>}
#set dictionary of <word> to token name <tokenName> (tokenName = "all" sets dictionary to all tokens) in this mode spaces cant be used
dictionary <string:tokenName> "load" <string:filename>
#loads dictionary from <filename> to token name <tokenName> (tokenName = "all" sets dictionary to all tokens)
dictionary <string:tokenName> "pregenerate" <number:count>
#pregenerate dictionary to token name <tokenName>

policy     <string:tokenName>  <string:policyName>
#set policy <policyName> to token name <tokenName> (tokenName = "all" sets policy to all tokens)
#policies:DefaultPolicy,MinMaxDPolicy,MinMaxPolicy<number:min,number:max>,NearAveragePolicy<double:average,double:range[r 0:average*range]>,DictionarySizePolicy<number:multiplier>

#strings begins from '#' - comment string
