SELECT DISTINCT CONCAT(CONCAT(p.nameFirst, ' '), p.nameLast) AS name \
FROM Players p, Salaries s \
WHERE p.playerID = s.playerID AND p.playerID NOT IN \
(SELECT playerID FROM Salaries WHERE salary > 100000)
