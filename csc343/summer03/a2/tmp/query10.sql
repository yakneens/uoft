SELECT p.nameFirst, p.nameLast, SUM(salary) AS totalEarned \
FROM Players p, Salaries s \
WHERE p.playerID = s.playerID \
GROUP BY s.playerID, p.nameFirst, p.nameLast \
HAVING SUM(salary) > 70000000
