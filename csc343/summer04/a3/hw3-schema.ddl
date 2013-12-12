drop table coach
drop table country
drop table player
drop table team
drop table game
drop table stadium

create table country ( \
    ctName varchar(12) not null, \
    capital varchar(12) not null, \
    population int not null, \
    primary key (ctName) )

create table team ( \
    tName varchar(12) not null, \
    color varchar(10) not null, \
    group char(1) not null, \
    primary key (tName), \
    foreign key (tName) references country(ctName) on delete cascade )

-- The only possible values for the group attribute are the following:
-- A, B, C, and D.

create table player ( \
    pID int not null, \
    name varchar(20) not null, \
    tName varchar(12) not null, \
    position varchar(10) not null, \
    goals# int not null, \
    primary key (pID), \
    foreign key (tName) references team on delete cascade )

create table coach ( \
    cID int not null, \
    name varchar(20) not null, \
    tName varchar(12) not null, \
    ctName varchar(12) not null, \
    primary key (cID), \
    foreign key (tName) references team on delete cascade, \
    foreign key (ctName) references country on delete cascade )

create table stadium ( \
    name varchar(12) not null, \
    capacity int not null, \
    location varchar(12) not null, \
    primary key (name) )

create table game ( \
    team1 varchar(12) not null, \
    team2 varchar(12) not null, \
    date DATE not null, \
    result varchar(4) not null, \
    score1 int not null, \
    score2 int not null, \
    stage char(1) not null, \
    stadium varchar(12) not null, \
    primary key (team1, team2, date), \
    foreign key (stadium) references stadium(name) on delete cascade, \ 
    foreign key (team1) references team(tName) on delete cascade, \
    foreign key (team2) references team(tName) on delete cascade )

-- score1 is for team1
-- score2 is for team2

-- result is for team1, and its values are win, lost or draw.

-- The only possible values for the stage attribute are the following:
-- C, which stands for qualifications
-- Q, which stands for quarters
-- S, which stands for semifinals, and
-- F, which stands for the final.

