<%@ page import="java.sql.*,javax.sql.*,javax.naming.*,java.util.*" %>
<%
  
  //Get user's current page from request object.
  String pg = request.getParameter("page");
  
  int catID = 0;
  String searchField ="";
  
  //If user was on product listing by category page
  //Get category id from request object.
  if(pg.equals("p")){
    
    catID  = Integer.parseInt(request.getParameter("cid"));
  
  //If user was on search result listing page
  //Get search keywords from request object.
  }else{
  
    searchField = request.getParameter("searchField");
  
  }
    
  //Establish a database connection, and create sql statement.
  Connection con = StorePool.getInstance().getConnection();
  con.setAutoCommit(false);
  java.sql.Statement sSQL = con.createStatement();
  
  //Attempt to remove all items from the shopping cart of this user.
  try{
    
    sSQL.executeUpdate("delete from shopping where sessionID = '" +
    session.getId() + "'");
    con.commit();
  
  //If transaction fails, rollback.
  }catch(SQLException e){
    con.rollback();
    
  }
  
  //Free database connection.
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
