function doSearch() {
	result = document.getElementById("searchVal");
	//alert(result.value);
	values = result.value.split(" ");
	for(index in values)
		alert(values[index]);

}