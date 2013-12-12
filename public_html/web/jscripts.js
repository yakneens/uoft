/*************************
 * CSC309, Assignment 3
 * Sergei Iakhnin
 * 981 843 420
 *************************/
 
 
//retruns true if value is blank
function isBlank(value){return (value == '') ? true : false;}
    
//returns true if c is a digit
function isDigit(c){return (c < "0" || "9" < c) ? false : true;}

//returns true if str is a number
function isNumber(str){
  val=String(str)
  
  i = 0
  
  if (val.charAt(0) == "-") {

    if (val.length > 1) 
      i++;
    else 
      return false;
  
  }    

  for (; i < val.length; i++) {
    ch = val.charAt(i);
    
    if (!isDigit(ch)) 
      return false;
  }
    
  return true

}

/* Checks if c is a letter [a-z] or [A-Z]*/
function isLetter(c){
    return ((c < "a" || "z" < c) && (c < "A" || "Z" < c)) ? false : true;
}


/* Checks if value is a valid Date format (mm/dd/yyyy) */   
function isValidDate(value){
    if (isBlank(value))
        return false;

    strArr = value.split("/");
    if (strArr.length != 3)
    return false;

    for (i=0; i < strArr.length; i++) {
        if (!isNumber(strArr[i]))
            return false;
    }
    
    if ((strArr[2] < 1000) || (strArr[2] > 9999) || (strArr[1] > 12) || 
  (strArr[0] > 31) || (strArr[1] < 1) || (strArr[0] < 1))
        return false;

    if (((strArr[1] == 4) || (strArr[1] == 6) || (strArr[1] == 9) || 
  (strArr[1] == 11)) && (strArr[0] > 30))
        return false;

    if ((strArr[1] == 2) && (strArr[0] > 29))
        return false;

    if ((strArr[1] == 2) && (strArr[0] > 28))
        if ((strArr[2] % 4 != 0) || (strArr[2] % 100 == 0))
            if (strArr[2] % 400 != 0)
                return false;

    return true;
}

/* Checks if value is a valid e-mail (simply checks for @)*/
function isValidEMail(value){
    if (isBlank(value))
        return false;
      
  strArr = value.split("@");

    if (strArr.length != 2)
      return false;
    
    return true;
}

/* 
A valid postal code has the form : X9X[ ]9X9
where X is any letter, 9 is any digit (0..9) and [ ] is an optionnal space.
*/
function isValidPostalCode(value){
    if (isBlank(value))
        return false;
      
    str=String(value);
    
    len = str.length;
    
    if (len < 6 || len > 7)
        return false;
      
    if (!isLetter(str.charAt(0)))
        return false;
      
    if (!isDigit(str.charAt(1)))
        return false;

    if (!isLetter(str.charAt(2)))
        return false;
      
    i = 3;
    
    if (str.charAt(3) == ' ')
        i++;
      
    if (!isDigit(str.charAt(i)))
      return false;
      
    if (!isLetter(str.charAt(i+1)))
        return false;
    
    if (!isDigit(str.charAt(i+2)))
        return false;
      
    return true;

}

//loads a specified URL in this window
function loadURL(url){
  //alert(createQueryString());
  loc = escape(url) + createQueryString();
  window.location = loc;

}



function openParent(url){

  window.parent.location = url;
  window.parent.focus();
  
}


/* Image rollovers */
function initRollovers() {
  if (!document.getElementById) return
  
  var aPreLoad = new Array();
  var sTempSrc;
  var aImages = document.getElementsByTagName('img');

  for (var i = 0; i < aImages.length; i++) {    
    if (aImages[i].className == 'imgover') {
      var src = aImages[i].getAttribute('src');
      var ftype = src.substring(src.lastIndexOf('.'), src.length);
      var hsrc = src.replace(ftype, '_r'+ftype);

      aImages[i].setAttribute('hsrc', hsrc);
      
      aPreLoad[i] = new Image();
      aPreLoad[i].src = hsrc;
      
      aImages[i].onmouseover = function() {
        sTempSrc = this.getAttribute('src');
        this.setAttribute('src', this.getAttribute('hsrc'));
      } 
      
      aImages[i].onmouseout = function() {
        if (!sTempSrc){ 
          sTempSrc = this.getAttribute('src').replace('_r'+ftype, ftype);
        }
        this.setAttribute('src', sTempSrc);
      }
    }
  }
}

/* Validate the billing information form */
function validate(){
    
    if(isBlank(document.billing.fname.value)){
      alert('First Name cannot be empty');
      document.billing.fname.focus();
      return false;
    
    }else if(isBlank(document.billing.lname.value)){
      alert('Last Name cannot be empty');
      document.billing.lname.focus();
      return false;
    
    }else if(isBlank(document.billing.addr.value)){
      alert('Address cannot be empty');
      document.billing.addr.focus();
      return false;
    
    }else if(isBlank(document.billing.city.value)){
      alert('City cannot be empty');
      document.billing.city.focus();
      return false;
    
    }else if(!isValidPostalCode(document.billing.pcode.value)){
      alert('Invalid Postal Code Format');
      document.billing.pcode.focus();
      return false;
    
    }else if(isBlank(document.billing.phone.value)){
      alert('Phone Number cannot be empty');
      document.billing.phone.focus();
      return false;
    
    }else if(!isValidEMail(document.billing.email.value)){
      alert('Invalid Email Format');
      document.billing.email.focus();
      return false;
    
    }else if(isBlank(document.billing.ccard.value)){
      alert('Credit Card Number Cannot be empty');
      document.billing.ccard.focus();
      return false;
    
    }else if(!isValidDate(document.billing.expiry.value)){
      alert('Invalid Date Format');
      document.billing.expiry.focus();
      return false;
    
    }

  return true;
}

//Javascript function that initiates a product listing
//by category.
function leftnav(catid){
  document.getElementById('cid').value = catid;
  document.getElementById('navForm').submit();
  return;
}

//Javascript function that initiates customer checkout.
function checkout(){
  document.getElementById('page').value = "c";
  document.getElementById('navForm').submit();
}

//Javascript function that inititates printable
//summary generation.
function doSummary(){
  document.getElementById('page').value = "y";
  document.getElementById('navForm').submit();
}

//Javascript function that initiates a quantity
//update for a product in the shopping cart.
function updateQuantity(prodID, qstring){
  qty = document.getElementById('prod' + prodID).value;
  
  if(!isNumber(qty) || qty <= 0){
    alert('Quantity must be a positive integer');
    
  }else{
    query = "update-shopping-quantity.jsp?prodID=" + prodID;
    query += "&qty=" + qty + "&" + qstring;
    window.location.href = query;
  
  }
}

/* Initialize rollover script */
window.onload = initRollovers;
