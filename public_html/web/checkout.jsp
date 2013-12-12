<%

  // This is the checkout form 
  //upon submission fields are validated 
  //by the validation script in jscripts.js

%>
<tr>
  <td>    
    <form id="billing" 
          name="billing" 
          onSubmit="return validate();" 
          action="index.jsp" 
          method="post">
      <table cellpadding="5">
        <tr>
          <td align="center" colspan="2" class="heading">
            Billing Information
          </td>
        </tr>
        <tr>
          <td class="heading">First Name</td>
          <td><input type="text" value="John" name="fname" id="fname"/></td>
        </tr>
        <tr>
          <td class="heading">Last Name</td>
          <td><input type="text" value="Doe" name="lname" id="lname"/></td>
        </tr>
        <tr>
          <td class="heading">Address</td>
          <td><input type="text" 
                     value="666 Spadina ave." 
                     name="addr" 
                     id="addr"/></td>
        </tr>
        <tr>
          <td class="heading">Appt.</td>
          <td><input type="text" value="13" name="apt"/></td>
        </tr>
        <tr>
          <td class="heading">City </td>
          <td><input type="text" value="Toronto" name="city" id="city"/></td>
        </tr>
        <tr>
          <td class="heading">Country</td>
          <td><input type="text" value="Canada" disabled name="country"/>*</td>
        </tr>
        <tr>
          <td class="heading">Postal Code</td>
          <td><input type="text" value="A1A 1A1" name="pcode" id="pcode"/></td>
        </tr>
        <tr>
          <td class="heading">Phone Number</td>
          <td><input type="text" 
                     value="(416)123-4567" 
                     name="phone" 
                     id="phone"/></td>
        </tr>
        <tr>
          <td class="heading">Email Address</td>
          <td><input type="text" value="a@b.com" name="email" id="email"/></td>
        </tr>
        <tr>
          <td class="heading">Credit Card #</td>
          <td><input type="text" 
                     value="123456789" 
                     name="ccard" 
                     id="ccard"/></td>
        </tr>
        <tr>
          <td class="heading">Expiry Date(dd/mm/yyyy)</td>
          <td><input type="text" 
                     value="11/11/1911" 
                     name="expiry" 
                     id="expiry"/></td>
        </tr>
        <tr>
          <td colspan="2" align="right">
            <a href="#" 
               onclick="document.getElementById('billing').submit();">
            <img class="imgover" 
                 src="images/button_purchase.jpg" 
                 border="0"/></a> 
            <a href="javascript:document.getElementById('billing').reset()">
            <img src="images/button_clear.jpg" class="imgover" border="0"/></a>
          </td>
        </tr>
      </table>
    <input type="hidden" name="page" value="y"/>
    </form>
    <p class="note">* Please note that our service is currently only 
                    available in Canada.</p>
  </td>
</tr>
