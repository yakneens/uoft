static char sqla_program_id[162] = 
{
 42,0,65,68,65,74,65,72,65,51,32,32,32,32,32,32,99,65,88,121,
 85,100,72,84,48,49,49,49,49,50,32,32,8,0,71,50,80,79,85,80,
 79,78,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0
};

#include "sqladef.h"

static struct sqla_runtime_info sqla_rtinfo = 
{{'S','Q','L','A','R','T','I','N'}, sizeof(wchar_t), 0, {' ',' ',' ',' '}};


#line 1 "a3.sqc"
/*****************************
 * CSC343 Assignment 3
 * Sergei Iakhnin, 981843420
 * L5101, g2poupon
 *****************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlenv.h>

 
int     exit_flag = 0, valid_choice;
char    menu_choice;


/*
EXEC SQL INCLUDE SQLCA;
*/

/* SQL Communication Area - SQLCA - structures and constants */
#include "sqlca.h"
struct sqlca sqlca;


#line 18 "a3.sqc"
 

/* Host variables to be used in queries */

/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 21 "a3.sqc"

char db_name[8], debut[11], teamID[3],lgID[2], franchID[3];
char playerID[64], managerID[64], birthCountry[64], birthState[64], birthCity[64];
char deathCountry[64], deathState[64], deathCity[64], nameFirst[64], nameLast[64];
char bath, throws, franchName[64], franchName2[64];
long birthYear, birthMonth, birthDay, deathYear, deathMonth, deathDay;
char* qstring;
long result, wins, losses, yearID, yearID2, recExists;
float salary, salary2;
short indManagerID, indBirthCountry, indBirthState, indBirthCity;
short indDeathCountry, indDeathState, indDeathCity, indNameFirst, indNameLast;
short indDebut, indBath, indThrows, indBirthYear, indBirthMonth, indBirthDay;
short indDeathYear, indDeathMonth, indDeathDay;

/*
EXEC SQL END DECLARE SECTION;
*/

#line 34 "a3.sqc"


/*******************************************************************************
 * function doQuery1() performs the query "all available information regarding
 * a user-specified playerID"
 ******************************************************************************/
void doQuery1(){
	
	/* Read user input */
	printf("Enter playerID: ");
	scanf("%s", playerID);
		
	
	/* Create cursor for query */
	
/*
EXEC SQL DECLARE c1 CURSOR FOR SELECT * FROM Players WHERE playerID = :playerID;
*/

#line 48 "a3.sqc"

	
	
/*
EXEC SQL OPEN c1;
*/

{
#line 50 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 50 "a3.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 50 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 64;
#line 50 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)playerID;
#line 50 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 50 "a3.sqc"
      sqlasetda(2,0,1,sql_setdlist,NULL,0L);
    }
#line 50 "a3.sqc"
  sqlacall((unsigned short)26,1,2,0,0L);
#line 50 "a3.sqc"
  sqlastop(0L);
}

#line 50 "a3.sqc"

	
	/* Fetch results using indicator variables to check for nulls */
	
/*
EXEC SQL FETCH c1 into :playerID, :managerID INDICATOR :indManagerID, 
	:birthYear INDICATOR :indBirthYear, :birthMonth INDICATOR :indBirthMonth, 
	:birthDay INDICATOR :indBirthDay, :birthCountry INDICATOR :indBirthCountry, 
	:birthState INDICATOR :indBirthState, :birthCity INDICATOR :indBirthCity, 
	:deathYear INDICATOR :indDeathYear, :deathMonth INDICATOR :indDeathMonth, 
	:deathDay INDICATOR :indDeathDay, :deathCountry INDICATOR :indDeathCountry, 
	:deathState INDICATOR :indDeathState, :deathCity INDICATOR :indDeathCity, 
	:nameFirst INDICATOR :indNameFirst, :nameLast INDICATOR :indNameLast, 
	:bath INDICATOR :indBath, :throws INDICATOR :indThrows, 
	:debut INDICATOR :indDebut;
*/

{
#line 62 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 62 "a3.sqc"
  sqlaaloc(3,19,2,0L);
    {
      struct sqla_setd_list sql_setdlist[19];
#line 62 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)playerID;
#line 62 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 62 "a3.sqc"
      sql_setdlist[1].sqltype = 461; sql_setdlist[1].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)managerID;
#line 62 "a3.sqc"
      sql_setdlist[1].sqlind = &indManagerID;
#line 62 "a3.sqc"
      sql_setdlist[2].sqltype = 497; sql_setdlist[2].sqllen = 4;
#line 62 "a3.sqc"
      sql_setdlist[2].sqldata = (void*)&birthYear;
#line 62 "a3.sqc"
      sql_setdlist[2].sqlind = &indBirthYear;
#line 62 "a3.sqc"
      sql_setdlist[3].sqltype = 497; sql_setdlist[3].sqllen = 4;
#line 62 "a3.sqc"
      sql_setdlist[3].sqldata = (void*)&birthMonth;
#line 62 "a3.sqc"
      sql_setdlist[3].sqlind = &indBirthMonth;
#line 62 "a3.sqc"
      sql_setdlist[4].sqltype = 497; sql_setdlist[4].sqllen = 4;
#line 62 "a3.sqc"
      sql_setdlist[4].sqldata = (void*)&birthDay;
#line 62 "a3.sqc"
      sql_setdlist[4].sqlind = &indBirthDay;
#line 62 "a3.sqc"
      sql_setdlist[5].sqltype = 461; sql_setdlist[5].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[5].sqldata = (void*)birthCountry;
#line 62 "a3.sqc"
      sql_setdlist[5].sqlind = &indBirthCountry;
#line 62 "a3.sqc"
      sql_setdlist[6].sqltype = 461; sql_setdlist[6].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[6].sqldata = (void*)birthState;
#line 62 "a3.sqc"
      sql_setdlist[6].sqlind = &indBirthState;
#line 62 "a3.sqc"
      sql_setdlist[7].sqltype = 461; sql_setdlist[7].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[7].sqldata = (void*)birthCity;
#line 62 "a3.sqc"
      sql_setdlist[7].sqlind = &indBirthCity;
#line 62 "a3.sqc"
      sql_setdlist[8].sqltype = 497; sql_setdlist[8].sqllen = 4;
#line 62 "a3.sqc"
      sql_setdlist[8].sqldata = (void*)&deathYear;
#line 62 "a3.sqc"
      sql_setdlist[8].sqlind = &indDeathYear;
#line 62 "a3.sqc"
      sql_setdlist[9].sqltype = 497; sql_setdlist[9].sqllen = 4;
#line 62 "a3.sqc"
      sql_setdlist[9].sqldata = (void*)&deathMonth;
#line 62 "a3.sqc"
      sql_setdlist[9].sqlind = &indDeathMonth;
#line 62 "a3.sqc"
      sql_setdlist[10].sqltype = 497; sql_setdlist[10].sqllen = 4;
#line 62 "a3.sqc"
      sql_setdlist[10].sqldata = (void*)&deathDay;
#line 62 "a3.sqc"
      sql_setdlist[10].sqlind = &indDeathDay;
#line 62 "a3.sqc"
      sql_setdlist[11].sqltype = 461; sql_setdlist[11].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[11].sqldata = (void*)deathCountry;
#line 62 "a3.sqc"
      sql_setdlist[11].sqlind = &indDeathCountry;
#line 62 "a3.sqc"
      sql_setdlist[12].sqltype = 461; sql_setdlist[12].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[12].sqldata = (void*)deathState;
#line 62 "a3.sqc"
      sql_setdlist[12].sqlind = &indDeathState;
#line 62 "a3.sqc"
      sql_setdlist[13].sqltype = 461; sql_setdlist[13].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[13].sqldata = (void*)deathCity;
#line 62 "a3.sqc"
      sql_setdlist[13].sqlind = &indDeathCity;
#line 62 "a3.sqc"
      sql_setdlist[14].sqltype = 461; sql_setdlist[14].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[14].sqldata = (void*)nameFirst;
#line 62 "a3.sqc"
      sql_setdlist[14].sqlind = &indNameFirst;
#line 62 "a3.sqc"
      sql_setdlist[15].sqltype = 461; sql_setdlist[15].sqllen = 64;
#line 62 "a3.sqc"
      sql_setdlist[15].sqldata = (void*)nameLast;
#line 62 "a3.sqc"
      sql_setdlist[15].sqlind = &indNameLast;
#line 62 "a3.sqc"
      sql_setdlist[16].sqltype = 453; sql_setdlist[16].sqllen = 1;
#line 62 "a3.sqc"
      sql_setdlist[16].sqldata = (void*)&bath;
#line 62 "a3.sqc"
      sql_setdlist[16].sqlind = &indBath;
#line 62 "a3.sqc"
      sql_setdlist[17].sqltype = 453; sql_setdlist[17].sqllen = 1;
#line 62 "a3.sqc"
      sql_setdlist[17].sqldata = (void*)&throws;
#line 62 "a3.sqc"
      sql_setdlist[17].sqlind = &indThrows;
#line 62 "a3.sqc"
      sql_setdlist[18].sqltype = 461; sql_setdlist[18].sqllen = 11;
#line 62 "a3.sqc"
      sql_setdlist[18].sqldata = (void*)debut;
#line 62 "a3.sqc"
      sql_setdlist[18].sqlind = &indDebut;
#line 62 "a3.sqc"
      sqlasetda(3,0,19,sql_setdlist,NULL,0L);
    }
#line 62 "a3.sqc"
  sqlacall((unsigned short)25,1,0,3,0L);
#line 62 "a3.sqc"
  sqlastop(0L);
}

#line 62 "a3.sqc"

		
	/* If player not found. */
	if(SQLCODE == 100){
		printf("Player not found\n");
		
/*
EXEC SQL CLOSE c1;
*/

{
#line 67 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 67 "a3.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 67 "a3.sqc"
  sqlastop(0L);
}

#line 67 "a3.sqc"

		return;
	}else if(SQLCODE != 0){
		return;
	}
	
	/* Print results of the query, while checking for nulls */
	/* For every null print N/A in the place of field value */
	printf("Player ID: %s\n", playerID);
	
	if(indManagerID >= 0){
		printf("Manager ID: %s\n", managerID);
	}else{
		printf("Manageer ID: N/A\n");
	}
	
	if(indBirthDay >= 0 && indBirthMonth > 0 && indBirthYear > 0){
		printf("Date of Birth(dd/mm/yyyy): %ld/%ld/%ld\n", birthDay, birthMonth, birthYear);
	
	}else{
		printf("Date of Birth: N/A\n");
	}
		
	if(indBirthCountry >= 0){
		printf("Country of Birth: %s\n", birthCountry);
	
	}else{
		printf("Country of Birth: N/A\n");
	}
			
	if(indBirthState >= 0){
		printf("State of Birth: %s\n", birthState);
	
	}else{
		printf("State of Birth: N/A\n");
	}
			
	if(indBirthCity >= 0){
		printf("City of Birth: %s\n", birthCity);
	
	}else{
		printf("City of Birth: N/A\n");
	}
			
	if(indDeathDay >= 0 && indDeathMonth >= 0 && indDeathYear >= 0){
		printf("Date of Death(dd/mm/yyy): %ld/%ld/%ld\n", deathDay, deathMonth, deathYear);
		
	}else{
		printf("Date of Death: N/A\n");
	}
			
	if(indDeathCountry >= 0){
		printf("Country of Death %s\n", deathCountry);
			
	}else{
		printf("Country of Death: N/A\n");
	}
			
	if(indDeathState >= 0){
		printf("State of Death: %s\n", deathState);
		
	}else{
		printf("State of Death: N/A\n");
	}
			
	if(indDeathCity >= 0){
		printf("City of Death: %s\n", deathCity);
		
	}else{
		printf("City of Death: N/A\n");
	}
			
	if(indNameFirst >= 0){
		printf("First Name: %s\n", nameFirst);
	
	}else{
		printf("First Name: N/A\n");
	}
			
	if(indNameLast >= 0){
		printf("Last Name: %s\n", nameLast);
		
	}else{
		printf("Last Name: N/A\n");
	}
			
	if(indBath >= 0){
		printf("Batting hand: %c\n", bath);
	}else{
		printf("Batting hand: N/A\n");
	}
			
	if(indThrows >= 0){
		printf("Throwing hand: %c\n", throws);
			
	}else{
		printf("Throwing hand: N/A\n");
	}
			
	if(indDebut >= 0){
		printf("Debut date: %s\n", debut);
			
	}else{
		printf("Debut date: N/A\n");
	}
	
	
/*
EXEC SQL CLOSE c1;
*/

{
#line 173 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 173 "a3.sqc"
  sqlacall((unsigned short)20,1,0,0,0L);
#line 173 "a3.sqc"
  sqlastop(0L);
}

#line 173 "a3.sqc"

		
}

/*******************************************************************************
 * function doQuery2() performs the query "the number of wins and losses per 
 * year for a user-specified playerID"
 ******************************************************************************/
void doQuery2(){
	
	/* Read user input */
	printf("Enter playerID: ");
	scanf("%s", playerID);
		
	
	/* Create cursor for query */
	
/*
EXEC SQL DECLARE c2 CURSOR FOR SELECT t.yearID, t.W, t.L 
	FROM Teams t, Salaries s 
	WHERE s.playerID = :playerID AND s.yearID = t.yearID 
	AND s.teamID = t.teamID ORDER BY yearID ASC;
*/

#line 192 "a3.sqc"

	
	
/*
EXEC SQL OPEN c2;
*/

{
#line 194 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 194 "a3.sqc"
  sqlaaloc(2,1,3,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 194 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 64;
#line 194 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)playerID;
#line 194 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 194 "a3.sqc"
      sqlasetda(2,0,1,sql_setdlist,NULL,0L);
    }
#line 194 "a3.sqc"
  sqlacall((unsigned short)26,2,2,0,0L);
#line 194 "a3.sqc"
  sqlastop(0L);
}

#line 194 "a3.sqc"

	
	/* go through the result set printing retreived values */
	do{
		
/*
EXEC SQL FETCH c2 into :yearID, :wins, :losses;
*/

{
#line 198 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 198 "a3.sqc"
  sqlaaloc(3,3,4,0L);
    {
      struct sqla_setd_list sql_setdlist[3];
#line 198 "a3.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 198 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)&yearID;
#line 198 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 198 "a3.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 198 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)&wins;
#line 198 "a3.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 198 "a3.sqc"
      sql_setdlist[2].sqltype = 496; sql_setdlist[2].sqllen = 4;
#line 198 "a3.sqc"
      sql_setdlist[2].sqldata = (void*)&losses;
#line 198 "a3.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 198 "a3.sqc"
      sqlasetda(3,0,3,sql_setdlist,NULL,0L);
    }
#line 198 "a3.sqc"
  sqlacall((unsigned short)25,2,0,3,0L);
#line 198 "a3.sqc"
  sqlastop(0L);
}

#line 198 "a3.sqc"

		printf("Year: %ld\n", yearID);
		printf("Wins: %ld\n", wins);
		printf("Losses: %ld\n\n", losses);
	}while(SQLCODE == 0);
	
	
/*
EXEC SQL CLOSE c2;
*/

{
#line 204 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 204 "a3.sqc"
  sqlacall((unsigned short)20,2,0,0,0L);
#line 204 "a3.sqc"
  sqlastop(0L);
}

#line 204 "a3.sqc"


}

/*******************************************************************************
 * function doQuery3() inserts a new tuple in the salaries table with information 
 * input by the user.
 ******************************************************************************/
void doQuery3(){
	
	/* Read user input */
	printf("Enter playerID: ");
	scanf("%s", playerID);
	
	
	/* Verify validity of playerID */
	
/*
EXEC SQL DECLARE c3 CURSOR FOR SELECT COUNT(playerID) FROM Players WHERE playerID = :playerID;
*/

#line 220 "a3.sqc"

	
/*
EXEC SQL OPEN c3;
*/

{
#line 221 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 221 "a3.sqc"
  sqlaaloc(2,1,5,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 221 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 64;
#line 221 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)playerID;
#line 221 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 221 "a3.sqc"
      sqlasetda(2,0,1,sql_setdlist,NULL,0L);
    }
#line 221 "a3.sqc"
  sqlacall((unsigned short)26,3,2,0,0L);
#line 221 "a3.sqc"
  sqlastop(0L);
}

#line 221 "a3.sqc"

	
/*
EXEC SQL FETCH c3 INTO :recExists;
*/

{
#line 222 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 222 "a3.sqc"
  sqlaaloc(3,1,6,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 222 "a3.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 222 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)&recExists;
#line 222 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 222 "a3.sqc"
      sqlasetda(3,0,1,sql_setdlist,NULL,0L);
    }
#line 222 "a3.sqc"
  sqlacall((unsigned short)25,3,0,3,0L);
#line 222 "a3.sqc"
  sqlastop(0L);
}

#line 222 "a3.sqc"


	if(SQLCODE == 100 || recExists != 1){
		printf("Invalid playerID!\n");
		
/*
EXEC SQL CLOSE c3;
*/

{
#line 226 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 226 "a3.sqc"
  sqlacall((unsigned short)20,3,0,0,0L);
#line 226 "a3.sqc"
  sqlastop(0L);
}

#line 226 "a3.sqc"

		return;
	}

	
/*
EXEC SQL CLOSE c3;
*/

{
#line 230 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 230 "a3.sqc"
  sqlacall((unsigned short)20,3,0,0,0L);
#line 230 "a3.sqc"
  sqlastop(0L);
}

#line 230 "a3.sqc"

	
	recExists = 0;

	/* Read user input */
	printf("Enter teamID:");
	scanf("%s", teamID);

	/* Verify validity of teamID */
	
/*
EXEC SQL DECLARE c4 CURSOR FOR SELECT COUNT(teamID) FROM Teams WHERE teamID = :teamID;
*/

#line 239 "a3.sqc"

	
/*
EXEC SQL OPEN c4;
*/

{
#line 240 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 240 "a3.sqc"
  sqlaaloc(2,1,7,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 240 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 3;
#line 240 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)teamID;
#line 240 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 240 "a3.sqc"
      sqlasetda(2,0,1,sql_setdlist,NULL,0L);
    }
#line 240 "a3.sqc"
  sqlacall((unsigned short)26,4,2,0,0L);
#line 240 "a3.sqc"
  sqlastop(0L);
}

#line 240 "a3.sqc"

	
/*
EXEC SQL FETCH c4 INTO :recExists;
*/

{
#line 241 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 241 "a3.sqc"
  sqlaaloc(3,1,8,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 241 "a3.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 241 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)&recExists;
#line 241 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 241 "a3.sqc"
      sqlasetda(3,0,1,sql_setdlist,NULL,0L);
    }
#line 241 "a3.sqc"
  sqlacall((unsigned short)25,4,0,3,0L);
#line 241 "a3.sqc"
  sqlastop(0L);
}

#line 241 "a3.sqc"


	if(SQLCODE == 100 || recExists < 1){
		printf("Invalid teamID!\n");
		
/*
EXEC SQL CLOSE c4;
*/

{
#line 245 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 245 "a3.sqc"
  sqlacall((unsigned short)20,4,0,0,0L);
#line 245 "a3.sqc"
  sqlastop(0L);
}

#line 245 "a3.sqc"

		return;
	}

	
/*
EXEC SQL CLOSE c4;
*/

{
#line 249 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 249 "a3.sqc"
  sqlacall((unsigned short)20,4,0,0,0L);
#line 249 "a3.sqc"
  sqlastop(0L);
}

#line 249 "a3.sqc"


	/* Read user input */
	printf("Enter yearID:");
	scanf("%ld", &yearID);

	recExists = 0;

	/* Verify validity of userID */
	
/*
EXEC SQL DECLARE c5 CURSOR FOR SELECT COUNT(teamID) FROM Teams WHERE teamID = :teamID AND yearID = :yearID;
*/

#line 258 "a3.sqc"

	
/*
EXEC SQL OPEN c5;
*/

{
#line 259 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 259 "a3.sqc"
  sqlaaloc(2,2,9,0L);
    {
      struct sqla_setd_list sql_setdlist[2];
#line 259 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 3;
#line 259 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)teamID;
#line 259 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 259 "a3.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 259 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)&yearID;
#line 259 "a3.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 259 "a3.sqc"
      sqlasetda(2,0,2,sql_setdlist,NULL,0L);
    }
#line 259 "a3.sqc"
  sqlacall((unsigned short)26,5,2,0,0L);
#line 259 "a3.sqc"
  sqlastop(0L);
}

#line 259 "a3.sqc"

	
/*
EXEC SQL FETCH c5 INTO :recExists;
*/

{
#line 260 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 260 "a3.sqc"
  sqlaaloc(3,1,10,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 260 "a3.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 260 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)&recExists;
#line 260 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 260 "a3.sqc"
      sqlasetda(3,0,1,sql_setdlist,NULL,0L);
    }
#line 260 "a3.sqc"
  sqlacall((unsigned short)25,5,0,3,0L);
#line 260 "a3.sqc"
  sqlastop(0L);
}

#line 260 "a3.sqc"


	if(SQLCODE == 100 || recExists != 1){
		printf("Invalid yearID for this teamID!\n");
		
/*
EXEC SQL CLOSE c5;
*/

{
#line 264 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 264 "a3.sqc"
  sqlacall((unsigned short)20,5,0,0,0L);
#line 264 "a3.sqc"
  sqlastop(0L);
}

#line 264 "a3.sqc"

		return;
	}
	
	
/*
EXEC SQL CLOSE c5;
*/

{
#line 268 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 268 "a3.sqc"
  sqlacall((unsigned short)20,5,0,0,0L);
#line 268 "a3.sqc"
  sqlastop(0L);
}

#line 268 "a3.sqc"


	/* Read user input */
	printf("Enter leagueID:");
	scanf("%s", lgID);
	
	/* Verify validity of leagueID */
	if(strcmp(lgID, "AL") != 0 && strcmp(lgID, "NL") != 0){
		printf("Invalid leagueID!\n");
		return;
	}

	/* Read user input */
	printf("Enter salary:");
	scanf("%f", &salary);

	/* Make sure salary >= 0 */
	if(salary < 0){
		printf("Salary must be greater than 0!\n");
		return;
	}

	/* Insert new tuple into table */
	
/*
EXEC SQL INSERT INTO Salaries (playerID, teamID, yearID, salary, lgID) 
	VALUES (:playerID, :teamID, :yearID, :salary, :lgID);
*/

{
#line 292 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 292 "a3.sqc"
  sqlaaloc(2,5,11,0L);
    {
      struct sqla_setd_list sql_setdlist[5];
#line 292 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 64;
#line 292 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)playerID;
#line 292 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 292 "a3.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 3;
#line 292 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)teamID;
#line 292 "a3.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 292 "a3.sqc"
      sql_setdlist[2].sqltype = 496; sql_setdlist[2].sqllen = 4;
#line 292 "a3.sqc"
      sql_setdlist[2].sqldata = (void*)&yearID;
#line 292 "a3.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 292 "a3.sqc"
      sql_setdlist[3].sqltype = 480; sql_setdlist[3].sqllen = 4;
#line 292 "a3.sqc"
      sql_setdlist[3].sqldata = (void*)&salary;
#line 292 "a3.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 292 "a3.sqc"
      sql_setdlist[4].sqltype = 460; sql_setdlist[4].sqllen = 2;
#line 292 "a3.sqc"
      sql_setdlist[4].sqldata = (void*)lgID;
#line 292 "a3.sqc"
      sql_setdlist[4].sqlind = 0L;
#line 292 "a3.sqc"
      sqlasetda(2,0,5,sql_setdlist,NULL,0L);
    }
#line 292 "a3.sqc"
  sqlacall((unsigned short)24,6,2,0,0L);
#line 292 "a3.sqc"
  sqlastop(0L);
}

#line 292 "a3.sqc"

	
	/* If insert succeeds then commit, otherwise rollback */
	if(SQLCODE != 0){
		
/*
EXEC SQL ROLLBACK;
*/

{
#line 296 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 296 "a3.sqc"
  sqlacall((unsigned short)28,0,0,0,0L);
#line 296 "a3.sqc"
  sqlastop(0L);
}

#line 296 "a3.sqc"

	}else{
		
/*
EXEC SQL COMMIT;
*/

{
#line 298 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 298 "a3.sqc"
  sqlacall((unsigned short)21,0,0,0,0L);
#line 298 "a3.sqc"
  sqlastop(0L);
}

#line 298 "a3.sqc"

	}

	
	printf("\nTable Salaries updated\n");
}


/*******************************************************************************
 * function doQuery4() sets a 10% salary increase for all the players of a
 * franchise
 ******************************************************************************/
void doQuery4(){
	
	
	
	/* Read user input */
	printf("Enter franchiseID:");
	scanf("%s", franchID);

	recExists = 0;

	/* Verify validity of franchiseID */
	
/*
EXEC SQL DECLARE c6 CURSOR FOR SELECT COUNT(franchID) FROM Franchise WHERE franchID = :franchID;
*/

#line 321 "a3.sqc"

	
/*
EXEC SQL OPEN c6;
*/

{
#line 322 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 322 "a3.sqc"
  sqlaaloc(2,1,12,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 322 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 3;
#line 322 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)franchID;
#line 322 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 322 "a3.sqc"
      sqlasetda(2,0,1,sql_setdlist,NULL,0L);
    }
#line 322 "a3.sqc"
  sqlacall((unsigned short)26,7,2,0,0L);
#line 322 "a3.sqc"
  sqlastop(0L);
}

#line 322 "a3.sqc"

	
/*
EXEC SQL FETCH c6 INTO :recExists;
*/

{
#line 323 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 323 "a3.sqc"
  sqlaaloc(3,1,13,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 323 "a3.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 323 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)&recExists;
#line 323 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 323 "a3.sqc"
      sqlasetda(3,0,1,sql_setdlist,NULL,0L);
    }
#line 323 "a3.sqc"
  sqlacall((unsigned short)25,7,0,3,0L);
#line 323 "a3.sqc"
  sqlastop(0L);
}

#line 323 "a3.sqc"


	if(SQLCODE != 0 || recExists != 1){
		printf("Invalid franchiseID!\n");
		
/*
EXEC SQL CLOSE c6;
*/

{
#line 327 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 327 "a3.sqc"
  sqlacall((unsigned short)20,7,0,0,0L);
#line 327 "a3.sqc"
  sqlastop(0L);
}

#line 327 "a3.sqc"

		return;
	}

	
/*
EXEC SQL CLOSE c6;
*/

{
#line 331 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 331 "a3.sqc"
  sqlacall((unsigned short)20,7,0,0,0L);
#line 331 "a3.sqc"
  sqlastop(0L);
}

#line 331 "a3.sqc"


	/* Create cursor for query */
	
/*
EXEC SQL DECLARE c8 CURSOR FOR SELECT s.playerID, s.salary, s.yearID FROM Salaries s, Teams t, Franchise f 
	WHERE s.teamID = t.teamID AND t.franchID = f.franchID AND f.franchID = :franchID AND s.yearID = t.yearID AND s.yearID = :yearID 
	AND s.yearID = (SELECT max(t.yearID) FROM Teams t, Franchise f WHERE t.franchID = f.franchID);
*/

#line 336 "a3.sqc"

	
/*
EXEC SQL OPEN c8;
*/

{
#line 337 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 337 "a3.sqc"
  sqlaaloc(2,2,14,0L);
    {
      struct sqla_setd_list sql_setdlist[2];
#line 337 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 3;
#line 337 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)franchID;
#line 337 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 337 "a3.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 337 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)&yearID;
#line 337 "a3.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 337 "a3.sqc"
      sqlasetda(2,0,2,sql_setdlist,NULL,0L);
    }
#line 337 "a3.sqc"
  sqlacall((unsigned short)26,8,2,0,0L);
#line 337 "a3.sqc"
  sqlastop(0L);
}

#line 337 "a3.sqc"


	/* For each salary record retreived increase the value by 10% */
	do{
		
/*
EXEC SQL FETCH c8 INTO :playerID, :salary, :yearID;
*/

{
#line 341 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 341 "a3.sqc"
  sqlaaloc(3,3,15,0L);
    {
      struct sqla_setd_list sql_setdlist[3];
#line 341 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 64;
#line 341 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)playerID;
#line 341 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 341 "a3.sqc"
      sql_setdlist[1].sqltype = 480; sql_setdlist[1].sqllen = 4;
#line 341 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)&salary;
#line 341 "a3.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 341 "a3.sqc"
      sql_setdlist[2].sqltype = 496; sql_setdlist[2].sqllen = 4;
#line 341 "a3.sqc"
      sql_setdlist[2].sqldata = (void*)&yearID;
#line 341 "a3.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 341 "a3.sqc"
      sqlasetda(3,0,3,sql_setdlist,NULL,0L);
    }
#line 341 "a3.sqc"
  sqlacall((unsigned short)25,8,0,3,0L);
#line 341 "a3.sqc"
  sqlastop(0L);
}

#line 341 "a3.sqc"

		if(SQLCODE != 0) break;
		
		
/*
EXEC SQL UPDATE Salaries SET salary = :salary + (:salary / 100) * 10 WHERE playerID = :playerID AND yearID = :yearID;
*/

{
#line 344 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 344 "a3.sqc"
  sqlaaloc(2,4,16,0L);
    {
      struct sqla_setd_list sql_setdlist[4];
#line 344 "a3.sqc"
      sql_setdlist[0].sqltype = 480; sql_setdlist[0].sqllen = 4;
#line 344 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)&salary;
#line 344 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 344 "a3.sqc"
      sql_setdlist[1].sqltype = 480; sql_setdlist[1].sqllen = 4;
#line 344 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)&salary;
#line 344 "a3.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 344 "a3.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 64;
#line 344 "a3.sqc"
      sql_setdlist[2].sqldata = (void*)playerID;
#line 344 "a3.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 344 "a3.sqc"
      sql_setdlist[3].sqltype = 496; sql_setdlist[3].sqllen = 4;
#line 344 "a3.sqc"
      sql_setdlist[3].sqldata = (void*)&yearID;
#line 344 "a3.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 344 "a3.sqc"
      sqlasetda(2,0,4,sql_setdlist,NULL,0L);
    }
#line 344 "a3.sqc"
  sqlacall((unsigned short)24,9,2,0,0L);
#line 344 "a3.sqc"
  sqlastop(0L);
}

#line 344 "a3.sqc"

	}while(1);

	
/*
EXEC SQL CLOSE c8;
*/

{
#line 347 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 347 "a3.sqc"
  sqlacall((unsigned short)20,8,0,0,0L);
#line 347 "a3.sqc"
  sqlastop(0L);
}

#line 347 "a3.sqc"


	printf("\nTable Salaries updated.\n");

}

/*******************************************************************************
 * function doQuery5() performs the query "the salary improvement rate per year
 * with the name of the franchise for a user specified playerID"
 ******************************************************************************/
void doQuery5(){
	
	/* Read user input */
	printf("Enter playerID:");
	scanf("%s", playerID);
	
	recExists = 0;
	
	/* Verify playerID validity */
	
/*
EXEC SQL DECLARE c10 CURSOR FOR SELECT COUNT(playerID) FROM Players WHERE playerID = :playerID;
*/

#line 366 "a3.sqc"

	
/*
EXEC SQL OPEN c10;
*/

{
#line 367 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 367 "a3.sqc"
  sqlaaloc(2,1,17,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 367 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 64;
#line 367 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)playerID;
#line 367 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 367 "a3.sqc"
      sqlasetda(2,0,1,sql_setdlist,NULL,0L);
    }
#line 367 "a3.sqc"
  sqlacall((unsigned short)26,10,2,0,0L);
#line 367 "a3.sqc"
  sqlastop(0L);
}

#line 367 "a3.sqc"

	
/*
EXEC SQL FETCH c10 INTO :recExists;
*/

{
#line 368 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 368 "a3.sqc"
  sqlaaloc(3,1,18,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 368 "a3.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 368 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)&recExists;
#line 368 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 368 "a3.sqc"
      sqlasetda(3,0,1,sql_setdlist,NULL,0L);
    }
#line 368 "a3.sqc"
  sqlacall((unsigned short)25,10,0,3,0L);
#line 368 "a3.sqc"
  sqlastop(0L);
}

#line 368 "a3.sqc"


	if(SQLCODE == 100 || recExists != 1){
		printf("Invalid playerID!\n");
		
/*
EXEC SQL CLOSE c10;
*/

{
#line 372 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 372 "a3.sqc"
  sqlacall((unsigned short)20,10,0,0,0L);
#line 372 "a3.sqc"
  sqlastop(0L);
}

#line 372 "a3.sqc"

		return;
	}

	
/*
EXEC SQL CLOSE c10;
*/

{
#line 376 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 376 "a3.sqc"
  sqlacall((unsigned short)20,10,0,0,0L);
#line 376 "a3.sqc"
  sqlastop(0L);
}

#line 376 "a3.sqc"
	
	
	/* Create cursor for query */
	
/*
EXEC SQL DECLARE c9 CURSOR FOR SELECT s1.yearID, s2.yearID, s1.salary, s2.salary, f1.franchName, f2.franchName  
	FROM Salaries s1, Salaries s2, Players p, Teams t1, Teams t2, Franchise f1, Franchise f2  
	WHERE p.playerID = :playerID AND s1.playerID = p.playerID AND s2.playerID = p.playerID 
	AND s1.teamID = t1.teamID AND t1.franchID = f1.franchID AND s2.teamID = t2.teamID 
	AND t2.franchID = f2.franchID 
	GROUP BY s1.yearID, s2.yearID, s1.salary, s2.salary, p.playerID, f1.franchName, f2.franchName 
	HAVING s2.yearID = (SELECT MIN(s3.yearID) 
	FROM Salaries s3 WHERE s3.playerID = :playerID AND s3.yearID > s1.yearID);
*/

#line 386 "a3.sqc"

	
/*
EXEC SQL OPEN c9;
*/

{
#line 387 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 387 "a3.sqc"
  sqlaaloc(2,2,19,0L);
    {
      struct sqla_setd_list sql_setdlist[2];
#line 387 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 64;
#line 387 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)playerID;
#line 387 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 387 "a3.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 64;
#line 387 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)playerID;
#line 387 "a3.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 387 "a3.sqc"
      sqlasetda(2,0,2,sql_setdlist,NULL,0L);
    }
#line 387 "a3.sqc"
  sqlacall((unsigned short)26,11,2,0,0L);
#line 387 "a3.sqc"
  sqlastop(0L);
}

#line 387 "a3.sqc"

	
	/* Retreive and print all results */
	do{
	
		
/*
EXEC SQL FETCH c9 INTO :yearID, :yearID2, :salary, :salary2, :franchName, :franchName2;
*/

{
#line 392 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 392 "a3.sqc"
  sqlaaloc(3,6,20,0L);
    {
      struct sqla_setd_list sql_setdlist[6];
#line 392 "a3.sqc"
      sql_setdlist[0].sqltype = 496; sql_setdlist[0].sqllen = 4;
#line 392 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)&yearID;
#line 392 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 392 "a3.sqc"
      sql_setdlist[1].sqltype = 496; sql_setdlist[1].sqllen = 4;
#line 392 "a3.sqc"
      sql_setdlist[1].sqldata = (void*)&yearID2;
#line 392 "a3.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 392 "a3.sqc"
      sql_setdlist[2].sqltype = 480; sql_setdlist[2].sqllen = 4;
#line 392 "a3.sqc"
      sql_setdlist[2].sqldata = (void*)&salary;
#line 392 "a3.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 392 "a3.sqc"
      sql_setdlist[3].sqltype = 480; sql_setdlist[3].sqllen = 4;
#line 392 "a3.sqc"
      sql_setdlist[3].sqldata = (void*)&salary2;
#line 392 "a3.sqc"
      sql_setdlist[3].sqlind = 0L;
#line 392 "a3.sqc"
      sql_setdlist[4].sqltype = 460; sql_setdlist[4].sqllen = 64;
#line 392 "a3.sqc"
      sql_setdlist[4].sqldata = (void*)franchName;
#line 392 "a3.sqc"
      sql_setdlist[4].sqlind = 0L;
#line 392 "a3.sqc"
      sql_setdlist[5].sqltype = 460; sql_setdlist[5].sqllen = 64;
#line 392 "a3.sqc"
      sql_setdlist[5].sqldata = (void*)franchName2;
#line 392 "a3.sqc"
      sql_setdlist[5].sqlind = 0L;
#line 392 "a3.sqc"
      sqlasetda(3,0,6,sql_setdlist,NULL,0L);
    }
#line 392 "a3.sqc"
  sqlacall((unsigned short)25,11,0,3,0L);
#line 392 "a3.sqc"
  sqlastop(0L);
}

#line 392 "a3.sqc"

		printf("Previous year: %ld, Current Year: %ld, Previous Franchise: %s, Current Franchise: %s, Salary Improvement Rate: %f%%\n", yearID, yearID2, franchName, franchName2, ((salary2 - salary)/salary)*100);
	
	}while(SQLCODE == 0);
	
	
/*
EXEC SQL CLOSE c9;
*/

{
#line 397 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 397 "a3.sqc"
  sqlacall((unsigned short)20,11,0,0,0L);
#line 397 "a3.sqc"
  sqlastop(0L);
}

#line 397 "a3.sqc"

	
}

/*******************************************************************************
 * function displayMainMenu() prints the program's main menu. 
 ******************************************************************************/
void displayMainMenu(){
	printf("\n[1] Player Information for given playerID");
	printf("\n[2] Number of wins and losses per year for a specified playerID");
	printf("\n[3] Insert a new tuple into Salaries");
	printf("\n[4] 10%% salary increase for players in a franchise");
	printf("\n[5] Salary improvement rate per year for a given player ID");
	printf("\n[6] Exit");
	printf("\n");
	printf("\nEnter choice: ");

}

/*******************************************************************************
 * function main() - entry point of the program. 
 ******************************************************************************/
int main(){
	
	int choiceIsValid,iMenuChoice;
	char chMenuChoice[2];
	
	/* Retrieve db name from environment variable DB2DATABASE */
	strcpy(db_name, getenv("DB2DATABASE"));

	/* Connect to db */
	
/*
EXEC SQL CONNECT TO :db_name;
*/

{
#line 428 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 428 "a3.sqc"
  sqlaaloc(2,1,21,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 428 "a3.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 8;
#line 428 "a3.sqc"
      sql_setdlist[0].sqldata = (void*)db_name;
#line 428 "a3.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 428 "a3.sqc"
      sqlasetda(2,0,1,sql_setdlist,NULL,0L);
    }
#line 428 "a3.sqc"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 428 "a3.sqc"
  sqlastop(0L);
}

#line 428 "a3.sqc"
 // connect to db
	
	
	/* Catch connection errors */
	if (SQLCODE != 0) {
		printf("Connect failed!: reason %ld\n", sqlca.sqlcode);
		exit(1);
	}
	
	/* Main menu loop */
	while(1)
	{
		
		/* print main menu */
		displayMainMenu();
		
		strcpy(chMenuChoice,"\0");

		choiceIsValid = 0;
		
		while(choiceIsValid == 0){
			
			/* read in user's menu choice */
			scanf("%s", chMenuChoice);
			
			if(!strcmp(chMenuChoice,"1")|| !strcmp(chMenuChoice,"2") || !strcmp(chMenuChoice,"3") ||
				!strcmp(chMenuChoice,"4") || !strcmp(chMenuChoice,"5") || 
				!strcmp(chMenuChoice,"6")){
				
				choiceIsValid = 1;
				
				iMenuChoice = atoi(chMenuChoice);
			
				break;
			
			}else{
				printf("\nThe option you have entered is not valid please try again.");
				displayMainMenu();
			}
		
		}
		
		printf("\n");
		
		/* Call the function appropriate to a user's menu choice */
		switch(iMenuChoice){
			case 1: 
				doQuery1();
		    break;
			case 2: 
				doQuery2();
		    break;
			case 3: 
				doQuery3();
				break;							
			case 4: 
				doQuery4();
	   	  break;
			case 5: 
				doQuery5();
		    break;
			case 6: 
			 	goto out;
		}
		
		/* prompt the user to either continue or exit */
		do{
			printf("\nBack to Main Menu? (y/n):");
			scanf("%s", chMenuChoice);
		}while(strcmp(chMenuChoice, "y") && strcmp(chMenuChoice, "n"));
		
		if(strcmp(chMenuChoice, "y")){
			break;
		}
	
	}

	/* Label for main loop exit */
	out:
	
	/* disconnect from db */
	
/*
EXEC SQL CONNECT RESET;
*/

{
#line 509 "a3.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 509 "a3.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 509 "a3.sqc"
  sqlastop(0L);
}

#line 509 "a3.sqc"

	printf("\nExiting.\n");
	
	return 0;

}
