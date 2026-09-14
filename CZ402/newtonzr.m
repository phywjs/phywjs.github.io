(* Newton method to find zero of a function f[x] *)

NewtonZero[expr_, x_, x0_] := NewtonZero[Function[x,expr], x0]

NewtonZero[f_, x0_] := 
Module[{x, x0p, residue, prec=Precision[x0], fp=f'},
   x0p = SetPrecision[x0, prec + 10];
   x = FixedPoint[(#-f[#]/fp[#])&, x0p, $RecursionLimit];
   residue = f[x];
   If [ Accuracy[residue] - Precision[residue] < prec,
         Print["Did not converge after ", $RecursionLimit, " steps."] ];
   N[x, prec]
]
