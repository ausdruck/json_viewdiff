<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<link href="css/style.css" rel="stylesheet" type="text/css">
<title>View Diff</title>                                                                  
<meta http-equiv="Content-Type" content="text/html; charset=gb18030"/>  
<?php
	session_start();
	session_unset();
	session_destroy();
?>
</head>		
<body>
        <div class="layout_header">
	  <div class="header"> 
		<div class="h_logo"><a href="#" title="viewdiff"><div style="font-size:250%;color:white">ViewDiff</div></a></div>
		<div class="clear"></div> 
	  </div>
	</div>
	<div align="center">
	<div style="width:700px;padding-top:60px;">
	    <div class="inner">
		<div class="pd10x20">
			<div class="page-title mb20"><i class="i_icon"></i>Login</div>
			<div class="panel">				
				<form action='home.php' method='get' name='reqForm'>
					<table class="table table-striped">
          	  				<thead><tr><th colspan="2">User Information</th></tr></thead>
						<tbody>
							<tr><td style="vertical-align:middle">UserName:</td><td><input type="text" name="user_name"></td></tr>
							<tr><td colspan="2" style="border-top:1px solid #e3e3e3"><a href="javascript:document.reqForm.submit()" class="btn btn-primary"><i class="icon16 icon16-zoom"></i> submit </a></td></tr>
						</tbody>
					</table>
				</form>
			</div>
		</div>
	</div></div>
</body>
</html>


	
