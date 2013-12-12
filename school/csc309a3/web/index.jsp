<%@ page import = "java.sql.*,javax.sql.*,javax.naming.*,java.util.*,java.math.*,java.util.regex.*;" %>
<%@ page contentType="text/html" %>
<%@ page session="true"%>
<%    
      
      //Get a connection to the database
      Connection con = StorePool.getInstance().getConnection();
      
      //Create a Statement object
      java.sql.Statement sSQL = 
      con.createStatement(ResultSet.TYPE_SCROLL_INSENSITIVE, 
                          ResultSet.CONCUR_READ_ONLY);
      
      
      //Create a session
      session = request.getSession();
      
      String sqlStr;

      //Prepared statement for listing products by category
      sqlStr = "select prodId, p.name as prodName, type, "; 
      sqlStr += "description, units, price, sale_price, ";
      sqlStr += "c.name as catName, m.name as manName, ";
      sqlStr += "image from products p, categories c, ";
      sqlStr += "manufacturers m where p.catID = ? and ";
      sqlStr += "p.catID = c.catID and p.manID = m.manID";
      PreparedStatement listByCategory = 
                        con.prepareStatement(sqlStr);
      
      //Prepared statement for getting the name of a category
      sqlStr = "select name from categories where catID = ?";
      PreparedStatement categoryName = 
                        con.prepareStatement(sqlStr);
      
      //Prepared statement for getting the name of a manufacturer
      sqlStr = "select name from manufacturers where manID = ?";
      PreparedStatement manufacturerName = 
                        con.prepareStatement(sqlStr);
      
      //Prepared statement for selecting items in the shopping cart
      sqlStr = "select p.prodID, m.name as manName, p.name, ";
      sqlStr += "s.quantity, p.price, p.sale_price from ";
      sqlStr += "products p, manufacturers m, shopping s where ";
      sqlStr += "s.sessionID = ? and s.prodID = p.prodID and ";
      sqlStr += "p.manID = m.manID";
      PreparedStatement shoppingCart = 
                        con.prepareStatement(sqlStr);
      
      
      ResultSet rs;
      
      //Determine what page the user requested
      String pg = request.getParameter("page");
      
      //If user just navigated to the site
      if(pg == null){
        pg = "i";
      
        
        //Clean out all shopping cart entries that are older
        //than 24 hours.
        java.util.Date now = new java.util.Date();
        long yesterday = now.getTime() - 86400000;
        sqlStr = "delete from shopping where transDate < " + 
                  yesterday;
        
        try{
          sSQL.executeUpdate(sqlStr);
          con.commit();
        }catch(SQLException e){
          con.rollback();
        }
      
      }
      
%>
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Frameset//EN" "http://www.w3.org/TR/html4/frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <title>DOM-inion Groceries</title>
    <style>
      
      li.menu {
        list-style-type:none;
        font-size: 14pt;
        
      }

      .heading {
        font-weight: bold;
      }

      .oldPrice {
        text-decoration: line-through;
      }

      .salePrice {
        color:red;
      }

      body {
        font-family: Verdana, Arial, Helvetica, sans-serif;
        font-size: 10pt;
         
      }

      a.remove{
        font-family: Verdana, Arial, Helvetica, sans-serif;
        font-size: 8pt;
        color:#dd0000;
        font-weight:bolder;
        text-decoration:none;
      }
      
      a.leftnav{
        text-decoration: none;
        color:#339900;
        font-weight: bolder;
        text-align:right;
      }
      
      .filler{
        font-size:12pt;
      
      }
       
      .note {
        font-style: italic;
      }
    </style>
    <script language="Javascript" src="jscripts.js"/>
  </head>
  <body style="margin-left: 50px; margin-right: 50px; margin-top: 20px;" 
        bgcolor="#FFFFFF">
    <table border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td colspan="7"><img src="images/page_top.gif" 
                             width="924" 
                             height="165" 
                             alt=""></td>
      </tr>
      <tr>
        <td background="images/leftmost_side.gif" width="6"></td>
<!-- Left navigation bar goes here -->
        <td align="center" valign="top" width="206">
        <%@ include file="leftnav.jsp" %></td>
        <td background="images/left_side.gif" width="5"></td>
        <td width="382" valign="top">
          <table border="0" cellspacing="0" cellpadding="5">
            <tr>
              <td width="382">
<!-- Main page body goes here -->             
<%  
  //Include the page the user requested.
  
  if(pg != null){ 
    if(pg.equals("p") || pg.equals("s")){
%>
<%@ include file="list-products.jsp" %>
<%  
    }else if(pg.equals("i")){
%>      
<%@ include file="intro.jsp" %>
<%      
    }else if(pg.equals("c")){
%>
<%@ include file="checkout.jsp" %>
<%
    }else if(pg.equals("y")){
%>
<%@ include file="summary.jsp" %> 
<%    
    }else if(pg.equals("n")){
%>
<%@ include file="not-found.jsp" %>
<%
    }
  }
%>
              </td>
            </tr>
          </table>
        </td>
        <td background="images/right_side.gif" width="5"></td>
        <td width="315" align="center" valign="top">
<!-- Shopping Cart goes here -->
          <%@ include file="shopping.jsp" %>
        </td>
        <td background="images/rightmost_side.gif" width="5"></td>
      </tr>
      <tr>
        <td colspan="7">
          <img src="images/page_bottom.gif" width=924 height=15 alt=""></td>
      </tr>
    </table>
    <form id="navForm" name="navForm" method="post" action="index.jsp">
      <input type="hidden" name="page" id="page" value="p"/>
      <input type="hidden" name="cid" id="cid" value=""/>
    </form>
  </body>
</html>

<% 
  
  //Free database connection.
  StorePool.freeCon(con);

%>
