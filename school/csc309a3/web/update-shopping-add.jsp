<%@ page import="java.sql.*,javax.sql.*,javax.naming.*,java.util.*" %>
<%

  String sqlStr;
  
  //Get product ID from request object.
  int prodID = Integer.parseInt(request.getParameter("prodID"));
  
  
  //Get user's current page from request object.
  String pg = request.getParameter("page");
  
  int catID = 0;
  
  String searchField ="";
  
  //If page is a product listing by category get category id
  //from request object.
  if(pg.equals("p")){
  
    catID  = Integer.parseInt(request.getParameter("cid"));
  
  //If page is a search result listing get search keywords
  //from request object.
  }else{
  
    searchField = request.getParameter("searchField");
  
  }
  
  
  //Get this user's session ID.
  String sessionID = session.getId();
  
  
  //Establish a database connection.
  Connection con = StorePool.getInstance().getConnection();
  
  
  
  con.setAutoCommit(false);
  
    
  java.sql.Statement sSQL = 
  con.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, 
                      ResultSet.CONCUR_READ_ONLY);
  
  
  
  //Select all items that are in this user's shopping cart 
  //and store them in rs.
   sqlStr = "select * from shopping where sessionID = '" + session.getId();
   sqlStr += "'";
  ResultSet rs = sSQL.executeQuery(sqlStr);
  
  
  
  int quantity = 0;
  int itemID = 1;
  
  //If shopping cart not empty.
  if(rs.next()){
    
    rs.beforeFirst();
  
    
    //Go through all the items in the shopping cart.
    while(rs.next()){
      
      
      //Get current item's product id.
      int thisProdID = rs.getInt("prodID");
    
      //If this is the same item as the one selected by the user
      //increase that item's current quantity in the shopping
      //cart by 1.
      if(thisProdID == prodID){
      
        quantity = 1 + rs.getInt("quantity");
              
        //Attempt a database update transaction that records the new
        //quantity of an item.
        try{
          
          java.util.Date now = new java.util.Date();
          
          sqlStr = "update shopping set quantity =" + quantity;
          sqlStr += ", transDate=" + now.getTime() + " where prodID=";
          sqlStr += prodID;
          sSQL.executeUpdate(sqlStr);
          
          con.commit();
        
        //If transaction fails, rollback transaction.
        }catch(SQLException e){
        
          con.rollback();
          
        }
        
        
        //Close SQL statement and free database connection.
        sSQL.close();
        StorePool.freeCon(con);
        
        
        String qstring = "";
  
        //Redirect user back to the page they were on.
        if(pg.equals("p")){
    
          qstring = "index.jsp?page=" + pg + "&cid=" + catID;
          response.sendRedirect(qstring);
  
        }else if(pg.equals("s")){
  
          qstring = "index.jsp?page=" + pg + "&searchField=" + searchField;
          response.sendRedirect(qstring);

        }else if(pg.equals("c")){

          qstring = "index.jsp?page=" + pg;
          response.sendRedirect(qstring);

        }
    
      }
      
    }       
  
  } 
    
  //If this item not in the shopping cart.
  //Select the largest value of item ID in the shopping cart table.
  //Increase the number by 1 to become this item's itemID.
  rs = sSQL.executeQuery("select max(itemID) as itemID from shopping");
  rs.first();
  itemID += rs.getInt("itemID");
    
    
  //Attempt inserting this item into the shoppping cart table.
  try{
  
    java.util.Date now = new java.util.Date();
    
    sqlStr = "insert into shopping values(" + itemID;
    sqlStr += "," + prodID + ",1" + "," + now.getTime();
    sqlStr += ",'" + sessionID + "')";
    sSQL.executeUpdate(sqlStr);
    
    con.commit();
      
  //If transaction fails, rollback.
  }catch(SQLException e){
  
        con.rollback();
        
  }
  
  
  //Close sql statement and free database connection.
  sSQL.close();
  StorePool.freeCon(con);
  
  String qstring = "";
  
  //Redirect user back to the page they were on.
  if(pg.equals("p")){
    
    qstring = "index.jsp?page=" + pg + "&cid=" + catID;
    response.sendRedirect(qstring);
  
  }else if(pg.equals("s")){
  
    qstring = "index.jsp?page=" + pg + "&searchField=" + searchField;
    response.sendRedirect(qstring);

  }else if(pg.equals("c")){

    qstring = "index.jsp?page=" + pg;
    response.sendRedirect(qstring);

  }

%>
