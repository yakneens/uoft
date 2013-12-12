<%
  
  
  //Select all items in the shopping cart for user with this
  //session ID and store in ResultSet rs.
  sqlStr = "select s.quantity, p.name, m.name as man, p.price, ";
  sqlStr += "p.sale_price from shopping s, products p, ";
  sqlStr += "manufacturers m where sessionID = '" + session.getId() + "'";
  sqlStr += " and s.prodID = p.prodID and p.manID = m.manID";
  rs = sSQL.executeQuery(sqlStr);

  
  // Get form values from checkout page. 
  String fname = request.getParameter("fname");
  String lname = request.getParameter("lname");
  String addr = request.getParameter("addr");
  String apt = request.getParameter("apt");
  String city = request.getParameter("city");
  String country = request.getParameter("country");
  String pcode = request.getParameter("pcode");
  String phone = request.getParameter("phone");
  String email = request.getParameter("email");
  String ccard = request.getParameter("ccard");
  String expiry = request.getParameter("expiry");
  
  

 //Print a summary of user's billing information
%>
<tr>
  <td>
    <table border="0" cellpadding="15" cellspacing="0">
      <tr>
        <td>
          <p>Thank you <%= fname %> <%= lname %> for shopping at DOM-inion 
          Groceries! Please visit us again!</p><p>Your groceries will be 
          shipped to you within 24 hours via one of our refrigerated trucks in 
          order to ensure ultimate freshness. The driver will call you one hour 
          prior to delivery. We hope that your online experience with DOM-inion 
          Groceries has been a pleasant one.</p><p>Below is a summary of your 
          billing information  and purchase order.</p><p><span class="heading">
          Name</span><br/><%= fname %> <%= lname %></p><p><span class="heading">
          Address</span><br/><%= addr %>, Appt. <%= apt %>,<br/><%= city %>, 
          Canada,<br/><%= pcode %></p><p><span class="heading">Phone Number
          </span><br/><%= phone %></p><p><span class="heading">Email Address
          </span><br/><%= email %></p><p><span class="heading">Credit Card 
          Number</span><br/><%= ccard %><br/><br/><span class="heading">
          Card Expiry Date</span><br/><%= expiry %>
          <p>
            <table border="1" cellpadding="2" cellspacing="0">
              <tr>
                <td colspan="4" align="center">
<% 
  
  //Generate a purchase order number :-)
  Random rand = new Random(); 
  int purch = rand.nextInt();
  if(purch < 0){
    purch *= -1;
  }

%>
                  <span class="heading">Purchase Order</span> #<%= purch %>
                </td>
              </tr>
              <tr>
                <td align="right"><span class="heading">Item</span></td>
                <td align="right"><span class="heading">Quantity</span></td>
                <td align="right"><span class="heading">Price</span></td>
                <td align="right"><span class="heading">Amount</span></td>
              </tr>
<%
  
  
  float total = 0;
    
  
  //Print information about each item in user shopping cart, including,
  //product name, manufacturer, current price, final quantity, final
  //total per item, and final total for all items.
  while(rs.next()){
    
    String name = rs.getString("name");
    String man = rs.getString("man");
    int qty = rs.getInt("quantity");
    float price = rs.getFloat("price");
    float salePrice = rs.getFloat("sale_price");
    
    if(salePrice != 0){
      price = salePrice;
    }
    
    //Round final total for this item to two decimal points. 
    BigDecimal amnt = new BigDecimal(qty*price);
    amnt.setScale(2,BigDecimal.ROUND_UP);
    
    
    //Compute total for all items by adding each item's final total;
    total += amnt.floatValue();
    
    
%>
              <tr>
                <td align="right"><%= man %> <%= name %></td>
                <td align="right"><%= qty %></td>
                <td align="right">$<%= price %></td>
                <td align="right">$<%= amnt.floatValue() %></td>
              </tr>
<%
  }

  
  //Round final total for all items to two decimal points.
  BigDecimal tempTotal = new BigDecimal(total);
  tempTotal.setScale(2,BigDecimal.ROUND_UP);
  total = tempTotal.floatValue();

%>
              <tr>
                <td colspan="4" align="right"><span class="heading">Total:
                </span> $<%= total %></td>
              </tr>
            </table>
          </p>
        </td>
      </tr>
    </table>
  </td>
</tr>
<%
  
  
  //Clear the shopping cart.
  con.setAutoCommit(false);
    
  //Attempt the transaction.
  try{
    
    sSQL.executeUpdate("delete from shopping where sessionID = '" +
    session.getId() + "'");
    con.commit();
  
  //Rollback if transaction fails.
  }catch(SQLException e){
    con.rollback();
    
  }
  
  
  //Free database connection.
  StorePool.freeCon(con);
  
%>
