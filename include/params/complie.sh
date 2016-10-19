flex++ param.l;
bison -d param.ypp
g++ param.tab.cpp  lex.yy.cc main.cpp  params.cpp paramConstructor.cpp -o main
