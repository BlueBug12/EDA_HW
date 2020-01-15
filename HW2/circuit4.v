module circuit4 (input a1, b1, c1, d1, e1, e2, f1, output OUT);

wire a, b, c, d, e, f, g, h, i, j, k, l, m, n, o;

NOT A(a, a1);
NOT B(b, b1);
NOT C(c, c1);
NOT D(d, d1);
NAND E(e, e1, e2);
NOT F(f, f1);
NAND G(g, a, b);
NAND H(h, c, d);
NOT I(i, e);
NOT J(j, g);
NOT K(k, h);
NOT L(l, i);
NAND M(m, j, k);
NAND N(n, m, l);
NAND O(o, n, f);
NOT P(OUT, o);


endmodule
