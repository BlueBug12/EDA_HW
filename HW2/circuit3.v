module circuit3(input a1, b1, d1, d2, output OUT);

wire a, b, c, d, e, f;

NOT A(a, a1);
NOT B(b, b1);
NOT C(c, a);
NAND D(d, d1, d2);
NAND E(e, b, c);
NAND F(f, d, e);
NOT G(OUT, f);


endmodule
