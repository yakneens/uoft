SELECT DISTINCT CONCAT(CONCAT(p.nameFirst, ' '), p.nameLast) AS name \
FROM Players p, Teams t, Salaries s \
WHERE p.playerID = s.playerID AND t.teamID = s.teamID AND t.yearID = s.yearID \
GROUP BY t.WSWin, p.nameFirst, p.nameLast \
HAVING COUNT(t.WSWin) >= ALL \
(SELECT COUNT(t1.WSWin) AS numSeries \
FROM Players p1, Teams t1, Salaries s1 \
WHERE p1.playerID = s1.playerID AND t1.teamID = s1.teamID \
AND t1.yearID = s1.yearID \
GROUP BY t1.WSWin, p1.nameFirst, p1.nameLast)
