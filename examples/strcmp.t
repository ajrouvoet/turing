16
q1
q2
q3
q4
q5
q6
q7
q8
q9
q10
q11
q12
q13
q14
q_accept A
q_reject R
q1 0 -> q2 x R
q1 1 -> q3 x R
q1 # -> q8 \ R
q2 0 -> q2 \ R
q2 1 -> q2 \ R
q2 # -> q4 \ R
q3 0 -> q3 \ R
q3 1 -> q3 \ R
q3 # -> q5 \ R
q4 0 -> q6 x L
q4 x -> q4 \ R
q5 x -> q5 \ R
q5 1 -> q6 x L
q6 0 -> q6 \ L
q6 1 -> q6 \ L
q6 x -> q6 \ L
q6 # -> q7 \ L
q7 0 -> q7 \ L
q7 1 -> q7 \ L
q7 x -> q1 \ R
q8 x -> q8 \ R
q8 _ -> q_accept \ R
