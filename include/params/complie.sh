flex++ param.l;
bison -d param.ypp
g++ param.tab.cpp  lex.yy.cc main.cpp -o main
