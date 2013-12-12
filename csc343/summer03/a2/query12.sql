SELECT p.playerID FROM Players p, Teams t, Salaries s \
WHERE p.playerID = s.playerID AND s.teamID = t.teamID \ 
AND t.yearID = s.yearID AND t.yearID = 1992 \
AND t.WSWin = 'Y' AND t.name = 'Toronto Blue Jays' \
AND p.playerID IN (SELECT  p1.playerID \
FROM Players p1, Salaries s1, Teams t1 \
WHERE p1.playerID = s1.playerID AND s1.teamID = t1.teamID \
AND t1.yearID = s1.yearID AND t1.yearID = 2002 \
AND t1.name = 'Toronto Blue Jays')
