<html>
<head>
<title>View Diff</title>
<meta http-equiv="Content-Type" content="text/html; charset=gb18030"/>
<link href="css/style.css" rel="stylesheet" type="text/css">
</head>
<body>
<?php
	session_start();
	$username = $_SESSION['username'];
?>
<div class="layout_header"> 
  <div class="header">
    <div class="h_logo"><a href="#" title="viewdiff"><div style="font-size:250%;color:white">ViewDiff</div></a></div>
    <div class="h_nav"> <span class="hi"><a href="home.php?user_name=<?php echo $username;?>" style="text-decoration: none;color: #fff;"><i class="icon16 icon16-home"></i> Welcome </a><?php echo $username;?>
&nbsp;&nbsp;&nbsp;<a href="login.php" style="text-decoration: none;color: #fff;"><i class="icon16 icon16-power"></i> Logout </a></span></div>
    <div class="clear"></div>
  </div>
</div>
</body>
</html>

