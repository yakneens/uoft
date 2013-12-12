SELECT name, yearID \
FROM Teams \
WHERE WSWin = 'Y' \
GROUP BY yearID, W, name  \
HAVING W = MAX(W)
