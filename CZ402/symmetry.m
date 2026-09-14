(* Program for computing symmetry number of a graph *)
Clear[vertices, edges, pair, SymmetryNumber]

SetAttributes[vertices,Orderless]
SetAttributes[edges,Orderless]
SetAttributes[pair,Orderless]

(* A graph is represented as a set of vertices, and set of edges; each edge
  is a set of two vertices. *)
(* here is a o--o *)
g1 = graph[vertices[a,b], edges[pair[a,b]]]

(* here is a o--o--o *)
g11 = graph[vertices[a,b,c], edges[pair[a,b],pair[b,c]]]

(* here is o--o *)
(*         |  | *)
(*         o--o *)
g2 = graph[vertices[a,b,c,d], edges[pair[a,b],pair[b,c],pair[c,d],pair[d,a]]]

(* here is o---o *)
(*         | \ | *)
(*         o---o *)
g3 = graph[vertices[a,b,c,d], 
           edges[pair[a,b],pair[b,c],pair[c,d],pair[d,a],pair[a,c]]]

(* here is o---o *)
(*         | X | *)
(*         o---o *)
g4 = graph[vertices[a,b,c,d], 
           edges[pair[a,b],pair[b,c],pair[c,d],pair[d,a],pair[a,c],pair[b,d]]]

(* here is o    *)
(*         | \  *)
(*         o--o *)
g5 = graph[vertices[a,b,c], edges[pair[a,b],pair[b,c],pair[c,a]]]

(* here is  o--o--o--o *)
g6 = graph[vertices[a,b,c,d], edges[pair[a,b],pair[b,c],pair[c,d]]]

(* here is  o--o--o--o--o *)
g7 = graph[vertices[a,b,c,d,e], edges[pair[a,b],pair[b,c],pair[c,d],pair[d,e]]]

(* here is     o     *)
(*             |     *)
(*          o--o--o  *)
(*             |     *)
(*             o     *)
g8 = graph[vertices[a,b,c,d,e], edges[pair[a,b],pair[a,c],pair[a,d],pair[a,e]]]

SymmetryNumber[g_graph] := Module[ {v, prules, pgraphs}, 
   v = List @@ First[g];                               (* List of vertices *)
   prules = Thread[(v -> #)] & /@ Permutations[v];    (* permute rule list *)
   pgraphs = Union[g /. prules];     (* List of permuted, canonical graphs *)
   Length[pgraphs]                        (* symmetry number of the graphs *)
]

