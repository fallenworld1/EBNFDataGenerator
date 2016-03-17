# EBNFDataGenerator
Simple tool for generate random texts by EBNF rule. 

To build use cmake or QtCreator;

src - build library
tests - build tests(google tests must be instaled and located by find_package() func in case of using cmake)

example of use in example.cpp

simple build in CMakeFiles.txt in root directory creates programm that read ebnf from input, settings from settings and store results to output
settings file each string must contain single command one of

probabilities <string:tokenName> <number:operationsNumber> {<number:probability>}
#set probabilities of adding elements in <tokenName> tree in <operationsNumber> or operations (1|2|3|...) to <probability> respectively

probability <string:tokenName> <number:operationsNumber> <number:probability>
#set probabilities of adding elements in <tokenName> tree in <operationsNumber> [] operations to <probability>

main <tokenName> 
#set top token name(default = grammar)

dictionary <string:tokenName> "set" {<string:word>}
#set dictionary of <word> to token name <tokenName> (tokenName = "all" sets dictionary to all tokens)
dictionary <string:tokenName> "load" <string:filename>
#loads dictionary from <filename> to token name <tokenName> (tokenName = "all" sets dictionary to all tokens)
policy     <string:tokenName>  <string:policyName>
#set policy <policyName> to token name <tokenName> (tokenName = "all" sets policy to all tokens)
#policies:DefaultPolicy,MinMaxDPolicy,MinMaxPolicy,NearAveragePolicy
