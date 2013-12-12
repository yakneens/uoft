SELECT COUNT(s.playerID) AS numPlayers, s.yearID \
FROM Salaries s, Teams t \
WHERE t.teamID = s.teamID AND t.name = 'Toronto Blue Jays' \
AND s.yearID = t.yearID GROUP BY s.yearID
