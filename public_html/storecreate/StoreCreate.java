import java.sql.*;
import java.io.*;

/**
* Example app using Pointbase. This program contains such features as:
*    - poor error handling
*    - very few useful comments
*    - bad overall design (everything's static, yuck!)
*
* All these fabulous features were added intentionally. Your apps that use
* JDBC database access are expected to be written with a great deal more care
* than this one. The sole purpose of this application is to give you a
* quick-and-dirty look at database access in java (and with Pointbase in
* particular). Enjoy!
*/
public class StoreCreate {
    public static void main(String[] args) {
        // connect to the database
        Connection c = connect("store", true);
        if (c == null) {
            System.err.println("Couldn't get a connection to the database, " + 
             "aborting.");
            System.exit(1);
        }

        // Create the table
        createTables(c);

       disconnect(c);
	System.out.println("Database Created");
    }

    private static void createTables(Connection connection) {
        String sql = "create table categories ( " +
         "catid int primary key, " +
         "name varchar(128) not null)";

        // swallow any exceptions, probably just a "table already exists"
        // exception which is fine (but what if it isn't???)
      try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){} 
        
	sql = "create table manufacturers(manid int primary key, name varchar(128) not null, address varchar(128) not null, city varchar(128) not null, country varchar(128) not null, phone varchar(128) not null)";
	try{connection.createStatement().executeUpdate(sql);
	}catch (Exception e){}
	
	sql = "create table products(prodid int primary key,"+
	"catid int foreign key references categories(catid),"+
	"manid int foreign key references manufacturers(manid),"+
	"name varchar(128) not null, type varchar(128) not null,"+
	"description varchar(256) not null,  image varchar(128) not null, "+
	"units int not null, price real not null, sale_price real default 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch (Exception e){}
	
	sql = "create table shopping(itemID int primary key, prodID int foreign key references products(prodID), quantity int not null, transDate numeric not null, sessionID varchar(128) not null)";
	try{connection.createStatement().executeUpdate(sql);
	}catch (Exception e){}
	
	try{connection.createStatement().executeUpdate(sql);
	}catch (Exception e){}
	
	sql = "insert into categories values(1,'Dairy')";
	try{connection.createStatement().executeUpdate(sql);
	}catch (Exception e){}
	
	sql = "insert into categories values(2,'Meat')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	sql = "insert into categories values(3,'Cans')";
	try{connection.createStatement().executeUpdate(sql);
	}catch (Exception e){}
	
	sql = "insert into categories values(4,'Bulk')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	sql = "insert into categories values(5,'Beverages')";
	try{connection.createStatement().executeUpdate(sql);
	}catch (Exception e){}
	sql = "insert into categories values(6,'Liquor')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	sql = "insert into categories values(7,'Baked Goods')";
	try{connection.createStatement().executeUpdate(sql);
	}catch (Exception e){}
	
	sql = "insert into manufacturers values(1,'Natrel', '123 Natrel place., #1234','Los Angeles', 'USA','(123)321-1223')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(2,'Equality', '123 Equality place.','Dallas', 'USA','(213)124-5422')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(3,'Yoplait', '1 Yoplait cres.','Calgary', 'Canada','(642)636-7347')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(4,'Schneiders', '1581 Sausage ave.','Toronto', 'Canada','(416)834-5284')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(5,'Lactancia', 'Farm #5','Prarieville', 'Canada','(631)732-3156')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(6,'Kraft', '23 Bachelors ave.','Eskimocity', 'North Pole','(111)222-3333')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(7,'Campbells', '7334 Hwy 11, Suite 14','Washington', 'USA','(651)733-6324')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(8,'PC', 'Presedential Palace','Toronto', 'Canada','(905)112-8456')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(9,'Upper Canada', '95 Algonquin Park','North Bay', 'Canada','(622)831-7422')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into manufacturers values(10,'No Name', '12 No Name blvd., #13','Amsterdam', 'Netherlands','(841)718-9821')";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(1,1,1,'Milk','2%',"+
	"'Good Milk','tomatoes.jpg', 1, 4.59, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(2,2,4,'Chicken','Wire Wrapped',"+
	"'Healthy and fat free. Great for any occasion.','tomatoes.jpg', 1, 8.50, 6.50)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(3,1,3,'Yogurt','Strawberry',"+
	"'Healthy and fat free. Great for any occasion.','tomatoes.jpg', 12, 5.99, 4.99)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(4,3,7,'Soup','Mushroom',"+
	"'Healthy and fat free. Great for any occasion.','tomatoes.jpg', 12, 10.23, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(5,4,6,'Dinner','Cheese',"+
	"'Healthy and fat free. Great for any occasion.','tomatoes.jpg', 1, 1.89, 1.50)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(6,1,5,'Butter','Salted',"+
	"'Healthy and fat free. Great for any occasion.','tomatoes.jpg', 1, 4.00, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(7,2,2,'Chicken','Stuffed',"+
	"'Very well produced and bread. Comes wrapped with a string','tomatoes.jpg', 1, 12.99, 12.95)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(8,2,4,'Bacon','Thin Sliced',"+
	"'Healthy and fat free. Great for any occasion.','tomatoes.jpg', 1, 4.59, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(9,1,2,'Milk','Skim',"+
	"'Good Milk','tomatoes.jpg', 1, 4, 3.50)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(10,5,8,'Soda','Soda',"+
	"'Plain soda water.','tomatoes.jpg', 1, 2.00, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(11,5,8,'Ginger Ale','Ginger Ale',"+
	"'Some good Ginger Ale','tomatoes.jpg', 1, 2.00, 1.0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(12,5,8,'Cola','Cola',"+
	"'This is not Coke.','tomatoes.jpg', 1, 2.00, 1.0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(13,5,8,'Iced Tea','Iced Tea',"+
	"'Some good shtuff.','tomatoes.jpg', 1, 2.00, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(14,6,9,'Lager','Dave's Beer',"+
	"'Cheap beer','tomatoes.jpg', 24, 24.5, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(15,6,9,'Ale','Dave's Beer',"+
	"'Cheap beer.','tomatoes.jpg', 24, 24.5, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(16,6,9,'Porter','Dave's Coctail',"+
	"'Very cheap beer.','tomatoes.jpg', 24, 24.5, 14.99)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(17,6,8,'Cooler','Milk',"+
	"'Cheap coctail.','tomatoes.jpg', 1, 4.59, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(18,6,8,'Rum and Coke','Dave's Coctail',"+
	"'Watered down drink','tomatoes.jpg', 6, 15, 13.5)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(19,7,10,'Bread','Stale',"+
	"'Good bread.','tomatoes.jpg', 1, 3, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(20,7,10,'Italian Bun','Stale',"+
	"'Hearty Italian Buns','tomatoes.jpg', 6, 5, 3.5)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(21,7,10,'Hot Dog Bun','Stale',"+
	"'Great for BBQ on the back yard.','tomatoes.jpg', 12, 8.99, 5.35)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(22,7,10,'Cake','Chocolate',"+
	"'Really cheap cake.','tomatoes.jpg', 1, 3, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(23,7,2,'Eclaire','Fat Free',"+
	"'Healthy and fat free. Great for any occasion.','tomatoes.jpg', 12, 15.89, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(24,7,6,'Bagels','Poppy Seed',"+
	"'Delicious bagels.','tomatoes.jpg', 6, 3.99, 3.59)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(25,7,6,'Kaiser','Plain',"+
	"'Healthy and fat free. Great for any occasion.','tomatoes.jpg', 6, 3.99, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(26,7,5,'Turnover','Apple',"+
	"'Fat free and delicious. Unparallel experience.','tomatoes.jpg', 1, 1, 0.89)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	
	sql = "insert into products values(27,1,2,'Roll','Round',"+
	"'Sweet, sweet roll.','tomatoes.jpg', 1, 5.00, 0)";
	try{connection.createStatement().executeUpdate(sql);
	}catch(Exception e){}
	

}

    /**
    * This method creates and returns a JDBC connection object to the named
    * Pointbase database.
    *
    * @param dbName  name of the database to connect to
    * @param createDb  if true create the database if it doesn't already exist
    * @return  the database Connection if successful, null otherwise
    */
    // TODO: should print out error messages to System.err
    public static Connection connect(String dbName, boolean createDb) {
        boolean connected = false;
        Connection connection = null;

        String dburl = "jdbc:pointbase:" + dbName;

        // try to load the JDBC driver
        try {
            String driver = "com.pointbase.jdbc.jdbcUniversalDriver";
            Class.forName(driver).newInstance();
        } catch (Exception e) {
            return null;
        }

        // try to connect to an existing database as the "public" user
        try {
            // always connecting as the "public" user isn't the best, but it's
            // fine for csc309
            connection = DriverManager.getConnection(
             dburl, "public", "public");
        } catch (Exception e) {
            // swallow the exception here
        }

        // if we're connected return the connection object
        if (connection != null) return connection;

        // still not connected eh? DB probably doesn't exist, create a new
        // database if we're allowed to...
        if (createDb) {
            dburl = dburl + ",new";

            try {
                connection = DriverManager.getConnection(
                 dburl, "public", "public");
            } catch (Exception e) {
                // swallow the exception here
            }
        }

        // ok now I've tried everything, just return the connection object
        // wether it's null or not
        return connection;
    }

    /**
    * Close the given database connection.
    *
    * @param connection  the connection to close
    * @return  true if successful, false otherwise
    */
    // TODO: should print out error messages to System.err
    public static boolean disconnect(Connection connection) {
        try { connection.close(); } catch (Exception e) { return false; }
        return true;
    }
}
