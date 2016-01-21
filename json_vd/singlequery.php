<html>
<head>
<meta charset="UTF-8">
<title>ViewDiff</title>
<link href="css/style.css" rel="stylesheet" type="text/css"> 
<link rel="stylesheet" href="../jquery.treeview.css" />
<link rel="stylesheet" href="../red-treeview.css" />
<style type="text/css"> 
#Float {position: absolute;top: 110px;right: 23px;z-index:999;}
</style>
</head>
<script type="text/javascript" src="js/jquery-1.8.3.min.js"></script>
<script language="javascript"> 
$(document).ready(function(){
	$(window.parent).scroll(function (){
		var offsetTop = $(window).scrollTop() + 200 +"px";
		$("#Float").animate({top : offsetTop },{ duration:300 , queue:false });
	});
    parent.document.getElementById("iFrame2").height =document.body.scrollHeight; 
});
</script> 
<script src="../lib/jquery.js" type="text/javascript"></script>
<script src="../lib/jquery.cookie.js" type="text/javascript"></script>
<script src="../jquery.treeview.js" type="text/javascript"></script>
<script type="text/javascript">
	$(function() {
		$("#tree").treeview({
			collapsed: true,
			animated: "medium",
			control:"#sidetreecontrol",
			persist: "location"
		});
	})
	$(function() {
		$(".open").treeview({
			collasped: false
		});
	})
	$(function() {
		$(".mask").css('color', '#F0F0F0');
	})
	$(function() {
		$(".openvalue").css('color', 'red');
	})
	$(function() {
		$("#tree2").treeview({
			collapsed: true,
			animated: "fast",
			unique: true,
			persist: "location",
			toggle: function() {
				window.console && console.log("%o was toggled", this);
			}
		});
	})
	$(document).ready(function(){
		$("#navigation").treeview({
			animated: "fast",
			collapsed: true,
			unique: true,
			persist: "location"
		});
	})
</script>
<body> 
<?php
	session_start();
?>
<div width=50 id="Float">
    <table class="table table-striped">
    <thead><tr><th>OPERATION</th></tr></thead>
    <tr><td style="border:1px solid #6e9e9e">
    <?php
	$mode = null;
	$c_mode = null;
	if (!isset($_GET['mode'])){
		$mode = $_SESSION['mode'];
	}else{
		$mode = $_GET['mode'];
		$_SESSION['mode'] = $mode;
	}
	$c_mode = $mode^1;
    ?>
    <a href="?key=<?php echo $_GET['key'];?>&mainkey=<?php echo $_GET['mainkey'];?>&diff=<?php echo $_GET['diff'];?>&view=<?php echo $_GET['view'];?>&mode=<?php echo $c_mode;?>"><?php
	if( $mode )
		echo "Full Mode";
	else
		echo "Simple Mode";
    ?></a>
    </td></tr><tr><td style="border:1px solid #6e9e9e">
    <a href="querylist.php?pn=<?php echo $_SESSION['pn'];?>">Back To QueryList</a></td></tr>
    <?php
	$nextline_num=$_GET['mainkey']+1;
	$prevline_num=$_GET['mainkey']-1;
	if($_SESSION['is_select']==0)
	{
	    //var_dump($nextline_num);
	    $tmp = $_SESSION['job_path']."/"."QL.log";
	    $next_tmp_QL=$nextline_num."p";
	    $next_QL=`sed -n '$next_tmp_QL' $tmp`;
	    //var_dump($next_QL);
	    $prev_tmp_QL=$prevline_num."p";
	    $prev_QL=`sed -n '$prev_tmp_QL' $tmp`;
	    //var_dump($prev_QL);
	}
	else if($_SESSION['is_select']==1)
	{ 
	    $next_viewkey=$_SESSION['result_array'][$nextline_num-1]; 
	    //var_dump($next_viewkey);
	    $prev_viewkey=$_SESSION['result_array'][$prevline_num-1];
	    //var_dump($prev_viewkey);
	    $tmp = $_SESSION['job_path']."/"."QL.log";
	    $difflistfile = fopen($tmp,"r");
	    $line=fgets($difflistfile, 2048);
	    if($line == iconv('utf-8', 'gb18030//IGNORE', iconv('gb18030', 'utf-8//IGNORE',$line)))
	    {
		$next_viewkey=iconv('utf-8', 'gb18030//IGNORE',$next_viewkey);
	    	$prev_viewkey=iconv('utf-8', 'gb18030//IGNORE',$prev_viewkey); 	    
	    }
	    fclose($difflistfile);
	    $next_viewkey_tmp=$next_viewkey." difflist";
	    $next_QL=`grep '^$next_viewkey difflist' $tmp`;
	    //var_dump($next_QL);
	    $prev_QL=`grep '^$prev_viewkey difflist' $tmp`;
	    //var_dump($prev_QL);
	    if($prevline_num==0) $prev_QL=NULL;
	    if($nextline_num==count($_SESSION['result_array'])+1) $next_QL=NULL;
	}
	if($next_QL == iconv('utf-8', 'gb18030//IGNORE', iconv('gb18030', 'utf-8//IGNORE',$next_QL)))
	{
		$next_QL=iconv('gb18030', 'utf-8//IGNORE', $next_QL);
	}
	if($prev_QL == iconv('utf-8', 'gb18030//IGNORE', iconv('gb18030', 'utf-8//IGNORE',$prev_QL)))
		$prev_QL=iconv('gb18030', 'utf-8//IGNORE', $prev_QL);
	$next_linedetail = explode("difflist:",$next_QL);
	$next_ViewKey=$next_linedetail[0];
	$next_MainKey=$nextline_num;
	$next_DiffList=$next_linedetail[1];
	$prev_linedetail = explode("difflist:",$prev_QL);
	$prev_ViewKey=$prev_linedetail[0];
	$prev_MainKey=$prevline_num;
	$prev_DiffList=$prev_linedetail[1];
    	if($prev_QL!=NULL)
	{
	    echo "<tr><td style=\"border:1px solid #6e9e9e\"><a href=\"singlequery.php?mainkey=".$prev_MainKey."&diff=".$prev_DiffList."&view=".$prev_ViewKey."\">Prev</a></td></tr>";
	} 
	if($next_QL!=NULL)
	{
	    echo "<tr><td style=\"border:1px solid #6e9e9e\"><a href=\"singlequery.php?mainkey=".$next_MainKey."&diff=".$next_DiffList."&view=".$next_ViewKey."\">Next</a></td></tr>";
	}
?>
    </table>
</div>
	<div class="panel"><div class="panel-header">
	<div class="panel-main pd10">
	MainKey:&nbsp;&nbsp;<span style="font-size:16px"><?php echo $_GET['mainkey']; ?> </span>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;ViewKey:&nbsp;&nbsp;<span style="font-size:16px"><?php echo $_GET['view']; ?></span>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;DiffKey:&nbsp;&nbsp;<span style="font-size:16px"><?php echo $_GET['diff']; ?></span>
	</div>
	<div class="panel-main pd10">
	 <table class="table table-striped">
           <thead>
             <tr>
		<th>Diff New Old</th>
	     </tr>
           </thead>
	   <tbody>
	     <tr><td> 
<?php
	$target_key = $_GET["view"]; 
	$newFile = $_SESSION['newFile'];
	$oldFile = $_SESSION['oldFile'];
	$jobpath = $_SESSION['job_path'];
	$debug_mode = $_SESSION['debug_mode'];
	$tmp_SQ = $jobpath."/SQ.log"; 
	$tmp_target_key=$target_key." ";
    #var_dump($tmp_target_key);
	$awk_SQ = "'{if($1==\"".$tmp_target_key."\") print substr($0,length(\"".$target_key."\")+2);}'";
	$result_SQ_num = `grep "$target_key" $tmp_SQ | awk -F '{' $awk_SQ`;
    #var_dump($result_SQ_num);
	if($result_SQ_num==null)
    {
        $target_key=iconv('utf-8', 'gb18030//IGNORE', $target_key);
        $tmp_target_key=iconv('utf-8', 'gb18030//IGNORE', $tmp_target_key);
        $awk_SQ = "'{if($1==\"".$tmp_target_key."\") print substr($0,length(\"".$target_key."\")+2);}'";
        $result_SQ_num = `grep "$target_key" $tmp_SQ | awk -F '{' $awk_SQ`;
    }
    #var_dump($result_SQ_num);
    $mask_path=$jobpath."/mask_file";
	$rfile = fopen($mask_path,"r");
	$masklist = fgets($rfile);
	$masklist = substr($masklist,11);
	$masklist = substr($masklist,0,-1);
	$result_SQ_num=str_replace('\u0002',"<em>",$result_SQ_num);
	$result_SQ_num=str_replace('\u0003',"</em>",$result_SQ_num);
	$result_SQ_num=str_replace('\u0004',"<b>",$result_SQ_num);
	$result_SQ_num=str_replace('\u0005',"</b>",$result_SQ_num);
	#var_dump($result_SQ_num);
	$json_result_SQ = json_decode($result_SQ_num,true);
	#var_dump($json_result_SQ);
	if ($json_result_SQ==null){
		$result_SQ = iconv('gb18030', 'utf-8//IGNORE', $result_SQ_num);
		#$result_SQ = mb_convert_encoding($result_SQ_num,'utf-8//IGNORE','gb18030');
		#var_dump($result_SQ);
		$json_result_SQ = json_decode($result_SQ,true);
	}
    #var_dump($json_result_SQ);
	if($json_result_SQ==null)
	{
		$result_SQ = iconv('gbk', 'utf-8//IGNORE', $result_SQ_num);
		$json_result_SQ = json_decode($result_SQ,true);
	}
    #var_dump($json_result_SQ);
	if($debug_mode)
		var_dump($json_result_SQ);
	function json_to_tree($path,$_array,$maskarray,$mask,$mode){
		$result=array();
		foreach($_array as $key=>$value){
			if($_SESSION['debug_mode']){
				var_dump($key);var_dump($value);var_dump($mask);
			}
			$tmpkey=$key;
			$pos=strpos($key,"[single]");
			if($pos)
				$tmpkey=substr($tmpkey,0,$pos-5);
			if($_SESSION['debug_mode'])
				var_dump($tmpkey);
			if(is_array($value)){
				if (in_array(strval($path."->".$tmpkey),$maskarray))
				{
					$result_child = json_to_tree($path."->".$tmpkey,$value,$maskarray,1,$mode);
				}else
					$result_child = json_to_tree($path."->".$tmpkey,$value,$maskarray,$mask,$mode);
				$temp=$result[1];
				if($result_child[0]==true)
				{
					$result[0]=true;
					$result[1]=$result[1]."<li class=\"open\"><span>".$key."</span><ul id=\"tree\">".$result_child[1]."</ul></li>";
				}else if(strval($key)=="diff_new" ||strval($key)=="diff_old"){
					$result[0]=true;
					$result[1]=$result[1]."<li class=\"open\"><span class=\"openvalue\">".$key."</span><ul id=\"tree\">".$result_child[1]."</ul></li>";
				}else{
					if($mode)
					{
						if(strstr($key,"[single]"))
						{
							$result[0]=true;
							$result[1]=$result[1]."<li><span class=\"openvalue\">".$key."</span><ul id=\"tree\">".$result_child[1]."</ul></li>";
						}else{
							$result[0]=(false or $result[0]);
							$result[1]=$result[1]."<li><span>".$key."</span><ul id=\"tree\">".$result_child[1]."</ul></li>";
						}
					}else
					{
						if(strstr($key,"[single]"))
						{
							$result[0]=true;
							$result[1]=$result[1]."<li class=\"open\"><span class=\"openvalue\">".$key."</span><ul id=\"tree\">".$result_child[1]."</ul></li>";
						}else
						{
							$result[0]=(false or $result[0]);
							$result[1]=$result[1]."<li class=\"open\"><span>".$key."</span><ul id=\"tree\">".$result_child[1]."</ul></li>";
						}
					}
				}
				if($mask || in_array(strval($path."->".$tmpkey),$maskarray))
				{
					$result[1]=$temp."<li><span class=\"mask\">".$key."</span><ul id=\"tree\"><li class=\"mask\">".$result_child[1]."</li></ul></li>";
				}
			}elseif(is_bool($value)){
				$temp=$result[1];
				if($mode)
				{
					if (!strstr($key,"[single]"))
					{
						if($value)
							$result[1]=$result[1]."<li><span>".$key."</span><ul id=\"tree\"><li>true</li></ul></li>";
						else
							$result[1]=$result[1]."<li><span>".$key."</span><ul id=\"tree\"><li>false</li></ul></li>";
					}else{
						$result[0]=true;
						if($value)
						{
							$result[1]=$result[1]."<li><span class=\"openvalue\">".$key."</span><ul id=\"tree\"><li>true</li></ul></li>";
						}else{
							$result[1]=$result[1]."<li><span class=\"openvalue\">".$key."</span><ul id=\"tree\"><li>false</li></ul></li>";
						}
					}
				}else{
					if (!strstr($key,"[single]"))
					{
						if($value)
							$result[1]=$result[1]."<li class=\"open\"><span>".$key."</span><ul id=\"tree\"><li>true</li></ul></li>";
						else
							$result[1]=$result[1]."<li class=\"open\"><span>".$key."</span><ul id=\"tree\"><li>false</li></ul></li>";
					}
					else
					{
						$result[0]=true;
						if($value)
							$result[1]=$result[1]."<li class=\"open\"><span class=\"openvalue\">".$key."</span><ul id=\"tree\"><li>true</li></ul></li>";
						else
							$result[1]=$result[1]."<li class=\"open\"><span class=\"openvalue\">".$key."</span><ul id=\"tree\"><li>false</li></ul></li>";
					}
				}
				if($mask || in_array(strval($path."->".$tmpkey),$maskarray))
				{
					if($value)
						$result[1]=$temp."<li><span class=\"mask\">".$key."</span><ul id=\"tree\"><li class=\"mask\">true</li></ul></li>";
					else
						$result[1]=$temp."<li><span class=\"mask\">".$key."</span><ul id=\"tree\"><li class=\"mask\">false</li></ul></li>";
				}
			}elseif(is_string(strval($value))){
				$temp=$result[1];
				if(count(explode("-->",$value))>1){
					$result[0]=true;
					$result[1]=$result[1]."<li class=\"open\"><span>".$key."</span><ul id=\"tree\"><li class=\"openvalue\">".$value."</li></ul></li>";
				}else if(strval($key)=="diff_new" ||strval($key)=="diff_old")
				{
					$result[0]=true;
					$result[1]=$result[1]."<li class=\"open\"><span class=\"openvalue\">".$key."</span><ul id=\"tree\"><li>".$value."</li></ul></li>";
				}else {
					if($mode)
					{
						if(!strstr($key,"[single]")){
							$result[0]=($result[0] or false);
							$result[1]=$result[1]."<li><span>".$key."</span><ul id=\"tree\"><li>".$value."</li></ul></li>";
						}else{
							$result[0]=true;
							$result[1]=$result[1]."<li><span class=\"openvalue\">".$key."</span><ul id=\"tree\"><li>".$value."</li></ul></li>";
						}
					}else{
						if(!strstr($key,"[single]")){
							$result[0]=($result[0] or false);
							$result[1]=$result[1]."<li class=\"open\"><span>".$key."</span><ul id=\"tree\"><li>".$value."</li></ul></li>";
						}else{
							$result[0]=true;
							$result[1]=$result[1]."<li class=\"open\"><span class=\"openvalue\">".$key."</span><ul id=\"tree\"><li>".$value."</li></ul></li>";
						}
					}
				}
				if($mask || in_array(strval($path."->".$tmpkey),$maskarray))
				{
					$result[1]=$temp."<li><span class=\"mask\">".$key."</span><ul id=\"tree\"><li class=\"mask\">".$value."</li></ul></li>";
				}
			}
			if($_SESSION['debug_mode']){
				var_dump("-------------");var_dump($result);
			}
		}
		return $result;
	}
	$maskarray=explode(";",$masklist);
	if($debug_mode)
	{
		var_dump($maskarray);
	}
	$return = json_to_tree("Result",$json_result_SQ,$maskarray,0,$mode);
	echo "<ul id=\"tree\">".$return[1]."</ul>";
?>
	</td></tr></tbody>
	</table>
	</div>
    </div>
</div>
</body>
</html>
