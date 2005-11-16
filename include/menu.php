<table class=menu cellpadding=0 cellspacing=0>
	<!-- OVERVIEW -->
	<tr>
		<? $class = ($page=="overview") ? "menu-item-selected" : "menu-item"; ?>
		<td class=<? echo $class; ?> onMouseOver="menu_hover(this)" onMouseOut="menu_out(this)" onClick="document.location.href='?page=overview'">
			Overview
		</td>
	</tr>

	<!-- TECHNOLOGY -->
	<tr>
		<? $class = ($page=="technology") ? "menu-item-selected" : "menu-item"; ?>
		<td class=<? echo $class; ?> onMouseOver="menu_hover(this)" onMouseOut="menu_out(this)" onClick="document.location.href='?page=technology'">
			Technology
		</td>
	</tr>
	
	<!-- CONTACT -->
	<tr>
		<? $class = ($page=="contact") ? "menu-item-selected" : "menu-item"; ?>
		<td class=<? echo $class; ?> onMouseOver="menu_hover(this)" onMouseOut="menu_out(this)" onClick="document.location.href='?page=contact'">
			Contact
		</td>
	</tr>
	
</table>
