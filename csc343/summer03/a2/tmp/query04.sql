SELECT DISTINCT CONCAT(CONCAT(nameFirst, ' '), nameLast) AS name \
FROM Players \
WHERE managerID IS NOT NULL \
AND LENGTH(RTRIM(LTRIM(managerID))) > 0
