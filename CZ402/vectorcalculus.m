(* Vector calculus examples *)
(* f for scale, v for vector, arg is a list of coordiates .e.g, {x,y,z} *)

Grad[f_, arg_] := D[f, #]& /@ arg                              (* Gradient *)
Div[v_, arg_] := Inner[D,v,arg,Plus]                         (* Divergence *)
Laplacian[f_, arg_] := Div[Grad[f,arg],arg]    
Jocobian[v_, arg_] := Outer[D, v,arg]
Tripleproduct[a_,b_,c_] := Det[{a,b,c}]                     (* a . (b x c) *)
Crossproduct[a_,b_] := {Det[{{1,0,0}, a, b}],                     (* a x b *)
                        Det[{{0,1,0}, a, b}],          (* apply to 3d only *)
                        Det[{{0,0,1}, a, b}]}  

