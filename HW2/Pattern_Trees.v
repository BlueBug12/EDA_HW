
module pattern_tree1 (input IN, output OUT);
NOT A(OUT, IN);
cost(OUT)=1;
endmodule


module pattern_tree2 (input IN, output OUT);
NOT A(a, IN);
NOT B(OUT, a);
cost(OUT)=0;
endmodule


module pattern_tree3 (input a1, a2, output OUT);
NAND A(OUT, a1, a2);
cost(OUT)=2;
endmodule


module pattern_tree4 (input a1, a2, c1, output OUT);
NAND A(a, a1, a2);
NOT B(b, a);
NAND C(OUT, b, c1);
cost(OUT)=3;
endmodule


module pattern_tree5 (input a1, a2, b1, output OUT);
NAND A(a, a1, a2);
NOT B(b, b1);
NAND C(c, a, b);
NOT B(OUT, c);
cost(OUT)=3;
endmodule


module pattern_tree6 (input a1, b1, d1, output OUT);
NOT A(a, a1);
NOT B(b, b1);
NAND C(c, a, b);
NAND D(OUT, c, d1);
cost(OUT)=3;
endmodule


module pattern_tree7 (input a1, a2, b1, b2, output OUT);
NAND A(a, a1, a2);
NAND B(b, b1, b2);
NAND C(c, a, b);
NOT D(OUT, c);
cost(OUT)=4;
endmodule
