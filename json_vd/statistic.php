<html>
<head>
<link href="css/style.css" rel="stylesheet" type="text/css">
<script type="text/javascript">
function   DisSelectAll()
{
	all=document.getElementsByTagName("input");
	for(i=0;i<all.length;i++)
	{
		if(all.item(i).name!="cb_samename"&&all.item(i).name!="is_select")
			all.item(i).checked=false;
	}
	document.reqForm.submit();
}
</script>
</head>
<body>
<div class="panel"><div class="panel-header">
<form action='statistic.php' method='post' name='reqForm' >
<div style="padding-left:10px;padding-top:10px;"><input type="radio" name="is_select" value="0" class="radio radio-first" <?php $tmp=$_POST["is_select"]==0?"checked":""; echo $tmp;?>>mask<input type="radio" name="is_select" value="1" class="radio" <?php $tmp=$_POST["is_select"]==1?"checked":""; echo $tmp;?>>select</div>
<div style="padding-left:10px;padding-top:10px;"><input type="checkbox" name="cb_samename" onclick="DisSelectAll()" <?php $tmp=isset($_POST["cb_samename"])?"checked":""; echo $tmp;?> > Select same name node</div>
<div style="padding:10px;">
  <a href="javascript:document.reqForm.submit();window.parent.frames[1].location.reload()" class="btn btn-primary"><i class="icon16 icon16-zoom"></i> submit selected nodes </a>
</div>
<?php
    session_start();
    if($_POST["is_select"]==1) 
        $_SESSION['is_select']=1;
    else if($_POST["is_select"]==0)
        $_SESSION['is_select']=0;
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
    $_SESSION['debug_mode']=0;
    $debug_mode=$_SESSION['debug_mode'];
    if($debug_mode)
	    var_dump($json_array);
    function array_to_tree($_array,$root){
	    $result = array();
	    foreach($_array as $key=>$value){ 
		    if($_SESSION['debug_mode'])
		    {
			    var_dump($key);var_dump($value);
		    }
		    if(!is_array($value)){
			    if($_SESSION['debug_mode'])
				    var_dump("is Not Array!");
			    if(!strpos($key,"_Main_Key")){
				    $cutpos=strpos($key,"_number");
				    $key=substr($key,0,$cutpos);
				    if($root==NULL)$result[$key]=array("root",$key,$key."[".$value."]",true,"bfly.gif");
				    else $result[$key]=array($root,$key,$key."[".$value."]",true,"bfly.gif");
				    if($_SESSION['debug_mode'])
					    var_dump($result[$key]);
			    }
		    }else{
			    if($_SESSION['debug_mode'])
				    var_dump("is Array!");
			    $result[$key]=array_to_tree($value,$key);
		    }
	    }
	    if($_SESSION['debug_mode']){
		    var_dump("Returned result!:");var_dump($result);
	    }
	    return $result;
    }
    $treearr=array_to_tree($json_array,NULL);
    if($debug_mode)
	    var_dump($treearr);
    require "./lib/KoolTreeView/kooltreeview.php";
    $_node_template = "<input type='checkbox' id='cb_{id}' name='cb_{name}' {check} onclick='toogle(\"{id}\")'><label for='cb_{id}'>{text}</label>";
    $treeview = new KoolTreeView("treeview");
    $treeview->scriptFolder = "./lib/KoolTreeView";
    $treeview->imageFolder= "./lib/KoolTreeView/icons";
    $treeview->styleFolder = "default";
    $root = $treeview->getRootNode();

    $root->text = str_replace("{id}","treeview.root",$_node_template);
    $root->text = str_replace("{name}","treeview_root",$root->text);
    $root->text = str_replace("{text}","Result",$root->text);
    if(isset($_POST["first_time"])){
	    $root->text = str_replace("{check}",isset($_POST["cb_treeview_root"])?"checked":"",$root->text);
    }else{
	    $root->text = str_replace("{check}","checked",$root->text);
	    $_POST["first_time"]=true;
    }
    $root->expand=true;
    $root->image="woman2S.gif";
    function treearr_to_view($_tree,$root,&$treeview,&$arr)
    {
	    $_node_template = "<input type='checkbox' id='cb_{id}' name='cb_{name}' {check} onclick='toogle(\"{id}\")'><label for='cb_{id}'>{text}</label>";
	    if($root==NULL)$root="root";
	    foreach ($_tree as $key=>&$value){
		    $flag=false;
		    if($_SESSION['debug_mode']){
			    var_dump($key);var_dump($value);
		    }
		    if(is_array($value)){
			    if($_SESSION['debug_mode'])
				    var_dump("Is Array!");
			    foreach ($value as $key_=>$value_)
			    {
				    if($_SESSION['debug_mode']){
					    var_dump("--------");var_dump($key_);var_dump($value_);
				    }
				    if(!is_array($value_)){$flag=false;break;}
				    if(!$flag){
                        //$key=str_replace(" ","_",$key);
					    $temp=$key;
					    while($arr[$key]!="")
					    {
					    	$arr[$key]=$arr[$key]+1;
						    $key=$key.$arr[$key];
					    }
					    $arr[$key]=1;
					    if($temp!=$key)
					    {
					    	$value[$temp][1]=$key;
					    }
					    #$_text = str_replace("{id}",$key,$_node_template);
			    		    #if(!isset($_POST["cb_samename"])){
					    #	$_text = str_replace("{name}",$key,$_text);
					    #}else
					    #	$_text = str_replace("{name}",$temp,$_text);
					    #$_text = str_replace("{text}",$key,$_text);
					    #$_text = str_replace("{check}",isset($_POST["cb_".$key])?"checked":"",$_text);
					    $tmp = $treeview->Add($root,$key,$_text,true,"");
					    if($_SESSION['debug_mode']){
						    var_dump($root);var_dump($key);var_dump($_text);
					    }
					    $tmp->expand=true;
					    if($_SESSION['debug_mode'])
						    var_dump("ตÝน้!");
					    treearr_to_view($value,$key,$treeview,$arr);
					    $flag = true;
				    }
			    }
			    if($flag){
				    if($_SESSION['debug_mode'])
					    var_dump("Flag is True!Continue!");
				    continue;
			    }
			    $_text = str_replace("{id}",$_tree[$key][1],$_node_template);
			    if(!isset($_POST["cb_samename"])){
			    	$_text = str_replace("{name}",$_tree[$key][1],$_text);
			    }else
				$_text = str_replace("{name}",$_tree[$key][0],$_text);
			    $_text = str_replace("{text}",$_tree[$key][2],$_text);
			    if(!isset($_POST["cb_samename"])){
			    	$_text = str_replace("{check}",isset($_POST["cb_".str_replace(" ","_",$_tree[$key][1])])?"checked":"",$_text);
			    }else
				$_text = str_replace("{check}",isset($_POST["cb_".str_replace(" ","_",$_tree[$key][0])])?"checked":"",$_text);
			    $treeview->getNode($_tree[$key][1])->text=$_text;
			    if($_SESSION['debug_mode'])
				    var_dump($treeview->getNode($_tree[$key][1])->text);
		    }
	    }
    }
    if($_SESSION['debug_mode'])
	    var_dump($treearr);
    treearr_to_view($treearr,NULL,$treeview,$arr);
    if($_SESSION['debug_mode']){
	    var_dump($treearr);var_dump($treeview);
    }
    $treeview->showLines = true;
    $treeview->selectEnable = false;
    $treeview->keepState = "onpage";
?>
<?php
    if( $_SESSION['username'] == '' ){
	    exit;
    }
    $select_all = $_GET['select_all'];
    $statistic_mode = $_GET['statistic_mode'];
    $change_mode = $_GET['change_mode'];
    if( !isset($statistic_mode) ){
	    if( !isset($_SESSION['statistic_mode']) ){
		    $_SESSION['statistic_mode'] = 1;
	    }
	    $statistic_mode = $_SESSION['statistic_mode'];
    }else{
	    $_SESSION['statistic_mode'] = $statistic_mode;
    }
    $c_mode = $statistic_mode ^ 1;
?>
    <div style="padding:10px;">
	<?php echo $treeview->Render();?> 
    </div>
    <script language="javascript">
    function markChild(nodeId)
    {
    	var status = document.getElementById("cb_" + nodeId ).checked;
	var childIds = treeview.getNode(nodeId).getChildIds();
	for(var i in childIds)
	{
		document.getElementById("cb_" + childIds[i] ).checked = status;
		markChild(childIds[i]);
	}
    }
    function markParent(nodeId)
    {
    	if (nodeId.indexOf(".root")<0)
	{
		var parentId = treeview.getNode(nodeId).getParentId();
		var siblingIds = treeview.getNode(parentId).getChildIds();
		var _parentSelected = true;
		for(var i in siblingIds)
			if (!document.getElementById("cb_" + siblingIds[i]).checked)
				 _parentSelected = false;
		document.getElementById("cb_" + parentId).checked = _parentSelected;
		markParent(parentId);
	}
    }
    function toogle(nodeId)
    {
    	markChild(nodeId);
	markParent(nodeId);
    }
    </script>
    <div style="padding:10px;">
    <b>Selected node are:</b> <br/>
    <?php
	$index = 1;
	function selected_id($_tree_key,$_tree_arr,$path,&$mask,&$arr){
		foreach($_tree_arr as $key=>$value){
			if($_SESSION['debug_mode']){
				var_dump($key);var_dump($_tree_key);var_dump($value);
			}
			if($key==$_tree_key)
			{
				if($_SESSION['debug_mode'])
					var_dump("11111111");
				if(!isset($_POST["cb_samename"])){
				while($arr[$key]!="")
				{
					$arr[$key]=$arr[$key]+1;
					$key=$key.$arr[$key];
				}}
				$arr[$key]=1;
				if(isset($_POST["cb_".str_replace(" ","_",$key)]))
				{
					echo $path."<br/>"; 
					$mask=$mask.";".$path;
				}
			}
			else if (is_array($value)){
				if($_SESSION['debug_mode'])
					var_dump("22222222");
				selected_id($key,$value,$path."->".$key,$mask,$arr);
			}
		}
	}
    if($_POST["is_select"]==0)
	    $mask_="mask diff:";
    else if($_POST["is_select"]==1)
        $mask_="select:"; 
	if($_SESSION['debug_mode'])
		var_dump($treearr);
	selected_id("",$treearr,"Result",$mask_,$arr2);
	if($_SESSION['debug_mode'])
		var_dump($arr2);
	if(isset($_SESSION["mask"])){
		if($_SESSION["mask"]==$mask_)
			$_SESSION["mask_changed"] = 0;
		else
		{
			$_SESSION["mask_changed"] = 1;
			$_SESSION["mask"]=$mask_;
		}
	}
    if($_POST["is_select"]==0)
	    exec("echo \"".$mask_."\" > ".$_SESSION['job_path']."/".$_SESSION['user_ip']."/mask_file");
    else if($_POST["is_select"]==1)
        exec("echo \"".$mask_."\" > ".$_SESSION['job_path']."/".$_SESSION['user_ip']."/select_file");
    ?>
    </div>
</form>
</div></div>
</body>
</html>



