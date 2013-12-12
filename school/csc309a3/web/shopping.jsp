
<% 
  
  //Retreive shopping cart items for user with this session id.
  shoppingCart.setString(1,session.getId());
  rs = shoppingCart.executeQuery();
  
  float total = 0;
  
  String qstring = "";

  //If at category listing page query string consists of page and 
  //category id encoding.
  if(pg.equals("p")){ 
    
    qstring = "page=" + request.getParameter("page") + "&cid=";
    qstring += request.getParameter("cid"); 
  
  //If it search results listing page query string consists of page
  //and search keywords enconding.
  }else if(pg.equals("s")){
   
    qstring = "page=" + request.getParameter("page");
    qstring += "&searchField=" + request.getParameter("searchField");
  
  //If at checkout page query string consists of page encoding.
  } else if(pg.equals("c")){

    qstring = "page=" + request.getParameter("page");

  }

%>
<table width="300" border="0" cellpadding="0" cellspacing="0">
  <tr>
    <td colspan="3">
      <img src="images/shopping_cart_top.gif" width=300 height=66 alt=""></td>
  </tr>
  <tr>
    <td background="images/shopping_cart_left.gif" width="9"></td>
    <td align="center" width="280">
      <table width="280" border="0" cellpadding="2" cellspacing="0">
        <tr class="heading">
          <td align="center" width="60%">Name</td>
          <td width="10%">Price</td>
          <td width="10%">Qty.*</td>
          <td width="10%" align="right">Total</td>
          <td width="10%"></td>
        </tr>
<!-- items in the shopping cart -->
<%
  
  /*
    List information on items in the shopping cart including
    product name, manufacturer, current price, and current quantity
    
    Current price depends on whether the product is on sale.
    
    Current quantity can be changed by the user via the quantity field.
    The user can increase or decrease the current quantity of an item
    by entering a positive integer in the quantity field.
    Checks for malformed input are performed in the javascript function
    updateQuantity.
    
    The user can remove an item from the shopping cart entirely by clicking
    on the remove link on the right side of the shopping cart.
    
    The user can remove all items from the shopping cart by clicking on the
    Clear button on the lower right of the shopping cart.
    
    The user can navigate to the checkout page by clicking on the Checkout
    button on the lower left of the shopping cart.
   
   */
  while(rs.next()){
    int prodID = rs.getInt("prodID");
    String man = rs.getString("manName");
    String name = rs.getString("name");
    int quantity = rs.getInt("quantity");
    float price = rs.getFloat("price");
    float salePrice = rs.getFloat("sale_price");
    
    if(salePrice != 0){
      price = salePrice;
    }
    
    
    
    //Compute the total price of an item
    //total price = current price * quantity
    //The number is rounded up to two decimal points.
    BigDecimal tot = new BigDecimal(price * quantity);
    tot = tot.setScale(2,BigDecimal.ROUND_UP);
    float thisTotal = tot.floatValue();
    
    
    //Increase current total for all items in the shopping cart.
    total += thisTotal;


    //An item record in the shopping cart.
%>
        <tr>
<% /* A product's manufacturer and name */ %>
          <td width="60%"><%= man %> <%= name %></td>
<% /* A product's current price. */ %>
          <td align="right" width="10%">
          $<% if(salePrice != 0){ %><%= salePrice %><% }else{ %><%= price %>
          <% } %>
          </td>
<% /* A product's current quantity. */ %>
          <td width="10%" align="right">
            <input id="prod<%= prodID %>" 
                   type="text" 
                   size="3" 
                   maxlength="3" 
                   value="<%= quantity %>" 
                   onChange="javascript:updateQuantity(
                                                      <%= prodID %>, 
                                                      '<%= qstring %>')"/>
          </td>
<% /* A product's current total price. */ %>
          <td width="10%" align="right">$<%= thisTotal %></td>
<% /* Product remove button. */ %>
<% 
  String qstr = "update-shopping-remove.jsp?prodID=" + prodID + "&" + qstring;
%>
          <td width="10%"><a class="remove" href="<%= qstr %>">remove</a></td>
        </tr>
          
<% 
    
  } 
  
  
  //Round the total for all items in the shopping cart to two decimal points.
  BigDecimal tempTotal = new BigDecimal(total);
  tempTotal = tempTotal.setScale(2,BigDecimal.ROUND_UP);
  total = tempTotal.floatValue();
  
  rs.close();
  
%>                
        <tr>
          <td colspan="5" align="right">
<% /* The grand total of all items in the shopping cart. */ %>
          <span class="heading">Grand Total:</span> 
          $<%= total %>
          </td>
        </tr>
        <tr>
          <td colspan="5" class="note">
            * When you change the quantity of an item click elsewhere on the 
            screen to update!
          </td>
        </tr>
      </table>
    </td>
    <td background="images/shopping_cart_right.gif" width="10"></td>
  </tr>
  <tr>
    <td colspan="3">
      <img src="images/shopping_cart_middle.gif" 
           width=300 
           height=26 
           alt=""></td>
  </tr>
  <tr>
    <td colspan="3">
      <table border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td><img src="images/shopping_cart_bottom_left.gif" 
                   width=35 
                   height=46 
                   alt=""></td>
          <td>
            <table border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td>
<% /* The Checkout button. */ %>
                  <a href="#" onclick="javascript:checkout();">
                  <img border="0" 
                       class="imgover" 
                       src="images/shopping_cart_checkout.gif" 
                       width=140 
                       height=34 
                       alt=""/></a></td>
                <td>
<% /* The Clear button. */ %>
                  <a href="update-shopping-clear.jsp?<%= qstring %>">
                  <img  border="0" 
                        class="imgover" 
                        src="images/shopping_cart_clear.gif" 
                        width=90 
                        height=34 
                        alt=""/></a></td>
              </tr>
              <tr>
                <td colspan="2"><img src="images/shopping_cart_bottom.gif" 
                                     width=230 
                                     height=12 
                                     alt=""></td>
              </tr>
            </table>
          </td>
          <td><img src="images/shopping_cart_bottom_right.gif" 
                   width=35 
                   height=46 
                   alt=""></td>
        </tr>
      </table>
    </td>
  </tr>
</table>
  
    
