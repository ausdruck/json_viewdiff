<!doctype html>
<html>
<head>
<meta charset="UTF-8">
<title>ViewDiff</title>
<link href="css/style.css" rel="stylesheet" type="text/css">            
<script type="text/javascript" src="js/jquery-1.8.3.min.js"></script>
<script language="javascript">
    $(document).ready(function(){
        parent.document.getElementById("iFrame2").height =document.body.scrollHeight;
    });
</script>
</head>
<body>
<?php
	session_start();
	$username = $_SESSION['username'];
	$newFile = $_SESSION['newFile'];
	$oldFile = $_SESSION['oldFile'];
    $_SESSION['debug_mode']=0;
	$debug_mode = $_SESSION['debug_mode'];
    $is_select=$_SESSION['is_select'];

    $file_path = $_SESSION['job_path']."/"."cmpresult.log";
    $rfile = fopen($file_path,"r");
    $cmpresult = fgets($rfile,102400);
    $json_array = json_decode($cmpresult,true);
    if ($json_array==null){
	    $Tmpcmpresult =iconv('gb18030', 'utf-8//IGNORE', $cmpresult);
	    $json_array = json_decode($Tmpcmpresult,true);
    }
    if ($json_array==null){
	    $Tmpcmpresult =iconv('gbk', 'utf-8//IGNORE', $cmpresult);
	    $json_array = json_decode($Tmpcmpresult,true);
    }
    $select_path = $_SESSION['job_path']."/"."select_file";
    $rfile = fopen($select_path,"r"); 
    $selectlist = fgets($rfile);
    $selectlist = substr($selectlist,8);
    $selectlist = substr($selectlist,0,-1);
    $selectarray=explode(";",$selectlist);
    if($debug_mode)
        var_dump($selectarray);
    function array_to_mainkey($_array,$path,&$result,$selectarray)
    {
        foreach($_array as $key=>$value){
            if($_SESSION['debug_mode'])
            {
                var_dump($key);var_dump($value);
            }
            if(!is_array($value)){
                if($_SESSION['debug_mode'])
                    var_dump("is Not Array!");
                if(strpos($key,"_Main_Key")){
                    $cutpos=strpos($key,"_Main_Key");
                    $key=substr($key,0,$cutpos);
                    if(in_array(strval($path),$selectarray))
                        $result=$result."_".$value;
                    if($_SESSION['debug_mode']){
                        var_dump("-----");
                        var_dump($result);
                    }
                }
            }else
            {
                array_to_mainkey($value,$path."->".$key,$result,$selectarray);
            }
        }
    }
    array_to_mainkey($json_array,"Result",$result,$selectarray);
    $result_array=explode("_",substr($result,1));
    $_SESSION['result_array']=$result_array;
    #var_dump($result_array);
?>
<?php
	if( !isset($_SESSION['total_lines']) ){
		$tmp = $_SESSION['job_path']."/".$_SESSION['oldFile'].".org";
		$total_lines = `wc -l $tmp | awk '{print $1}'`;
		$_SESSION['total_lines'] = $total_lines;
	}else
		$total_lines = $_SESSION['total_lines'];

        if(!$is_select){
		    $tmp = $_SESSION['job_path']."/"."SQ.log";
		    $total_diff_lines =`wc -l $tmp | awk '{print $1}'`;
        }else{               
            $total_diff_lines =count($result_array);
        }
		$_SESSION['total_diff_lines'] = $total_diff_lines;
		
    //var_dump($total_diff_lines);
	$pageCount = 25;
	$allPages = 0;
	$modLines = 0;

	$Page = $_GET['pn'];
	if( !isset($Page) ){
		$Page = 1;
	}
	$_SESSION['pn'] = $Page;
	$querykey=$_GET['querykey'];
	if( !isset($querykey) ){
		$querykey=$_SESSION['querykey'];
	}else
		$_SESSION['querykey']=$querykey;

	if( $total_lines != 0 ){
		$modLines= $total_diff_lines%$pageCount;
		if($modLines>0)
			$allPages = intval($total_diff_lines/$pageCount) + 1;
		else
			$allPages = intval($total_diff_lines/$pageCount);
	}

	if( $Page >= $allPages ){
		$_SESSION['pn'] = $allPages;
	}

	$tmp = $_SESSION['job_path']."/"."QL.log";
	$_SESSION['org_total_lines']=`wc -l $tmp | awk '{print $1}'`;
?>
<div class="panel">
	<div class="panel-header">
	    <div class="mt10">
	    	<span class="ml20" >All diff line: &nbsp;&nbsp;&nbsp;&nbsp;<span style="color:red;font-size:18px"><b> <?php echo $_SESSION['org_total_lines']; ?>  </b></span><span style="font-size:18px"><b>/ <?php echo $_SESSION['total_lines']; ?></b></span></span>
	    </div>
 <?php
    $tmpnew = $_SESSION['job_path']."/"."NMnew.log";
    $tmpold = $_SESSION['job_path']."/"."NMold.log";
    $filenew=fopen($tmpnew,"r");
    $filenewsize=filesize($tmpnew);
    $fileold=fopen($tmpnew,"r");
    $fileoldsize=filesize($tmpold);
    if($filenew && $filenewsize)
    {
	    if($fileold && $fileoldsize)
	    {
		    $tempale_line ="<div class=\"mt10\"><span class=\"ml20\">No Match Line: &nbsp;&nbsp;&nbsp;&nbsp;<a href=\"$tmpnew\">NEWFILE</a> &nbsp;&nbsp;&nbsp;<a href=\"$tmpold\">OLDFILE</a></span></div>";	
	    }
	    else
	    {
	    	$tempale_line ="<div class=\"mt10\"><span class=\"ml20\">No Match Line: &nbsp;&nbsp;&nbsp;&nbsp;<a href=\"$tmpnew\">NEWFILE</a></span></div>";
	    }
	    echo $tempale_line;
    }else if ($fileold && $fileoldsize)
    {
    	$tempale_line ="<div class=\"mt10\"><span class=\"ml20\">No Match Line: &nbsp;&nbsp;&nbsp;&nbsp;<a href=\"$tmpold\">OLDFILE</a></span></div>";
	echo $tempale_line;
    }
    $notjson =$_SESSION['job_path']."/"."Illegal.log";
    $filenj=fopen($notjson,"r");
    $filenjsize=filesize($notjson);
    if($filenj && $filenjsize)
    {
    	$tempale_line ="<div class=\"mt10\"><span class=\"ml20\">Illegal Line: &nbsp;&nbsp;&nbsp;&nbsp;<a href=\"$notjson\">ILLEGAL</a></span></div>";
	echo $tempale_line;
    }
    $duplicate=$_SESSION['job_path']."/"."DuplicateKey.log";
    $filedup=fopen($duplicate,"r");
    $filedupsize=filesize($duplicate);
    if($filedup && $filedupsize)
    {
	$tempale_line ="<div class=\"mt10\"><span class=\"ml20\">Duplicate Key In OneFile: &nbsp;&nbsp;&nbsp;&nbsp;<a href=\"$duplicate\">DUPLICATE</a></span></div>";
	echo $tempale_line;
    }
?>     
	    <div class="mt10">
		<form action='querylist.php' method='get' name='reqForm' >
			<span class="ml20" >Query key: &nbsp;&nbsp;&nbsp;&nbsp;</span>
			<input type="text" class="input" name="querykey" value="<?php echo $querykey;?>">&nbsp;&nbsp;
			<a href="javascript:document.reqForm.submit()" class="btn btn-primary"><i class="icon16 icon16-zoom"></i> submit </a>
		  </form>
	    </div>
	    <div class="panel-main pd10">
    		<table class="table table-striped">
          	  <thead>
            	    <tr>
              	        <th width="60">MainKey</th>
                        <th width="90">ViewKey</th>
	      		<th>DiffKey</th>
	      		<th width="200">Query</th>
              		<th width="90" style="padding-left:15px;">Operation</th>
                    </tr>
	         </thead>
	         <tbody>
<?php 
	  $tempale_line = "<tr><td><a href=\"singlequery.php?mainkey={MainKey}&diff={DiffList}&view={ViewKey}\"> {MainKey}</a></td><td>{ViewKey}</td><td>{DiffList}</td><td>{query}</td><td class=\"i-operate\" style=\"text-align:center;\"><a href=\"singlequery.php?mainkey={MainKey}&diff={DiffList}&view={ViewKey}\" title=\"show\">show</a></td></tr>";
	  $tmp = $_SESSION['job_path']."/"."QL.log";
	  $tmp1 =$_SESSION['job_path']."/"."SQ.log";
	  $difflistfile = fopen($tmp,"r");
	  if(!$difflistfile&&($_SESSION['total_lines']!=0))
		  echo 'open QueryListfile fail~';
	  $i =$pageCount;
	  $line_num = 0;
	  $line=fgets($difflistfile, 2048);
      if(!$is_select){
	  while(!feof($difflistfile)&&$i>0){
		  $line_num++;
		  if($line_num>$pageCount*$_SESSION['pn']){
			  break;
		  }
		  if($line_num<=$pageCount*($_SESSION['pn']-1)){
			  $line=fgets($difflistfile, 2048);
			  continue;
		  }
          	  if($line == iconv('utf-8', 'gb18030//IGNORE', iconv('gb18030', 'utf-8//IGNORE', $line)))
          	  {
			  $line=iconv('gb18030', 'utf-8//IGNORE', $line);
			  $linedetail = explode("difflist:",$line);
			  $tmpkey=iconv('utf-8', 'gb18030//IGNORE',$linedetail[0]);
         	  }else{
		  	$linedetail = explode("difflist:",$line);
		  	$tmpkey=$linedetail[0];
		  }
		  $singleline =`grep '^$tmpkey' $tmp1`;
		  #var_dump($singleline);
		  #if($singleline == iconv('utf-8', 'gb18030//IGNORE', iconv('gb18030', 'utf-8//IGNORE', $singleline)))
		  #	  $singleline=iconv('gb18030', 'utf-8//IGNORE', $singleline);
		  $querystr=strstr($singleline,"\"".$querykey."\":");
		  $pos1=strpos($querystr,",");$pos2=strpos($querystr,"}");	
		  if($pos1<$pos2) $pos=$pos1;
		  else $pos=$pos2;
		  $query=substr($querystr,strlen($querykey)+3,$pos-strlen($querykey)-3);
		  #var_dump($query);
		  if($query == iconv('utf-8', 'gb18030//IGNORE', iconv('gb18030', 'utf-8//IGNORE',$query)))
			$query=iconv('gb18030', 'utf-8//IGNORE', $query);  
		  $next=str_replace("{ViewKey}",$linedetail[0],$tempale_line,$i);
		  $next=str_replace("{query}",$query,$next);
		  $next=str_replace("{MainKey}",$line_num,$next);
		  $result=str_replace("{DiffList}",$linedetail[1],$next);
		  echo $result;
          $i--;
		  $line=fgets($difflistfile, 2048);
	  }
      }else{
	      foreach($result_array as $searchkey)
	      {
	          if($i>0)
		  {
			$line=`grep "^$searchkey difflist" $tmp`;
			if($line==NULL)
			{
				$searchkey=iconv('utf-8', 'gb18030//IGNORE',$searchkey);
				$line=`grep "^$searchkey difflist" $tmp`;
			}
			if($line == iconv('utf-8', 'gb18030//IGNORE', iconv('gb18030', 'utf-8//IGNORE', $line))){
				$line=iconv('gb18030', 'utf-8//IGNORE', $line);
				$linedetail = explode("difflist:",$line);
				$tmpkey=iconv('utf-8', 'gb18030//IGNORE',$linedetail[0]);
			}else{
				$linedetail = explode("difflist:",$line);
				$tmpkey=$linedetail[0];
			}
			$line_num++;
			if($line_num>$pageCount*$_SESSION['pn']){
				break;
			}
			if($line_num<=$pageCount*($_SESSION['pn']-1)){
				continue;
			}
			$singleline =`grep '^$tmpkey' $tmp1`;
			$querystr=strstr($singleline,"\"".$querykey."\":");
			$pos1=strpos($querystr,",");$pos2=strpos($querystr,"}");
			if($pos1<$pos2) $pos=$pos1;
			else $pos=$pos2;
			$query=substr($querystr,strlen($querykey)+3,$pos-strlen($querykey)-3);
			if($query == iconv('utf-8', 'gb18030//IGNORE', iconv('gb18030', 'utf-8//IGNORE',$query)))
				$query=iconv('gb18030', 'utf-8//IGNORE', $query);
			$next=str_replace("{ViewKey}",$linedetail[0],$tempale_line,$i);
			$next=str_replace("{query}",$query,$next);
			$next=str_replace("{MainKey}",$line_num,$next);
			$result=str_replace("{DiffList}",$linedetail[1],$next);
			echo $result;
			$i--;
		  }
	      }
      }
      fclose($difflistfile);
?>
          </tbody>
	</table>
        <div class="list-page pd10">
	    <div class="i-total">Total Page number: <b><?php echo $allPages?></b></div>
	    <div class="i-list" id="pagelists"></div>
            <div class="clear"></div>
	</div>
      </div>
    </div>
</div>
<script type="text/javascript">
//container 容器，count 总页数 pageindex 当前页数
function setPage(container, count, pageindex) {
	var container = container;
	var count = count;
	var pageindex = pageindex;
	var a = [];
	if (pageindex == 1) {
		a[a.length] = "<a style=\"color:#ccc\">&lt;</a>";
	} else {
		a[a.length] = "<a href=\"?pn="+(pageindex-1)+"\">&lt;</a>";
	}
	function setPageList() {
	    if (pageindex == i) {
		a[a.length] = "<a href=\"?pn="+i+"\" class=\"active\">" + i + "</a>";
	    } else {
		a[a.length] = "<a href=\"?pn="+i+"\">" + i + "</a>";
	    }
	}
	if(count>=5)
	{
	if (pageindex <= 4) {
	    for (var i = 1; i <= 5; i++) {
		setPageList();
	    }
	} else if (pageindex >= count - 3) {
	    for (var i = count - 4; i <= count; i++) {
		setPageList();
	    }
	}
	else {
	    for (var i = pageindex - 2; i <= pageindex + 2; i++) {
		setPageList();
	    }
	}
	}else
	{
	    for (var i = 1; i <= count; i++) { 
		setPageList();
	    }  
	}
	if (pageindex == count) {
		a[a.length] = "<a style=\"color:#ccc\">&gt;</a>";
	} else {
		a[a.length] = "<a href=\"?pn="+(pageindex+1)+"\">&gt;</a>";
	}
	container.innerHTML = a.join("");	
}
setPage(document.getElementById("pagelists"),<?php echo $allPages;?>,<?php echo $Page;?>);
</script>
</body>
</html>
