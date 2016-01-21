<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<link href="css/style.css" rel="stylesheet" type="text/css">
<?php
    session_start();
    $_SESSION = array();
    if (isset($_COOKIE[session_name()])) {
        setcookie(session_name(), '', time()-42000, '/');
    }
    if(isset($_REQUEST['user_name']))
	    $_SESSION['username'] = $_REQUEST['user_name'];
    else{
	    header("Location: login.php");
	    exit; 
    }
    $_SESSION['newFile'] = "new_input";
    $_SESSION['oldFile'] = "old_input";
    $_SESSION['debug_mode'] = 0;
    if( !isset($_SESSION['job_flag']) ){
        $_SESSION['current_job'] = date('YmdHis',time());
    }else{
        $_SESSION['current_job'] = $_SESSION['job_flag'];
    }

    if( isset($_POST['new_file']) ){
        $_SESSION['wget_newFile'] = $_POST["new_file"];
    }
    if( isset($_POST['old_file']) ){
        $_SESSION['wget_oldFile'] = $_POST["old_file"];
    }
    
    if( isset($_POST['main_key']) ){
        $_SESSION['mainkey'] = $_POST["main_key"];
    }else
	$_SESSION['mainkey'] = "";
    if( isset($_POST['new_file'])&&isset($_POST['old_file']) ){
	$_SESSION['job_path'] = $_SESSION['username']."/".$_SESSION['current_job'];
    	$newFile = $_SESSION['newFile'];
    	$oldFile = $_SESSION['oldFile'];
	$mainkey = $_SESSION['mainkey'];
	if( !is_dir($_SESSION['job_path']) ){
		exec("mkdir -p ".$_SESSION['job_path']);
	}
        // get file
	$notrealpath=exec("wget ".$_SESSION['wget_newFile']." -O ".$_SESSION['job_path']."/".$_SESSION['newFile'].";echo $?");
	if(!$notrealpath)
	{
		$notrealpath=exec("wget ".$_SESSION['wget_oldFile']." -O ".$_SESSION['job_path']."/".$_SESSION['oldFile'].";echo $?");
		if(!$notrealpath)
		{
        		exec("mv ".$_SESSION['job_path']."/".$_SESSION['newFile']." ".$_SESSION['job_path']."/".$_SESSION['newFile'].".org"); 
        		exec("mv ".$_SESSION['job_path']."/".$_SESSION['oldFile']." ".$_SESSION['job_path']."/".$_SESSION['oldFile'].".org");

			$cmd_viewdiff = "./main -s ".$_SESSION['job_path']."/".$_SESSION['oldFile'].".org "." ".$_SESSION['job_path']."/".$_SESSION['newFile'].".org -k ".$_SESSION['mainkey']." 2> ".$_SESSION['job_path']."/viewdiff_error.log";
        		exec($cmd_viewdiff);
			exec("mv *.log  ".$_SESSION['job_path']);
			header("Location:pi_get.php?username=".$_SESSION["username"]."&job_flag=".$_SESSION["current_job"]);
			exit;
		}
	}
	exec("rm -rf ".$_SESSION['job_path']);
	echo "<script>alert('Invalid Wget Path!');</script>";	
    }
    
?>
<title>View Diff</title>                                                                  
<meta http-equiv="Content-Type" content="text/html; charset=gb18030"/>    
</head>		
<body>
        <div class="layout_header">
	  <div class="header"> 
		<div class="h_logo"><a href="#" title="viewdiff"><div style="font-size:250%;color:white">ViewDiff</div></a></div>
		<div class="h_nav"> <span class="hi"><i class="icon16 icon16-home"></i> Welcome <?php echo $_SESSION['username'];?>&nbsp;&nbsp;&nbsp;<a href="login.php" style="text-decoration: none;color: #fff;"><i class="icon16 icon16-power"></i> Logout</a> </span></div>
		<div class="clear"></div> 
	  </div>
    	</div>
	<div align="center">
	  <div style="width:700px;padding-top:60px;">
		<div class="inner">
		    <div class="pd10x20">
		        <div class="page-title mb20"><i class="i_icon"></i>View Diff</div>
			<div class="panel"><form action="" method="post" name="reqForm"><input type="text" name="user_name" style="display:none"value=<?php echo $_SESSION['username'];?>>
			    <table class="table table-striped">
				<thead><tr><th colspan="3" >Choose Files To View Diff</th></tr></thead>
				<tbody>
				    <tr><td style="vertical-align:middle">New File : (new file, wget path)</td><td><input type="text" name="new_file"></td><td rowspan="3" style="border-left:1px solid #e3e3e3;vertical-align:middle" align="center"><a href="javascript:document.reqForm.submit()" class="btn btn-primary"><i class="icon16 icon16-zoom"></i> submit </a></td></tr>
				    <tr><td style="vertical-align:middle">Old File : (old file, wget path)</td><td><input type="text" name="old_file"></td></tr>
				    <tr><td style="vertical-align:middle">File Key : (unique identifier)</td><td><input type="text" name="main_key"></td></tr>
				</tbody>
			    </table></form>
			</div>
			<div class="page-title mb20" style="padding-top:20px"><i class="i_icon"></i>Job List &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;<a href="delfile.php?dirname=chen&type=1&user_name=chen" style="font-size:14px">Delete ALL !!!</a></div>
			<div class="panel">
			    <table class="table table-striped">
				<thead><tr><th>Job Sign</th><th>Operation</th><th>Date</th></tr></thead>
				<tbody>
     <?php
        if( isset($_SESSION['username']) ){

            exec("rm ".$_SESSION['username']."/mask_file");
            if(!is_dir($_SESSION['username']) ){
                mkdir($_SESSION['username']);
            }

            $username = $_SESSION['username'];

	    $dir = opendir($username);
	    $file = null;
            while( ($file = readdir($dir)) != false){
                $basename = basename($file);
                if( !($basename == "." || $basename == "..") && is_dir($username."/".$basename) ){
                echo "<tr><td>".$basename."</td><td> <a href=\"pi_get.php?username=".$_SESSION['username']."&job_flag=".$basename."\" target=\"top\"> show </a> &nbsp; &nbsp;<a href=\"delfile.php?dirname=".$username."/".$basename."&user_name=".$_SESSION['username']."\">delete</a></td><td>".date("Y-m-d H:i:s",filemtime($username."/".$basename))."</td></tr>";
                }
                
            }
	    closedir($dir);
	}
    ?> 
 
				</tbody>	
		    </div>
		</div>
	  </div>
	</div>
</body>
</html>	
