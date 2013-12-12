SELECT DISTINCT f.franchName \
FROM Franchise f, Teams t \
WHERE f.franchID = t.franchID \
AND t.lgID = 'AL'
