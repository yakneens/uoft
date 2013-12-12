<!-- This is the left navigation bar -->
<table cellpadding="1" cellspacing="0" border="0">
	<tr>
		<td>
			<img src="images/categories_med_shadow.jpg" width="198" height="48" />
		</td>
	</tr>
	<tr>
		<td>
			<ul>
<% 
			//Select all categories in the database and print each category's name
			
			rs = sSQL.executeQuery("select * from categories");
			
			while(rs.next()){
			String cid = rs.getString("catID");
			String name = rs.getString("name");
%>
	<li class="menu">
	<a class="leftnav" href="#" onclick="javascript:leftnav(<%= cid %>)">
		<%= name %>
	</a>
	</li>
<% 		} 

			rs.close();
%>	
	
			</ul>
		</td>
	</tr>
	<tr>
		<td>
<!-- This is the search form -->
			<form id="searchForm" name="searchForm" method="post" action="index.jsp">
				<table cellspacing="0" cellpadding="0" border="0">
					<tr>
						<td align="center" 
								id="searchTop" 
								background="images/search_top.jpg" 
								width="200" 
								height="60" 
								colspan="3">
							<input type="text" name="searchField" />
						</td>
					</tr>
					<tr>
						<td>
							<img src="images/search_left.jpg" />
						</td>
						<td>
							<a 
							href="javascript:document.getElementById('searchForm').submit()">
								<img border="0" 
										 class="imgover" 
										 id="srch" 
										 src="images/search_button.jpg"/>
							</a>
						</td>
						<td>
							<img src="images/search_right.jpg" />
						</td>
					</tr>
				</table>
			<input type="hidden" name="page" value="s"/>
			</form>
		</td>
	</tr>
</table>

	
