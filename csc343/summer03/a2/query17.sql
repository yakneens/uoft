SELECT DISTINCT CONCAT(CONCAT(nameFirst, ' '), nameLast) AS name \
FROM Players \
WHERE playerID IN \
(SELECT p1.playerID \
FROM Players p1, Teams t1, Salaries s1 \
WHERE p1.playerID = s1.playerID AND t1.teamID = s1.teamID \
AND t1.yearID = s1.yearID AND t1.WSWin = 'Y' AND t1.lgID = 'AL') \
AND playerID IN \
(SELECT p2.playerID \
FROM Players p2, Teams t2, Salaries s2 \
WHERE p2.playerID = s2.playerID AND t2.teamID = s2.teamID \
AND t2.yearID = s2.yearID AND t2.WSWin = 'Y' AND t2.lgID = 'NL') 
