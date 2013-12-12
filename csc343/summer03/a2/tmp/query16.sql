SELECT DISTINCT f.franchName \
FROM Franchise f, Teams t \
WHERE t.franchID = f.franchID \
AND NOT EXISTS (SELECT * FROM Franchise, Teams t1 \
WHERE f.franchID = t1.franchID \
AND t.teamID <> t1.teamID)
