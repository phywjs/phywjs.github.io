(* Data should be in the form {{x1,y1}, {x2, y2}, ... }, i.e, an n by 2
   matrix;  f is a list of linearly independent basis functions, and 
   x is the variable.  For example
      leastquarefit[{{1,2}, {3.4, 5.6}, {7.2, 8.2}}, {1, x}, x]
   fit the data to a straight line.
*)
leastsquarefit[data_List, f_List, x_Symbol] := 
  Module[{xv, yv, A, At, c}, 
    {xv, yv} = Transpose[data]; 
    A = f /. (({x -> #1} &) /@ xv); 
    At = Transpose[A]; 
    c = LinearSolve[At . A, At . yv];
    c . f
]
