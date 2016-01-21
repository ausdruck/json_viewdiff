<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta charset="UTF-8">
<script type="text/javascript"> 
var iframe;     
function load() {
	iframe = document.getElementById("iFrame2");
	iframe. onload = iframe. onreadystatechange = iframeload;           
	}     
function iframeload() {     
	if (!iframe.readyState || iframe.readyState == "complete") {     
	//alert("Local iframe is now loaded.");     
	} 	
}     
</script> 
<link href="css/style.css" rel="stylesheet" type="text/css">
<?php
    //header('Content-type:text/html;charset=utf-8');
    session_start();
    session_unset();
    session_destroy();
    session_start();
    $_SESSION['job_flag'] = $_GET['job_flag'];
    $_SESSION['username'] = $_GET['username']; 
    $_SESSION['debug_mode'] = 0;
    $_SESSION['mode'] = 1;
    $_SESSION['job_path']=$_SESSION['username']."/".$_SESSION['job_flag']; 
    $_SESSION['newFile']="new_input";
    $_SESSION['oldFile']="old_input";   
?>
<title>View Diff</title>                                                                  
<meta http-equiv="Content-Type" content="text/html; charset=gb18030"/>   
</head>		
<body onload="load()">
        <div class="layout_header">
	  <div class="header"> 
		<div class="h_logo"><a href="#" title="viewdiff"><div style="font-size:250%;color:white">ViewDiff</div></a></div>
		<div class="h_nav"> <span class="hi"><a href="home.php?user_name=<?php echo $_SESSION['username'];?>" style="text-decoration: none;color: #fff;"><i class="icon16 icon16-home"></i> Welcome </a><?php echo $_SESSION['username'];?>&nbsp;&nbsp;&nbsp;<a href="login.php" style="text-decoration: none;color: #fff;"><i class="icon16 icon16-power"></i> Logout </a></span></div>
		<div class="clear"></div> 
	  </div>
	</div>
	<div class="layout_leftnav">
	  <div class="nav-vertical">
	 	<div class="pd10x20">
			<div class="page-title mb20" ><i class="i_icon"></i> Statistic Information</div>
			<iframe src="statistic.php" id="iFrame1" name="iFrame1" frameborder="no" border="0" onload="this.height=iFrame1.document.body.scrollHeight" width="100%"></iframe>
		</div>
	  </div>
	</div>
	<div class="layout_rightmain">
	  <div class="inner"><div class="pd10x20">
		<div class="page-title mb20"><i class="i_icon"></i> Query List</div>
		<iframe src="querylist.php" id="iFrame2" name="iFrame2" frameborder="no" border="0" onload="this.height=iFrame2.document.body.scrollHeight" width="100%"></iframe>	
	  </div></div>
	</div>
</body>				 
</html>

