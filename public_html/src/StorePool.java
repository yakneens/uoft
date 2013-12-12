import java.sql.*;

/***********************************************
 * Class StorePool a singleton variant of
 * connection pool
 ***********************************************/  

public class StorePool extends ConnectionPool { 
	
	//The single instance of StorePool.
	private static StorePool pool = null; 
	
	//Private consturctor StorePool
	//Creates an instance of store pool
	//that is connected to database
	//jdbc:pointbase:store
	//using the com:pointbase:jdbcUniversalDriver
	private StorePool() throws SQLException{ 
		super("com.pointbase.jdbc.jdbcUniversalDriver", 
						"jdbc:pointbase:store",
						"PUBLIC",
						"PUBLIC",
						10,
						50,
						true);
				
	} 
	
	//Method used to access an instance of StorePool.
	public static synchronized StorePool getInstance(){ 
	
		//If first time a connection is requested.
		//create a new connection pool.
		if (pool == null) { 
			
			try{ 
			
				pool = new StorePool();
			
			}catch (SQLException e){
			
				System.out.println(e.toString());
			
			} 
		
		} 
	
		//otherwise return the connection pool.
		return(pool); 
	} 
	
	//Free a connection.
	public static synchronized void freeCon(Connection con){
	
		pool.free(con);
	
	}
	
	//Return total number of connections.
	public static synchronized int total(){
	
		return pool.totalConnections();
	
	}
}
