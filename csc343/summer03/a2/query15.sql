SELECT DISTINCT CONCAT(CONCAT(p.nameFirst, ' '), p.nameLast) AS name \
FROM Players p, Teams t, Salaries s \
WHERE managerID IS NOT NULL AND LENGTH(RTRIM(LTRIM(managerID))) > 0 \
AND p.playerID = s.playerID AND t.teamID = s.teamID and t.yearID = s.yearID \
AND t.WSWin = 'Y' 
