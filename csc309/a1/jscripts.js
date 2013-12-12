//array to keep track of the shopping cart throughout the site
var shoppingCart = new Array(5);

for(i = 0; i < shoppingCart.length; i++)
	shoppingCart[i] = new Product(null,"","","","","",0,0,"");

//Parses the query string into an associative array
//according to name=value pairs
//Note: does not do white-space checking in URL
function ParseQueryString(){
 		
		// an array for the (name,value) pairs to go in:
	 qstring = new Array();
 		
		// split the query string into an array of pairs:
	 pairs = document.location.href.split('?')[1].split('&');
	 
	 for(var i = 0; i < pairs.length; i++){
		 	//split each (name,value) pair on the = sign
		 	thisPair = pairs[i].split('=');
		 	
			//store the values
			qstring[thisPair[0]] = thisPair[1];
	 	}
	 
	 return qstring;
	 
}

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


//opens a pop-up window
function popup(filename, h, w, resizable, scrollbars, top, left){ 

	popup = window.open(filename,'popDialog','height='+h+',width='+w+',resizable='+resizable+',scrollbars='
+scrollbars+',menubar=yes,status=yes, top='+top+", left="+left);

}

//loads a specified URL in this window
function loadURL(url){
	//alert(createQueryString());
	loc = escape(url) + createQueryString();
	window.location = loc;

}

//returns the child of the element with ID = id 
function getChild(id){
  if(document.getElementById) 
		return document.getElementById(id).firstChild;
  
	return null;
}

//Returns the length of the text field in the element whose ID = id
function textLength(id){
  
	child = getChild(id);
	
 	if(child) 
		return(child.length);
  	else 
		return null;
}

//Replaces the value of the text field in the element whose ID = id
//starting from position start and ending in position end with 
//string text.
function textReplace(id,start,end,text){
  
	child = getChild(id);
	
  if(child && child.replaceData) 
		child.replaceData(start,end,text);
  else 
		return null;
}

//creates a string in the form of a query string
//which contains the id's of all products stored in the shopping cart
function createQueryString(){
	
	qstring = "?"
	
	for(i = 0; i < shoppingCart.length; i++){
		qstring =qstring + "id" + i + "=" + shoppingCart[i].id + "&";
		//alert(shoppingCart[i].id);
	}
		
	return qstring;

}


//populates the bigger version of the shopping cart
//when the document loads
function populateBigCart(){
	
	qstring = ParseQueryString();
		
	for(slotId in qstring){
		
		if(qstring[slotId] != "null"){
			thisProduct = getProduct(qstring[slotId]);
			shoppingCart[slotId.substring(2)] = thisProduct;
		}
	
		renderBigCart();
	}
	
}
	
//populates the smaller version of the shopping cart
//when the document loads
function populateCart(){
	qstring = ParseQueryString();
		
	for(slotId in qstring){
		
		if(qstring[slotId] != "null"){
			thisProduct = getProduct(qstring[slotId]);
			shoppingCart[slotId.substring(2)] = thisProduct;
		}
	
		renderCart();
	}

}

function renderCart(){

	for(i = 0; i<shoppingCart.length;i++){
	
		val = shoppingCart[i].manufacturer + " " + shoppingCart[i].name;
		textReplace("id"+i,0,textLength("id"+i),val);
	}

}

function renderBigCart(){

	for(i = 0; i < shoppingCart.length; i++){
		val = shoppingCart[i].manufacturer + " " + shoppingCart[i].name;
		textReplace("id"+i,0,textLength("id"+i),val);
		textReplace("pr"+i,0,textLength("pr"+i),"$" + shoppingCart[i].price);
	
	}
	
}

//add a product with ID = prodId to shopping cart
//if it's not in the cart already and if there is space
function addToCart(prodId){
	
	if(!isDuplicate(prodId)){
		
			//find a free slot in the cart
			indx = findFreeSlot();
		
		
			//get the product record corresponding to this product
			thisProduct = getProduct(prodId);
			shoppingCart[indx] = thisProduct;
			
			//display the information about this product in the cart
			if(indx != null){
				renderCart();
			
			}else{
			
				alert('cart is full');
		
			}
		
		}else{
		
		alert('That item already exists in your cart');
		
	}

}

//find a slot in the shopping cart which contains no item
function findFreeSlot(){
	
	for(i = 0; i <= 4; i++){
		
		if(shoppingCart[i].id == null)
			return i;
				
	}
		
	return null;
	
}
	
//returns the product recrod for a product with ID=prodId
function getProduct(prodId){
	
	for(i = 0; i < myProducts.length; i++){
		if(myProducts[i].id == prodId)
			return myProducts[i];
			
	}
		
	return null;
	
}
		
//remove element which resides in slot slotNum from cart
function removeFromCart(slotNum){
		
	if(confirm('Are you sure you want to remove this item?')){
	
		shoppingCart[slotNum] = new Product(null,"","","","","",0,0,"");
			
		textReplace("id" + slotNum,0,textLength("id" + slotNum),"");
		
	}
	
}

//remove element which resides in slot slotNum from cart
function removeFromBigCart(slotNum){
		
	if(confirm('Are you sure you want to remove this item?')){
	
		shoppingCart[slotNum] = new Product(null,"","","","","",0,0,"");
			
		textReplace("id" + slotNum,0,textLength("id" + slotNum),"");
		textReplace("pr" + slotNum,0,textLength("pr"+slotNum),"$0");
	}
	
}
	
//determine whether a product with ID = prodId is currently in the cart
function isDuplicate(prodId){

	for(i = 0; i < shoppingCart.length; i++){

		if(shoppingCart[i].id != null && shoppingCart[i].id == prodId)
			return true;
		
	}
	
	return false;
	
}
	

//calculate the total price of all the items in the shopping cart
//according to the quantity selected
function calculateTotal(){
	
	totalPrice = 0;
	
	for(indx in shoppingCart){
		
		if(indx){
			quantity = document.getElementById("qty"+indx);
			if(!isBlank(quantity.value) && isNumber(quantity.value) && quantity.value >= 0){
			
				totalPrice += quantity.value * shoppingCart[indx].price;
			
			}else{
				alert('Quantity must be non-negative integer');
				return;
			}
		
		}
	
	}
	
	return totalPrice;
	
}


function displayTotal(){

	textReplace("totalAmount",0,textLength("totalAmount"), "Total Amount is: $" + calculateTotal());

}

function doSearch(){
	qrystr ="search.html?";
	search_items = document.getElementById("searchVal").value;
	search_arr = search_items.split(" ");
	for(indx in search_arr){
		qrystr = qrystr + indx + "=" + search_arr[indx] +  "&";
		
	}
	
	qrystr = qrystr + createQueryString().substring(1);
	popup(qrystr, 400, 400, 'no', 'yes', 400, 600);

}


function openParent(url){

	window.parent.location = url;
	window.parent.focus();
	
}



