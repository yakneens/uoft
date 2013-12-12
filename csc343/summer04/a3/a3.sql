--Query 1
WITH g (team1, team2, score1, score2) AS (SELECT team1, team2, score1, score2 FROM game WHERE stage = 'C'), g1 (goals, team1) AS ((SELECT SUM(score1) AS goals, team1 FROM g GROUP BY team1 UNION (SELECT 0 AS goals, tName AS team1 FROM team WHERE tName NOT IN (SELECT team1 FROM g)))), g2 (goals, team2) AS (SELECT SUM(score2) AS goals, team2 FROM g GROUP BY team2 UNION (SELECT 0 AS goals, tName AS team2 FROM team WHERE tName NOT IN (SELECT team2 FROM g))) SELECT g1.team1 AS teamName, (g1.goals + g2.goals) AS goalNum FROM g1, g2 WHERE g1.team1 = g2.team2 AND (g1.goals + g2.goals) = (SELECT MAX(g1.goals + g2.goals) FROM g1, g2 WHERE g1.team1 = g2.team2)

--Query 2
SELECT s.location AS city FROM stadium s, game g WHERE g.stage = 'F' AND g.stadium = s.name

--Query 3
SELECT p.goals#, p.name, p.tName FROM player p WHERE 2 >= (SELECT COUNT(DISTINCT p1.goals#) FROM player p1 WHERE p1.tName = p.tName AND p1.goals# > p.goals#)

--Query 4
SELECT p.position FROM player p GROUP BY p.position HAVING SUM(p.goals#) = (SELECT MAX(p2.goals) FROM (SELECT SUM(goals#) AS goals FROM player p1 GROUP BY p1.position) p2)

--Query 5
SELECT ch.ctName, COUNT(ch.cID) AS numCoaches FROM coach ch WHERE ch.ctName <> ch.tName GROUP BY ch.ctName UNION (SELECT ctName, 0 AS numCoaches FROM country WHERE ctName NOT IN (SELECT ctName FROM coach WHERE ctName <> tName GROUP BY ctName))

--Query 6
SELECT w.tName, (3 * wins + 1 * draws + goals) AS points, goals, wins, draws, losses FROM (SELECT w.tName, SUM(w.wins) AS wins FROM ((SELECT team1 AS tName, COUNT(*) AS wins FROM game WHERE result = 'won' GROUP BY team1 UNION ALL (SELECT team2 AS tName, COUNT(*) AS wins FROM game WHERE result = 'lost' GROUP BY team2)) UNION ALL (SELECT tName, 0 AS wins FROM team WHERE tName NOT IN (SELECT team1 AS tName FROM game WHERE result = 'won' UNION (SELECT team2 AS tName FROM game WHERE result = 'lost')))) w GROUP BY w.tName) w, (SELECT d.tName, SUM(d.draws) AS DRAWS FROM ((SELECT team1 AS tName, COUNT(*) AS draws FROM game WHERE result = 'draw' GROUP BY team1 UNION ALL (SELECT team2 AS tName, COUNT(*) AS draws FROM game WHERE result = 'draw' GROUP BY team2)) UNION ALL (SELECT tName, 0 AS draws FROM team WHERE tName NOT IN (SELECT team1 AS tName FROM game WHERE result = 'draw' UNION (SELECT team2 AS tName FROM game WHERE result = 'draw')))) d GROUP BY d.tName) d, (SELECT l.tName, SUM(l.losses) AS losses FROM ((SELECT team1 AS tName, COUNT(*) AS losses FROM game WHERE result = 'lost' GROUP BY team1 UNION ALL (SELECT team2 AS tName, COUNT(*) AS losses FROM game WHERE result = 'won' GROUP BY team2)) UNION ALL (SELECT tName, 0 AS losses FROM team WHERE tName NOT IN (SELECT team1 AS tName FROM game WHERE result = 'lost' UNION (SELECT team2 AS tName FROM game WHERE result = 'won')))) l GROUP BY l.tName) l, (SELECT p.tName, SUM(goals#) AS goals FROM player p GROUP BY p.tName) g WHERE w.tName = d.tName AND d.tName = l.tName AND l.tName = g.tName ORDER BY points DESC

--Query 7
WITH g1 (goals, tName, group) AS (SELECT SUM(score1) AS goals, team1 AS tName, t.group FROM game g, team t WHERE stage = 'C' AND g.team1 = t.tName GROUP BY team1, t.group UNION (SELECT 0 AS goals, tName, group FROM team WHERE tName NOT IN (SELECT team1 FROM game))), g2 (goals, tName, group) AS (SELECT SUM(score2) AS goals, team2 AS tName, t.group FROM game g, team t WHERE stage = 'C' AND g.team2 = t.tName GROUP BY team2, t.group UNION (SELECT 0 AS goals, tName, group FROM team WHERE tName NOT IN (SELECT team2 FROM game))), g (goalNum, groupName) AS (SELECT max(goalNum) AS goalNum, groupName FROM (SELECT (g1.goals + g2.goals) AS goalNum, g1.group AS groupName FROM g1, g2 WHERE g1.tName = g2.tName) gTemp GROUP BY gTemp.groupName), q (tName) AS (SELECT team1 AS tName FROM game WHERE stage = 'Q' UNION (SELECT team2 AS tName FROM game WHERE stage = 'Q')) SELECT groupName, 'true' AS qualified FROM g1, g2, g WHERE g1.tName = g2.tName AND (g1.goals + g2.goals) = g.goalNum AND g1.group = g.groupName AND g1.tName IN (SELECT * FROM q) UNION (SELECT groupName, 'false' AS qualified FROM g1, g2, g WHERE g1.tName = g2.tName AND (g1.goals + g2.goals) = g.goalNum AND g1.group = g.groupName AND g1.tName NOT IN (SELECT * FROM q))

--Query 8
WITH g (team1, team2) AS (SELECT g1.team1, g2.team2 FROM game g1, game g2 WHERE g1.team2 = g2.team1 AND g1.team1 <> g2.team2 UNION (SELECT g2.team1, g1.team2 FROM game g1, game g2 WHERE g1.team1 = g2.team2 AND g1.team2 <> g2.team1 UNION (SELECT g1.team1, g2.team1 AS team2 FROM game g1, game g2 WHERE g1.team2 = g2.team2 AND g1.team1 <> g2.team1 UNION (SELECT g1.team2 AS team1, g2.team2 FROM game g1, game g2 WHERE g1.team1 = g2.team1 AND g1.team2 <> g2.team2)))) SELECT g1.team1, g1.team2 FROM (SELECT DISTINCT team1, team2 FROM game UNION (SELECT * FROM g)) g1