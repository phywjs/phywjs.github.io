(* Predicates for algebraic axioms on finite set with operation op.  
   op is passed in as a pure function or a name of associative array. *)

(* Closeness of the operation op to the set *)
closeQ[set_, op_] := Module[{n,i,j,a,b,c},
   n = Length[set];
   For[i = 1, i <= n, ++i,
      a = set[[i]];
      For[j = 1, j <= n, ++j,
         b = set[[j]];
         c = a ~op~ b;                          (* every a*b belongs to set *)
         If[!MemberQ[set, c], Return[False]]
      ] 
   ];
   True 
]

(* Associativity *)
associativeQ[set_, op_] := Module[{n, i,j,k, a,b,c, left, right},
   n = Length[set];                               (* cardinality of the set *)
   For[i=1, i<=n, ++i,                       (* for each a, b, c in the set *)
      a = set[[i]];                           (* check if (a.b).c = a.(b.c) *)
      For[j=1, j<=n, ++j,              (* multiplication . is defined by op *)
         b = set[[j]];
         For[k=1, k<=n, ++k,
           c = set[[k]];
           If[ ((a ~op~ b) ~op~ c) =!=                           (* (a.b).c *)
                (a ~op~ (b ~op~ c)),                             (* a.(b.c) *)
               Return[False]]                    (* return False as soon as *)
         ]                                      (* the equation is not true *)
      ]
   ];                                            (* if it has not returned, *)
   True                                       (* then associativity is True *)
]

(* Commutativity *)
commutativeQ[set_, op_] := Module[{n, a, b, i, j},
   n = Length[set];                               (* cardinality of the set *)
   For[i=1, i<=n, ++i,                          (* for each a, b in the set *)
      a = set[[i]];                               (* check if (a.b) = (a.b) *)
      For[j=1, j<=n, ++j,              
         b = set[[j]];
         If[(a ~op~ b) =!= (b ~op~ a), 
            Return[False]]
      ]
   ];                                            (* if it has not returned, *)
   True                                       (* then associativity is True *)
]


(* Identity Exist?, return the identity or False *)
identityQ[set_, op_] := Module[{n, e, ok, i, j, a},
   n = Length[set];
   For[i=1, i<=n, ++i,                            (* test each for identity *)
      e = set[[i]];                           (* check if (a.e) = (e.a) = a *)
      For[j=1, j<=n, ++j,              
         a = set[[j]];
         ok = ((a ~op~ e) === a) && ((e ~op~ a) === a);
         If[!ok, Break[]]                          (* exit j-loop if not OK *)
      ];
      If[ok, Break[]]                         (* identity find, exit i-loop *)
   ];                                            (* if it has not returned, *)
   If[ok, e, False]                           (* then associativity is True *)
]

(* Inverse exist for all element? with respect operation op and identity e *)
(* Return a list of pair of element and its inverse (if exist) *)
inverseQ[set_, op_, e_] := Module[ {n,res,a,b,ok,i,j},
   n = Length[set];
   res = {};
   Do[ a = set[[i]];
      For[j = 1, j <= n, ++j,
         b = set[[j]];                             (* inverse is a*b=b*a=e *)
         ok = ((a ~op~ b) === e) && ((b ~op~ a) === e);
         If[ok, Break[]]                   (* quit looping if find inverse *)
      ];
      res = If[ok, Append[res, {a,b}], Append[res, {a, Null}]]
   , {i, n}];                                  (* i varies from 1 to n *)
   res
]


(*  The final question, is the set with op operation a group? *)
(*  A group is a set with operation op which (0) is closed, 
    and (1) is associative, and (2) identity element exists, 
    and (3) every element has an inverse.
    For example the set {0,1,2,3} with addition modulo 4 is a group,
    we can test as groupQ[{0,1,2,3}, Mod[#1+#2,4]&].  Similarly
    the set {1,2,3,4} with multiplication modulo 5 is a group.
*)
groupQ[set_, op_] := Module[{e},
   e = identityQ[set,op];              (* A group is *)
   closeQ[set,op] &&                   (* closed with respect to op[a,b] *)
   associativeQ[set, op] &&            (* associative a*(b*c) = (a*b)*c  *)
   (e =!= False) &&                    (* identity exists, a*e = e*a = a *)
   FreeQ[inverseQ[set,op,e], Null]  (* inverse exists, a*a^-1=a^-1*a = e *)
]

