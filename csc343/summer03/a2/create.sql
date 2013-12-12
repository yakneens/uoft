
create table Players ( \ 
       playerId VARCHAR(64) not null primary key, \ 
       managerId VARCHAR(64), \ 
       birthYear integer, \ 
       birthMonth integer, \ 
       birthDay integer, \ 
       birthCountry VARCHAR(64), \ 
       birthState VARCHAR(64), \ 
       birthCity VARCHAR(64), \ 
       deathYear integer, \ 
       deathMonth integer, \ 
       deathDay integer, \ 
       deathCountry VARCHAR(64), \ 
       deathState VARCHAR(64), \ 
       deathCity VARCHAR(64),  \ 
       nameFirst VARCHAR(64), \ 
       nameLast VARCHAR(64), \ 
       bath CHAR, \ 
       throws CHAR, \ 
       debut date \ 
)


create table Franchise ( \ 
       franchID CHAR(3) not null primary key, \ 
       franchName VARCHAR(64) \ 
)

create table Teams ( \ 
       yearID integer not null, \ 
       lgID CHAR(2), \ 
       teamID CHAR(4) not null, \ 
       franchID CHAR(3), \ 
       divID CHAR(1), \ 
       Rank integer, \ 
       W integer, \ 
       L integer, \ 
       DivWin CHAR, \ 
       WCWin CHAR, \ 
       LgWin CHAR, \ 
       WSWin CHAR, \ 
       name VARCHAR(64), \ 
       park VARCHAR(64), \ 
       attendance integer, \ 
       primary key (yearID, teamID), \ 
       foreign key (franchID) references Franchise \ 
)

create table Salaries ( \ 
       yearID integer not null, \ 
       teamID CHAR(3) not null, \ 
       lgId CHAR(2) not null, \ 
       playerID VARCHAR(64) not null, \ 
       salary integer not null, \ 
       primary key (yearID, teamID, playerID), \ 
       foreign key (playerID) references Players, \ 
       foreign key (yearID, teamID) references Teams \ 
)
