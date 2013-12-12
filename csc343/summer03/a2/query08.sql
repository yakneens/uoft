SELECT p.nameFirst, p.nameLast, t.name, t.yearID \
FROM Players p, Teams t, Salaries s \
WHERE t.WSWin = 'Y' AND t.LgWin = 'Y' AND DivWin = 'Y' \
AND p.playerID = s.playerID AND t.teamID = s.teamID \
AND t.yearID = s.yearID AND p.bath = 'B' AND p.throws = 'L'
