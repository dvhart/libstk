<?php
	if (!isset($page)) $page="overview";
?>
<html>
<head>
	<link rel=stylesheet type=text/css href=/include/theme.css>
	<!-- link rel=icon type=image/gif href=/images/icon.gif -->
	<script type=text/javascript language=javascript src=/include/javascript.js></script>
	<title>libstk::home</title>
</head>
	<body>
		<table class=main border=0 cellspacing=0 cellpadding=0>
			<!-- HEADER -->
			<tr height=50>
				<td class=header colspan=3><? include("include/header.php"); ?></td>
			</tr>
			<!-- MENU | SPACER | BODY -->
			<tr height=380>
				<td class=menu-box><? include("include/menu.php"); ?></td>
				<td width=20></td>
				<td class=body-box><? include($page.".php"); ?></td>
			</tr>
			<!-- FOOTER -->
			<tr>
				<td class=footer colspan=3><? include("include/footer.php"); ?></td>
			</tr>
		</table>
	</body>
</html>
