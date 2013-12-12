SELECT name, yearID as yearOfCreation \
FROM Teams \
WHERE yearID = (SELECT MIN(yearID) FROM Teams)
