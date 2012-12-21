7
q1
q2
q3
q4
q5
q_accept A
q_reject R
q1 _ -> q_reject \ R
q1 x -> q_reject \ R
q1 0 -> q2 _ R
q2 x -> q2 \ R
q2 _ -> q_accept \ R
q2 0 -> q3 x R
q3 x -> q3 \ R
q3 0 -> q4 \ R
q3 _ -> q5 \ L
q4 0 -> q3 x R
q4 x -> q4 \ R
q4 _ -> q_reject \ R
q5 0 -> q5 \ L
q5 x -> q5 \ L
q5 _ -> q2 \ R
