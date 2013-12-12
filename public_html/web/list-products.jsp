<% 
  
  //If the page requested is a listing of items in a category
  if(pg.equals("p")){
  
    String cid ="";
    
    //Retreive the category ID from request object
    cid = request.getParameter("cid");
    
    
    //Select all products in this category and store in
    //ResultSet rs
    listByCategory.setLong(1,Long.parseLong(cid));
    rs = listByCategory.executeQuery();
  
  //If the page requested is a listing of search results
  }else if(pg.equals("s")){
    
    Matcher m;
    Pattern p;
    String searchField = "";
    
    
    //Retreive search keywords from request object
    searchField = request.getParameter("searchField");
    
    
    //Split keywords into tokens
    String[] keywordArray = searchField.split(" ");
    
    HashSet searchResults = new HashSet();
    
    //Select all the products and store them in ResultSet rs
    rs = sSQL.executeQuery("select name, prodID from products");
    
    //For each keyword check if that keyword exists as a substring
    //in the name of a product
    //If a match is found add the productID to the set of results
    for(int i = 0; i < keywordArray.length; i++){
      
      p = Pattern.compile(keywordArray[i], 
                          Pattern.CASE_INSENSITIVE | 
                          Pattern.UNICODE_CASE | 
                          Pattern.COMMENTS);
            
      while(rs.next()){
        
        String name = rs.getString("name");
        m = p.matcher(name);
      
        if(m.find()){
          searchResults.add(rs.getString("prodID"));
        }
      
      }
    
      rs.beforeFirst();
    }
    
    rs.close(); 
  
    //If any products were found that match the search criteria
    if(!searchResults.isEmpty()){
      
      Iterator prods = searchResults.iterator();
  
      //Create a statement that selects all the products
      //that match search criteria
      sqlStr = "select prodId, p.name as prodName, type, description, units, ";
      sqlStr += "price, sale_price, c.name as catName, m.name as manName, ";
      sqlStr += "image from products p, categories c, manufacturers m ";
      sqlStr += "where p.catID = c.catID and p.manID = m.manID and(";
  
      if(prods.hasNext())
        sqlStr += "p.prodID = " + prods.next();
    
      while(prods.hasNext()){
          sqlStr +="or p.prodID = " + prods.next();
      }
  
      sqlStr += ")";
    
      rs = sSQL.executeQuery(sqlStr);
  
    //If no products were found that match search criteria 
    //redirect to the page that informs the user that
    //no match was found
    } else{
%>    
      <jsp:forward page="index.jsp">
        <jsp:param name="page" value="n"/>
      </jsp:forward>
<%  }
  
  }     

  //Print to the user each item found as a result of either search
  //or list by category
  while(rs.next()){ 
    int prodID = rs.getInt("prodID");
    String name = rs.getString("prodName");
    String type = rs.getString("type");
    String description = rs.getString("description");
    int units = rs.getInt("units");
    float price = rs.getFloat("price");
    float salePrice = rs.getFloat("sale_price");
    String cat = rs.getString("catName");
    String man = rs.getString("manName");
    String image = rs.getString("image");
        
%>
<table border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td colspan="3" width="369"><img src="images/product_top.gif"/></td>
  </tr>
  <tr>
    <td background="images/product_left.gif" width="10"></td>
    <td align="center" width="349">
      <table cellpadding="0" cellspacing="0" border="0">
        <tr>
          <td>
            <table>
              <tr>
                <td><span class="heading">Manufacturer:</span> <%= man %></td>
              </tr>
              <tr>
                <td><span class="heading">Name:</span> <%= name %></td>
              </tr>
              <tr>
                <td><span class="heading">Type: </span><%= type %></td>
              </tr>
              <tr>
                <td>
                  <span class="heading">Description:</span> <%= description %>
                </td>
              </tr>
              <tr>
                <td><span class="heading">Units:</span> <%= units %></td>
              </tr>
              <tr>
                <td>
                  <span class="heading">Price:</span>
<%

  //If item is on sale list the regular price in
  //a strikethrough font and list sale price in red
  if(salePrice != 0){ 

%>
                  <span class="oldPrice"> $<%= price %></span>
                </td>
              </tr>
              <tr>
                <td>
                  <span class="heading">Sale Price:</span>
                  <span class="salePrice"> $<%= salePrice %></span>
                  <img align="left" src="images/sale_small.gif"/>
                </td>
<%

  //If item is not on sale list the regular price
  //in regular fashion and list sale price
  //as N/A
  }else{ 

%>
                  $<%= price %></span>
                </td>
              </tr>
              <tr>
                <td><span class="heading">Sale Price:</span> N/A</td>
<%  } %>
              </tr>
            </table>
          </td>
          <td width="130">
            <table>
              <tr>
                <td><img src="images/<%=image%>"</td>
              </tr>
              <tr>
                <td align="right">
<%

  String qstring = "update-shopping-add.jsp?prodID=" + prodID;
  
  //The add to shopping cart button redirects the user
  //to the script that performs shopping cart add
 if(pg.equals("p")){ 
 
    qstring += "&page=p&cid=" + request.getParameter("cid"); 
 %>
                  <a href="<%= qstring %>">
<% 

  }else if(pg.equals("s")){ 
    qstring += "&page=s&searchField=" + request.getParameter("searchField");
%>
                  <a href="<%= qstring %>">
<% } %>
                    <img src="images/button_add.jpg" border="0" class="imgover">
                  </a>
                </td>
              </tr>
            </table>
          </td>
        </tr>
      </table>
    </td>
    <td background="images/product_right.gif" width="10"></td>  
  </tr>
  <tr>
    <td colspan="3">
      <img src="images/product_bottom.gif" />
    </td>
  </tr>
</table>
<br/>         
<%  
    
  }
  
  rs.close(); 
  
%>
