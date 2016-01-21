<?php
$user_name=$_GET['user_name'];
$fn=$_GET['dirname'];
$type=$_GET['type'];
if( isset($type) and $type=="1"){
    exec("rm -rf ".$fn."/*");
}
if( isset($fn) ){
    exec("rm -rf ".$fn);
}
header("location:home.php?user_name=".$user_name);
?>
