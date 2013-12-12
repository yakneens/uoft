SELECT AVG(attendance) AS avgAttendance \
FROM Teams \
GROUP BY yearID \
ORDER BY avgAttendance DESC

 
