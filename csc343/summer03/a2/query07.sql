SELECT DISTINCT p.nameFirst, p.nameLast, s.salary \
FROM Players p, Salaries s \
WHERE p.playerID = s.playerID  \
AND s.salary = (SELECT MAX(salary) \
								FROM Salaries)
